//
// Created by Alessandro Crugnola
//

#include <android-jni-support/jni_uuid.hpp>
#include <android-jni-support/jni_logging.hpp>

#include <map>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <uuid/uuid.h>

USING_NAMESPACE_ANDROID_JNI

// ----------------------------------------------------------------------------
// JNIUUID:generate_uuid
// ----------------------------------------------------------------------------
void JNIUUID::generate_uuid(std::string &out) {
    char uuid_str[37];
    uuid_t uuid;
    uuid_generate_time(uuid);
    uuid_unparse_lower(uuid, uuid_str);

    out.assign(uuid_str);
    uuid_clear(uuid);    
}