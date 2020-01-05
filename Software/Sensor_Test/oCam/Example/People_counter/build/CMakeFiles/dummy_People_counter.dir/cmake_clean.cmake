file(REMOVE_RECURSE
  "devel/include/People_counter/camConfig.h"
  "devel/share/People_counter/docs/camConfig.dox"
  "devel/share/People_counter/docs/camConfig-usage.dox"
  "devel/lib/python2.7/dist-packages/People_counter/cfg/camConfig.py"
  "devel/share/People_counter/docs/camConfig.wikidoc"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/dummy_People_counter.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
