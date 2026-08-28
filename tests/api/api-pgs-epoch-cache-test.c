/*
 * PGS encoder epoch palette-cache regression test.
 *
 * The decoder discards its palette at every Epoch Start, so an entry
 * that is transparent when a new epoch opens must be re-transmitted
 * once it turns opaque again, even when its colour matches a value
 * the previous epoch carried. The delta encoder's cache mirrors what
 * the decoder has received in the current epoch only; a stale entry
 * from the previous epoch would suppress the re-transmission and the
 * player would render the event with the wrong colour.
 *
 * Copyright (c) 2026 David Connolly
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <string.h>

#include "libavcodec/avcodec.h"
#include "libavutil/mem.h"

#include "pgs-test-util.h"

/* True when the PDS payload carries an entry with the given id and
 * alpha. Entries are 5 bytes: id, Y, Cr, Cb, A. */
static int pds_has_entry(const uint8_t *pds, uint8_t id, uint8_t alpha)
{
    int payload = (pds[1] << 8) | pds[2];
    int n       = (payload - 2) / 5;
    const uint8_t *e = pds + 5;
    int i;

    for (i = 0; i < n; i++, e += 5)
        if (e[0] == id && e[4] == alpha)
            return 1;
    return 0;
}

int main(void)
{
    const AVCodec *codec;
    AVCodecContext *ctx = NULL;
    uint8_t *buf;
    int ret = 0, size;
    const uint8_t *pcs, *pds;

    uint8_t indices[20];
    uint32_t pal_red[8] = {
        0xFF101010, 0xFFFF0000, 0xFF00FF00, 0xFF0000FF,
        0xFFFFFF00, 0xFFFF00FF, 0xFF00FFFF, 0xFFFFFFFF,
    };
    uint32_t pal_gone[8];
    uint32_t pal_back[8];
    AVSubtitle sub;
    AVSubtitleRect rect;

    /* entry 1 transparent at the new epoch, others unchanged */
    memcpy(pal_gone, pal_red, sizeof(pal_red));
    pal_gone[1] = 0x00000000;
    /* entry 1 back to the epoch-one red value */
    memcpy(pal_back, pal_red, sizeof(pal_red));

    memset(indices, 0, sizeof(indices));
    indices[0]  = 1;
    indices[5]  = 2;
    indices[10] = 3;
    indices[15] = 4;

    buf = av_malloc(1024 * 1024);
    if (!buf) {
        fprintf(stderr, "Failed to allocate buffer\n");
        return 1;
    }

    codec = avcodec_find_encoder(AV_CODEC_ID_HDMV_PGS_SUBTITLE);
    if (!codec) {
        fprintf(stderr, "PGS encoder not found\n");
        av_free(buf);
        return 1;
    }

    ctx = avcodec_alloc_context3(codec);
    if (!ctx)
        return 1;
    ctx->width     = 1920;
    ctx->height    = 1080;
    ctx->time_base = (AVRational){ 1, 90000 };
    if (avcodec_open2(ctx, codec, NULL) < 0) {
        fprintf(stderr, "Failed to open encoder\n");
        avcodec_free_context(&ctx);
        return 1;
    }

    /* Step 1: first epoch, entry 1 opaque red. The full PDS carries
     * it and the decoder receives it. */
    setup_subtitle(&sub, &rect, indices, pal_red, 100, 800, 8);
    size = avcodec_encode_subtitle(ctx, buf, 1024 * 1024, &sub);
    if (size <= 0) {
        fprintf(stderr, "Step 1: encode failed (%d)\n", size);
        ret = 1;
        goto end;
    }
    pcs = find_segment(buf, size, 0x16);
    pds = find_segment(buf, size, 0x14);
    if (!pcs || !pds || pcs[10] != 0x80) {
        fprintf(stderr, "Step 1: expected an Epoch Start with a PDS\n");
        ret = 1;
        goto end;
    }
    if (!pds_has_entry(pds, 0x01, 0xFF)) {
        fprintf(stderr, "Step 1: PDS lacks entry 1\n");
        ret = 1;
        goto end;
    }
    printf("Step 1: epoch one carries entry 1\n");
    cleanup_subtitle(&sub);

    /* Step 2: the object height changes, which opens a second epoch.
     * Entry 1 is transparent now, so nothing transmits it; the
     * decoder discards its palette and holds no value for entry 1. */
    setup_subtitle(&sub, &rect, indices, pal_gone, 100, 800, 8);
    rect.h = 5;
    size = avcodec_encode_subtitle(ctx, buf, 1024 * 1024, &sub);
    if (size <= 0) {
        fprintf(stderr, "Step 2: encode failed (%d)\n", size);
        ret = 1;
        goto end;
    }
    pcs = find_segment(buf, size, 0x16);
    pds = find_segment(buf, size, 0x14);
    if (!pcs || !pds || pcs[10] != 0x80) {
        fprintf(stderr, "Step 2: dimension change did not open an "
                "epoch\n");
        ret = 1;
        goto end;
    }
    if (pds_has_entry(pds, 0x01, 0xFF)) {
        fprintf(stderr, "Step 2: transparent entry 1 was transmitted\n");
        ret = 1;
        goto end;
    }
    printf("Step 2: epoch two opened, entry 1 untransmitted\n");
    cleanup_subtitle(&sub);

    /* Step 3: entry 1 returns to the epoch-one red value. The delta
     * PDS must carry it: the decoder never received entry 1 in this
     * epoch. A cache that survived the epoch boundary suppresses the
     * write and this assertion fails. */
    setup_subtitle(&sub, &rect, indices, pal_back, 100, 800, 8);
    rect.h = 5;
    size = avcodec_encode_subtitle(ctx, buf, 1024 * 1024, &sub);
    if (size <= 0) {
        fprintf(stderr, "Step 3: encode failed (%d)\n", size);
        ret = 1;
        goto end;
    }
    pcs = find_segment(buf, size, 0x16);
    pds = find_segment(buf, size, 0x14);
    if (!pcs || !pds || pcs[10] != 0x00 || pcs[11] != 0x80) {
        fprintf(stderr, "Step 3: expected a palette update on a "
                "normal display set\n");
        ret = 1;
        goto end;
    }
    if (!pds_has_entry(pds, 0x01, 0xFF)) {
        fprintf(stderr, "Step 3: stale cache dropped entry 1 from the "
                "delta PDS\n");
        ret = 1;
        goto end;
    }
    printf("Step 3: delta PDS carries entry 1\n");

    printf("\nEpoch palette-cache test passed.\n");

end:
    av_freep(&sub.rects);
    avcodec_free_context(&ctx);
    av_free(buf);
    return ret;
}
