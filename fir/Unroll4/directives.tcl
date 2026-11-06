set_directive_pipeline -off "decimate/output_loop"
set_directive_pipeline -II 1 "decimate/load_loop"
set_directive_array_partition -type cyclic -factor 4 -dim 1 decimate samples
set_directive_array_partition -type complete -dim 1 decimate coefficients
set_directive_unroll -factor 4 "decimate/tap_loop"
set_directive_pipeline -II 1 "decimate/tap_loop"
