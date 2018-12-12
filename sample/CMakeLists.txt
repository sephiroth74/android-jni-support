
cmake_minimum_required(VERSION 3.6.0)

project(native-lib)

set(CMAKE_VERBOSE_MAKEFILE on)

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWERCASE)

add_library(
    native-lib
    SHARED
    src/main/cpp/native-lib.cpp
    src/main/cpp/android_unittest.cc
    src/main/cpp/shared_preferences_unittest.cc
    src/main/cpp/jni_references_unittest.cc
    src/main/cpp/jni_context_unittest.cc
    src/main/cpp/jni_uuid_unittest.cc
)

find_library(log-lib log)
find_library(android-lib android)


# ------------------------------
# Google Test
# ------------------------------

set(
    GOOGLETEST_ROOT
    ${ANDROID_NDK}/sources/third_party/googletest
)

add_library(
    gtest
    STATIC
    ${GOOGLETEST_ROOT}/src/gtest_main.cc
    ${GOOGLETEST_ROOT}/src/gtest-all.cc
)

target_include_directories(gtest PRIVATE ${GOOGLETEST_ROOT})

target_include_directories(gtest PUBLIC ${GOOGLETEST_ROOT}/include)


target_link_libraries(
    native-lib
    gtest
    ${log-lib}
    ${android-lib}
)

target_include_directories(
  native-lib
  PUBLIC
  ${GOOGLETEST_ROOT}/include
)

target_compile_options(
  native-lib
  PUBLIC
  -DDEBUG=1
  -D_DEBUG=1
  -DANDROID_DEBUG_EXTENDED
  -Wno-undefined-var-template
  -Wno-inaccessible-base
)

find_package(cdep-dependencies REQUIRED)
add_all_cdep_dependencies(native-lib)