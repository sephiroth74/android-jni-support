
cmake_minimum_required(VERSION 3.4.1)

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWERCASE)

add_library(
    native-lib
    SHARED
    src/main/cpp/native-lib.cpp
    src/main/cpp/android_unittest.cc
)

find_library(log-lib log)
find_library(android-lib android)

add_library(android-jni-support STATIC IMPORTED)
set_target_properties(
    android-jni-support
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_CURRENT_LIST_DIR}/../android-jni-support/lib/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libandroid-jni-support.a
)

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
    android-jni-support
    ${log-lib}
    ${android-lib}
)

target_include_directories(
  native-lib
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/../android-jni-support/src/main/cpp/includes/public
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
