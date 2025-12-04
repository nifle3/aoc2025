lib_common = static_library(
  'common', 
  'utils.cc', 
  include_directories: include_directories('.')
)

dep_common = declare_dependency(
  link_with: lib_common,
  include_directories: include_directories('.')
)
