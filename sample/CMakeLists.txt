
cmake_minimum_required(VERSION 3.4.1)

string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWERCASE)

add_library(
    native-lib
    SHARED
    src/main/cpp/native-lib.cpp
    src/main/cpp/android_unittest.cc
    src/main/cpp/shared_preferences_unittest.cc
    src/main/cpp/jni_references_unittest.cc
    src/main/cpp/jni_context_unittest.cc
    src/main/cpp/jni_utils_unittest.cc
)

find_library(log-lib log)
find_library(android-lib android)


# ------------------------------
# android-jni-support
# ------------------------------

# set(
#     ANDROID_JNI_SUPPORT_BASE_DIR
#     ${CMAKE_CURRENT_LIST_DIR}/../android-jni-support
# )

# include(${ANDROID_JNI_SUPPORT_BASE_DIR}/android-jni-support.cmake)

# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../android-jni-support/build/android-jni-support/cmake ${CMAKE_CURRENT_BINARY_DIR}/android-jni-support)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../android-jni-support ${CMAKE_CURRENT_BINARY_DIR}/android-jni-support)

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
    # my_library
    ${log-lib}
    ${android-lib}
)

target_include_directories(
  native-lib
  PUBLIC
  # ${CMAKE_CURRENT_LIST_DIR}/../android-jni-support/src/main/cpp/includes/public
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
