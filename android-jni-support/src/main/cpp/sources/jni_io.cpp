//
// Created by Alessandro Crugnola on 8/27/18.
//


#include <android-jni-support/jni_io.hpp>

ANDROID_JNI_NAMESPACE_BEGIN_DECL

namespace filesystem {

bool exists(const std::string &filename) {
    if (filename.empty())
        return false;
    struct stat st;
    return (stat(filename.c_str(), &st) == 0);
}

bool is_file(const std::string &filename) {
    struct stat buf;
    if (stat(filename.c_str(), &buf) == 0)
        return S_ISREG(buf.st_mode);
    return false;
}

bool is_directory(const std::string &filename) {
    struct stat st;
    if (stat(filename.c_str(), &st) == 0)
        return S_ISDIR(st.st_mode);
    return false;
}

int mkdirs(const std::string &filename) {
    if (filename.empty())
        return ENOENT;
    char tmp[PATH_MAX];
    char *p = NULL;
    int result;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", filename.c_str());
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (!is_directory(tmp)) {
                result = mkdir(tmp, S_IRWXU | S_IRWXG | S_IRWXO);
                if (result != 0)
                    return result;
            }
            *p = '/';
        }
    }
    if (!exists(tmp)) {
        return mkdir(tmp, S_IRWXU | S_IRWXG | S_IRWXO);
    } else {
        if (is_directory(tmp))
            return 0;
        else
            return EEXIST;
    }
}

} // namespace filesystem

ANDROID_JNI_NAMESPACE_END_DECL