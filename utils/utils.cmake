
set(BOX2D_DEBUG ${CMAKE_CURRENT_LIST_DIR}/box2d_debug_draw)
add_library(Utils::Box2dDebug INTERFACE IMPORTED)
target_sources(Utils::Box2dDebug INTERFACE ${BOX2D_DEBUG}/box2d_debug.cpp)
target_include_directories(Utils::Box2dDebug INTERFACE ${BOX2D_DEBUG})