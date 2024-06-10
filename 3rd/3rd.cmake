#
# raylib
#
set(RAYLIB_DIR ${CMAKE_CURRENT_LIST_DIR}/prebuilt/raylib)

add_library(3rd::raylib STATIC IMPORTED)

set_target_properties(3rd::raylib PROPERTIES
    IMPORTED_LOCATION ${RAYLIB_DIR}/lib/libraylib.a
    INTERFACE_INCLUDE_DIRECTORIES ${RAYLIB_DIR}/include
)

#
# box2d
#
set(BOX2D_DIR ${CMAKE_CURRENT_LIST_DIR}/prebuilt/box2d)

add_library(3rd::box2d STATIC IMPORTED)

set_target_properties(3rd::box2d PROPERTIES
    IMPORTED_LOCATION ${BOX2D_DIR}/lib/libbox2d.a
    INTERFACE_INCLUDE_DIRECTORIES ${BOX2D_DIR}/include
)
