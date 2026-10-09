file(REMOVE_RECURSE
  "libavif_internal.a"
  "libavif_internal.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/avif.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
