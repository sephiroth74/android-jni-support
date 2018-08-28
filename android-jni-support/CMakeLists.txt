
cmake_minimum_required(VERSION 3.4.1)

set(LIBRARY_NAME android-jni-support)

if("${ANDROID_ABI}" STREQUAL "x86_64")
  set(TARGET_CPU_X86    0)
  set(TARGET_CPU_X86_64 1)
  set(TARGET_CPU_64_BIT 1)
  set(TARGET_CPU_32_BIT 0)
  set(TARGET_CPU_ARM    0) # legacy
  set(TARGET_CPU_ARM7   0)
  set(TARGET_CPU_ARM64  0)
elseif("${ANDROID_ABI}" STREQUAL "x86")
  set(TARGET_CPU_X86    1)
  set(TARGET_CPU_X86_64 0)
  set(TARGET_CPU_64_BIT 0)
  set(TARGET_CPU_32_BIT 1)
  set(TARGET_CPU_ARM    0)
  set(TARGET_CPU_ARM7   0)
  set(TARGET_CPU_ARM64  0)
elseif("${ANDROID_ABI}" STREQUAL "arm64-v8a")
  set(TARGET_CPU_X86    0)
  set(TARGET_CPU_X86_64 0)
  set(TARGET_CPU_64_BIT 1)
  set(TARGET_CPU_32_BIT 0)
  set(TARGET_CPU_ARM    1)
  set(TARGET_CPU_ARM7   0)
  set(TARGET_CPU_ARM64  1)
elseif("${ANDROID_ABI}" STREQUAL "armeabi-v7a")
  set(TARGET_CPU_X86    0)
  set(TARGET_CPU_X86_64 0)
  set(TARGET_CPU_64_BIT 0)
  set(TARGET_CPU_32_BIT 1)
  set(TARGET_CPU_ARM    1)
  set(TARGET_CPU_ARM7   1)
  set(TARGET_CPU_ARM64  0)
endif()

set(SHAREDLIBS_CFLAGS_Debug -O0 -g -DDEBUG=1)
set(SHAREDLIBS_CFLAGS_Release -O2 -DNDEBUG -DBOOST_DISABLE_ASSERTS)
set(SHAREDLIBS_CFLAGS_CONFIG ${SHAREDLIBS_CFLAGS_${CMAKE_BUILD_TYPE}})

set(SHAREDLIBS_CFLAGS
  ${SHAREDLIBS_CFLAGS_CONFIG}
  -D__ANDROID__
  -DANDROID_ABI=${ANDROID_ABI}
  -DANDROID_ENV=1
  -DUNIX_ENV
  -DENABLE_ZIP=1
  -D_LARGEFILE64_SOURCE
  -D_FILE_OFFSET_BITS=64
  -Wbool-conversion
  -Wconstant-conversion
  -Wempty-body
  -Wenum-conversion
  -Werror
  -Werror=deprecated-objc-isa-usage
  -Werror=objc-root-class
  -Werror=return-type
  -Wint-conversion
  -Wno-\#pragma-messages
  -Wno-conversion
  -Wno-dangling-else
  -Wno-deprecated-declarations
  -Wno-exit-time-destructors
  -Wno-expansion-to-defined
  -Wno-four-char-constants
  -Wno-infinite-recursion
  -Wno-invalid-offsetof
  -Wno-macro-redefined
  -Wno-missing-braces
  -Wno-missing-field-initializers
  -Wno-missing-prototypes
  -Wno-move
  -Wno-newline-eof
  -Wno-nonportable-include-path
  -Wno-null-dereference
  -Wno-shadow
  -Wno-sign-conversion
  -Wno-trigraphs
  -Wno-undefined-var-template
  -Wno-unknown-pragmas
  -Wno-unused-label
  -Wno-unused-parameter
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wparentheses
  -Wshorten-64-to-32
  -Wswitch
  -Wuninitialized
  -Wunreachable-code
  -Wunused-function
  -Wunused-value
  -Wunused-variable
  -Wno-inaccessible-base
  -fasm-blocks
  -fdiagnostics-show-note-include-stack
  -ferror-limit=1
  -fexceptions
  -fmacro-backtrace-limit=0
  -fms-extensions
  -fno-common
  -fpascal-strings
  -fstrict-aliasing
  -fvisibility-inlines-hidden
  -gmodules
  -w
  -x c++
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  set(SHAREDLIBS_CFLAGS ${SHAREDLIBS_CFLAGS} -DRELEASE_BUILD -DNDEBUG  -D_NDEBUG=1)
else()
  set(SHAREDLIBS_CFLAGS ${SHAREDLIBS_CFLAGS} -DDEBUG=1 -D_DEBUG=1 -DANDROID_DEBUG_EXTENDED)
endif()


## CPU Flags ##

set(TARGET_CPU_CFLAGS
  -DANDROID_ABI=${ANDROID_ABI}
  -DTARGET_CPU_X86=${TARGET_CPU_X86}
  -DTARGET_CPU_X86_64=${TARGET_CPU_X86_64}
  -DTARGET_CPU_64_BIT=${TARGET_CPU_64_BIT}
  -DTARGET_CPU_32_BIT=${TARGET_CPU_32_BIT}
  -DTARGET_CPU_ARM=${TARGET_CPU_ARM}
  -DTARGET_CPU_ARM7=${TARGET_CPU_ARM7}
  -DTARGET_CPU_ARM64=${TARGET_CPU_ARM64}
)

if(TARGET_CPU_64_BIT)
  set(TARGET_CPU_CFLAGS ${TARGET_CPU_CFLAGS} -DTARGET_CPU_64_BIT=1)
else()
  set(TARGET_CPU_CFLAGS ${TARGET_CPU_CFLAGS} -DTARGET_CPU_32_BIT=1)
endif()


if(TARGET_CPU_X86)
  set(TARGET_CPU_CFLAGS
    ${TARGET_CPU_CFLAGS}
    -DCORE_SUPPORT_SSE)

elseif(TARGET_CPU_X86_64)
  set(TARGET_CPU_CFLAGS
    ${TARGET_CPU_CFLAGS}
    -DCORE_SUPPORT_SSE)
endif()


if((TARGET_CPU_ARM7 EQUAL 1) OR (TARGET_CPU_ARM64 EQUAL 1))
  set(TARGET_CPU_CFLAGS
    ${TARGET_CPU_CFLAGS}
    -D__ARM_NEON__
    -mfpu=neon
  )
endif()

## set all ##

set(SHAREDLIBS_CFLAGS
  ${TARGET_CPU_CFLAGS}
  ${SHAREDLIBS_CFLAGS}
)

set(SHAREDLIBS_CXX_FLAGS
  ${SHAREDLIBS_CFLAGS}
)

set(LIBRARY_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/sources)


add_library(
    ${LIBRARY_NAME}
    STATIC
    ${LIBRARY_SRC_DIR}/jni_logging.cpp
    ${LIBRARY_SRC_DIR}/jni_context.cpp
    ${LIBRARY_SRC_DIR}/jni_env.cpp
    ${LIBRARY_SRC_DIR}/jni_helper.cpp
    ${LIBRARY_SRC_DIR}/jni_references.cpp
    ${LIBRARY_SRC_DIR}/jni_thread.cpp
    ${LIBRARY_SRC_DIR}/jni_boxed.cpp
    ${LIBRARY_SRC_DIR}/jni_locale.cpp
    ${LIBRARY_SRC_DIR}/jni_io.cpp
    ${LIBRARY_SRC_DIR}/jni_shared_preferences.cpp
    ${LIBRARY_SRC_DIR}/jni_exceptions.cpp
    ${LIBRARY_SRC_DIR}/jni_value_adapter.cpp
    ${LIBRARY_SRC_DIR}/jni_file_manager.cpp
    ${LIBRARY_SRC_DIR}/main.cpp
)

set_target_properties(
    ${LIBRARY_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}"
)


target_include_directories(
  ${LIBRARY_NAME}
  PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/includes/public
)

target_compile_options(
  ${LIBRARY_NAME}
  PUBLIC
  ${SHAREDLIBS_CFLAGS}
)


find_library(log-lib log)
find_library(android-lib android)


target_link_libraries(
    ${LIBRARY_NAME}
    ${log-lib}
    ${android-lib}
    c++_shared
)