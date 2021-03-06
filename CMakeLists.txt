cmake_minimum_required(VERSION 3.9)
project(cqcpp_dice_maid) # 填写项目名称

set(CMAKE_CXX_STANDARD 17) # 设置 C++ 标准版本为 C++17, 必须

include_directories(src) # 设置 src 目录为包含目录, 可根据实际情况修改
include_directories(extern/cqcppsdk/include) # 设置 SDK 的 include 目录为包含目录, 必须

include(extern/cqcppsdk/cqcppsdk.cmake) # 包含 SDK 的 CMake 脚本, 必须
file(GLOB_RECURSE SOURCE_FILES src/*.cpp) # 递归搜索 src 目录中的源文件, 可根据实际情况修改

# 从 app_id.txt 文件读取 app id, 也可以直接设置
file(READ "app_id.txt" APP_ID)
string(STRIP "${APP_ID}" APP_ID)

cq_set_app_id(${APP_ID}) # 设置 app id, 必须

find_package(boost REQUIRED)
find_path(VCPKG_INCLUDE_DIR boost)
include_directories(${VCPKG_INCLUDE_DIR})

include(CMakeRC.cmake)
cmrc_add_resource_library(
	app-resources 
	ALIAS app::rc
	NAMESPACE app
	resources/maid.txt
	resources/manual.txt
	resources/madness.txt
	resources/tarot.txt
)

if (CQ_CAN_BUILD_STD_MODE)
	add_definitions(-DSTD)
	cq_add_std_app(${SOURCE_FILES}) # 添加 std 模式的动态链接库构建目标
	target_link_libraries(app ${Boost_LIBRARIES})
	target_link_libraries(app app::rc)
	cq_install_std_app("D:/Program Files/CQA")
endif ()

# 添加 dev 模式的可执行文件构建目标
cq_add_dev_app(${SOURCE_FILES})
target_link_libraries(app_dev ${Boost_LIBRARIES})
target_link_libraries(app_dev app::rc)

# 添加单元测试，如果无需测试，可删除
# enable_testing()
# add_subdirectory(extern/cqcppsdk/tests)
