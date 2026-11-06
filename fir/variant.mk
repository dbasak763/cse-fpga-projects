HLS ?= vitis_hls
.PHONY: test report cosim
test:
	$(MAKE) -C ../.. test
report:
	cd ../.. && VARIANT=$(VARIANT) $(HLS) -f fir/scripts/run_hls.tcl
cosim:
	cd ../.. && VARIANT=$(VARIANT) COSIM=1 $(HLS) -f fir/scripts/run_hls.tcl
