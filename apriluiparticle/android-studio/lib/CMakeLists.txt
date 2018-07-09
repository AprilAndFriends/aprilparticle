cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

project(apriluiparticle)

include("../../../../hltypes/android-studio/generic/CMakeLists.txt")

file(
	GLOB_RECURSE CppSrc
	"../../src/*.c"
	"../../src/*.cpp"
)

add_library(apriluiparticle STATIC ${CppSrc})

include_directories(AFTER "../../include/apriluiparticle")
include_directories(AFTER "../../../../aprilui/include")
include_directories(AFTER "../../../../atres/include")
include_directories(AFTER "../../../../april/include")
include_directories(AFTER "../../../../hltypes/include")
include_directories(AFTER "../../../../gtypes/include")

add_definitions(
	-DAPRILUIPARTICLE_EXPORTS
)

target_link_libraries(
	apriluiparticle
)
