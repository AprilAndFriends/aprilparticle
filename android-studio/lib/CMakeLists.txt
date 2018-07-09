cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_VERBOSE_MAKEFILE on)

project(aprilparticle)

include("../../../hltypes/android-studio/generic/CMakeLists.txt")

file(
	GLOB_RECURSE CppSrc
	"../../src/*.c"
	"../../src/*.cpp"
)

add_library(aprilparticle STATIC ${CppSrc})

include_directories(AFTER "../../include/aprilparticle")
include_directories(AFTER "../../../april/include")
include_directories(AFTER "../../../hltypes/hlxml/include")
include_directories(AFTER "../../../gtypes/include")
include_directories(AFTER "../../../hltypes/include")

add_definitions(
	-DAPRILPARTICLE_EXPORTS
)
