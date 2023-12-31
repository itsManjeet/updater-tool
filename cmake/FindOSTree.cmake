find_package(PkgConfig REQUIRED)
pkg_check_modules(OSTREE REQUIRED ostree-1)
add_library(OSTREE INTERFACE IMPORTED)
set_property(TARGET OSTREE PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OSTREE_INCLUDE_DIRS})
set_property(TARGET OSTREE PROPERTY INTERFACE_LINK_LIBRARIES ${OSTREE_LIBRARIES})
set_property(TARGET OSTREE PROPERTY INTERFACE_COMPILE_OPTIONS ${OSTREE_CFLAGS_OTHER})