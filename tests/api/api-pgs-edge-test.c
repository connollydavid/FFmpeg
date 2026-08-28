/*
 * Edge-case coverage for the PGS subtitle encoder:
 *  1. a rect with a full 256-colour palette encodes through PCS and
 *     PDS, exercising the PDS writer's worst-case size guard
 *  2. a zero-dimension rect encodes without crashing (output may be empty)
 *
 * Historical note: an early draft of this test segfaulted on the
 * full-palette case because the harness itself never allocated its
 * output buffer; the encoder was never at fault, and the PDS writer
 * bounds-checks the worst case before writing.
 *
 * Copyright (c) 2026 David Connolly
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libavutil/error.h"
#include "libavutil/mem.h"

#include "pgs-test-util.h"

static AVCodecContext *ctx;
static uint8_t *buf;
static int buf_size = 1 << 20;
static uint32_t palette[256];
static uint8_t indices[64 * 64];

static int encode_rect(int64_t pts, int x, int y, int w, int h, int nc)
{
    AVSubtitle sub;
    AVSubtitleRect rect;

    memset(&sub, 0, sizeof(sub));
    memset(&rect, 0, sizeof(rect));
    sub.num_rects = 1;
    sub.rects = av_mallocz(sizeof(*sub.rects));
    if (!sub.rects)
        return AVERROR(ENOMEM);
    sub.rects[0] = &rect;
    sub.start_display_time = 0;
    sub.end_display_time   = 3000;
    sub.pts                = pts;
    rect.type              = SUBTITLE_BITMAP;
    rect.x                 = x;
    rect.y                 = y;
    rect.w                 = w;
    rect.h                 = h;
    rect.nb_colors         = nc;
    rect.data[0]           = indices;
    rect.linesize[0]       = w;
    rect.data[1]           = (uint8_t *)palette;
    rect.linesize[1]       = 4 * 256;

    int ret_size = avcodec_encode_subtitle(ctx, buf, buf_size, &sub);
    av_freep(&sub.rects);
    return ret_size;
}

int main(void)
{
    const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_HDMV_PGS_SUBTITLE);
    int ret = 0, size;

    if (!codec) {
        fprintf(stderr, "PGS encoder not found\n");
        return 1;
    }
    buf = av_mallocz(buf_size);
    if (!buf)
        return 1;
    ctx = avcodec_alloc_context3(codec);
    if (!ctx)
        return 1;
    ctx->width     = 1920;
    ctx->height    = 1080;
    ctx->time_base = (AVRational){ 1, 90000 };
    if (avcodec_open2(ctx, codec, NULL) < 0) {
        fprintf(stderr, "Failed to open encoder\n");
        return 1;
    }

    for (int i = 0; i < 256; i++)
        palette[i] = 0xFF000000u | ((i * 7) << 16) | ((i * 13) << 8) | (i * 29);
    for (int k = 0; k < 64 * 64; k++)
        indices[k] = k % 256;

    /* Test 1: full 256-entry palette through PCS and PDS. */
    size = encode_rect(90000, 100, 100, 64, 64, 256);
    if (size <= 0) {
        fprintf(stderr, "Test 1: full-palette rect failed (%d)\n", size);
        return 1;
    }
    printf("Test 1: full-palette rect -- encoded %d bytes OK\n", size);

    /* Test 2: zero-dimension rect must not crash (output may be empty). */
    size = encode_rect(180000, 0, 0, 0, 0, 4);
    printf("Test 2: zero-dimension rect returned %d (no crash = pass)\n", size);

    avcodec_free_context(&ctx);
    return ret;
}
