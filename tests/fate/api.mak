fate-api-pgs-rate-control: $(APITESTSDIR)/api-pgs-rate-control-test$(EXESUF)
fate-api-pgs-rate-control: CMD = run $(APITESTSDIR)/api-pgs-rate-control-test$(EXESUF)
fate-api-pgs-rate-control: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-fade
fate-api-pgs-fade: $(APITESTSDIR)/api-pgs-fade-test$(EXESUF)
fate-api-pgs-fade: CMD = run $(APITESTSDIR)/api-pgs-fade-test$(EXESUF)
fate-api-pgs-fade: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-dts
fate-api-pgs-dts: $(APITESTSDIR)/api-pgs-dts-test$(EXESUF)
fate-api-pgs-dts: CMD = run $(APITESTSDIR)/api-pgs-dts-test$(EXESUF)
fate-api-pgs-dts: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-overlap-verify
fate-api-pgs-overlap-verify: $(APITESTSDIR)/api-pgs-overlap-verify-test$(EXESUF)
fate-api-pgs-overlap-verify: CMD = run $(APITESTSDIR)/api-pgs-overlap-verify-test$(EXESUF)
fate-api-pgs-overlap-verify: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-multi-object
fate-api-pgs-multi-object: $(APITESTSDIR)/api-pgs-multi-object-test$(EXESUF)
fate-api-pgs-multi-object: CMD = run $(APITESTSDIR)/api-pgs-multi-object-test$(EXESUF)
fate-api-pgs-multi-object: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-ap-interval
fate-api-pgs-ap-interval: $(APITESTSDIR)/api-pgs-ap-interval-test$(EXESUF)
fate-api-pgs-ap-interval: CMD = run $(APITESTSDIR)/api-pgs-ap-interval-test$(EXESUF)
fate-api-pgs-ap-interval: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-forced
fate-api-pgs-forced: $(APITESTSDIR)/api-pgs-forced-test$(EXESUF)
fate-api-pgs-forced: CMD = run $(APITESTSDIR)/api-pgs-forced-test$(EXESUF)
fate-api-pgs-forced: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-palette-delta
fate-api-pgs-palette-delta: $(APITESTSDIR)/api-pgs-palette-delta-test$(EXESUF)
fate-api-pgs-palette-delta: CMD = run $(APITESTSDIR)/api-pgs-palette-delta-test$(EXESUF)
fate-api-pgs-palette-delta: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-palette-reuse
fate-api-pgs-palette-reuse: $(APITESTSDIR)/api-pgs-palette-reuse-test$(EXESUF)
fate-api-pgs-palette-reuse: CMD = run $(APITESTSDIR)/api-pgs-palette-reuse-test$(EXESUF)
fate-api-pgs-palette-reuse: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-animation-timing
fate-api-pgs-animation-timing: $(APITESTSDIR)/api-pgs-animation-timing-test$(EXESUF)
fate-api-pgs-animation-timing: CMD = run $(APITESTSDIR)/api-pgs-animation-timing-test$(EXESUF)
fate-api-pgs-animation-timing: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-animation-util
fate-api-pgs-animation-util: $(APITESTSDIR)/api-pgs-animation-util-test$(EXESUF)
fate-api-pgs-animation-util: CMD = run $(APITESTSDIR)/api-pgs-animation-util-test$(EXESUF)
fate-api-pgs-animation-util: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-coalesce
fate-api-pgs-coalesce: $(APITESTSDIR)/api-pgs-coalesce-test$(EXESUF)
fate-api-pgs-coalesce: CMD = run $(APITESTSDIR)/api-pgs-coalesce-test$(EXESUF)
fate-api-pgs-coalesce: CMP = null

FATE_API_LIBAVCODEC-$(CONFIG_PGSSUB_ENCODER) += fate-api-pgs-rectsplit
fate-api-pgs-rectsplit: $(APITESTSDIR)/api-pgs-rectsplit-test$(EXESUF)
fate-api-pgs-rectsplit: CMD = run $(APITESTSDIR)/api-pgs-rectsplit-test$(EXESUF)
fate-api-pgs-rectsplit: CMP = null
