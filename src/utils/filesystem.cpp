/*!
 * \file src/utils/filesystem.cpp
 * \brief file-system utilities
 *
 * \author xythobuz
 */

#include "global.h"
#include "utils/filesystem.h"

#if defined(HAVE_UNISTD_H) && defined(HAVE_GETCWD)
#include <unistd.h>
#endif

#if defined(HAVE_STDLIB_H) && defined(HAVE_GETENV)
#include <stdlib.h>
#endif

#if defined(HAVE_DIRECT_H) && defined(HAVE__GETCWD)
#include <direct.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

std::string getCurrentWorkingDirectory() {
#if defined(HAVE_UNISTD_H) && defined(HAVE_GETCWD)

    char path[1024];
    orAssertEqual(getcwd(path, 1024), path);
    return std::string(path);

#elif defined(HAVE_DIRECT_H) && defined(HAVE__GETCWD)

    char path[1024];
    orAssertEqual(_getcwd(path, 1024), path);
    return std::string(path);

#else

    orAssert(false);
    return "";

#endif
}

std::string getHomeDirectory() {
#if defined(_WIN32)

    char path[MAX_PATH];
    orAssertEqual(SHGetFolderPath(nullptr, CSIDL_PROFILE, nullptr, 0, path), S_OK);
    size_t lenPath = strlen(path);
    for (unsigned int i = 0; i < lenPath; i++)
        if (path[i] == '\\')
            path[i] = '/';
    return std::string(path);

#elif defined(HAVE_STDLIB_H) && defined(HAVE_GETENV)

    char* path = getenv("HOME");
    orAssert(path != nullptr);
    return path;

#else

    orAssert(false);
    return "";

#endif
}

