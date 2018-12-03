//
// Created by Alessandro Crugnola
//

#include <android-jni-support/jni_utils.hpp>
#include <android-jni-support/jni_logging.hpp>

#include <map>
#include <sstream>
#include <stdio.h>
#include <unistd.h>

USING_NAMESPACE_ANDROID_JNI

// ----------------------------------------------------------------------------
// JNIUtils:generate_uuid
// ----------------------------------------------------------------------------
void JNIUtils::generate_uuid(std::string &out) {
    FILE *fp;
    char buffer[38];

    fp = popen("cat /proc/sys/kernel/random/uuid", "r");
    if (fp != NULL) {
        if (fgets(buffer, 37, fp) != NULL)
            printf("%s", buffer);
        pclose(fp);
    } else {
        LOGE("cannot open uuid process");
    }

    buffer[37] = '\0';
    out.assign(buffer);
}