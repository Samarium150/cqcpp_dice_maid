cmake_minimum_required(VERSION 3.9)
project(cqcpp_dice_maid)

set(CMAKE_CXX_STANDARD 17) # Set C++ standard as C++17, manditory

include_directories(src) # set src directory as included, mutable
include_directories(extern/cqcppsdk/include) # set SDK directory as included, manditory

include(extern/cqcppsdk/cqcppsdk.cmake) # include SDK CMake script, manditory
file(GLOB_RECURSE SOURCE_FILES src/*.cpp) # recursively search source file in src, mutable

# read app id from app_id.txt
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)

cq_set_app_id(${APP_ID}) # set app id, manditory

find_package(boost REQUIRED)
find_path(VCPKG_INCLUDE_DIR boost)
include_directories(${VCPKG_INCLUDE_DIR})

include(CMakeRC.cmake)
cmrc_add_resource_library(
    app-resources 
    ALIAS app::rc
    NAMESPACE app
)

if (CQ_CAN_BUILD_STD_MODE)
    cq_add_std_app(${SOURCE_FILES}) # build target in std mode
    target_link_libraries(app ${Boost_LIBRARIES})
    target_link_libraries(app PRIVATE app::rc)
    cq_install_std_app()
endif ()

# build target in dev mode
cq_add_dev_app(${SOURCE_FILES})
target_link_libraries(app_dev ${Boost_LIBRARIES})
target_link_libraries(app_dev PRIVATE app::rc)

# add unit test, mutable
enable_testing()
add_subdirectory(extern/cqcppsdk/tests)
