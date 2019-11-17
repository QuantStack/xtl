/***************************************************************************
* Copyright (c) Johan Mabille, Sylvain Corlay and Wolf Vollprecht          *
* Copyright (c) QuantStack                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XTL_XPREFIX_HPP
#define XTL_XPREFIX_HPP

#include <cstring>
#include <string>

#if defined(__linux__)
#  include <unistd.h>
#endif
#if defined(_WIN32)
#  include <windows.h>
#endif
#ifdef __APPLE__
#  include <cstdint>
#  include <mach-o/dyld.h>
#endif
#if defined(__sun)
#  include <stdlib.h>
#endif

namespace xtl
{
    /*******************
     * executable_path *
     *******************/

    inline std::string executable_path()
    {
        std::string path;
        char buffer[1024];
        std::memset(buffer, '\0', sizeof(buffer));
#if defined(__linux__)
        if (readlink("/proc/self/exe", buffer, sizeof(buffer)) != -1)
        {
            path = buffer;
        }
        else
        {
            // failed to determine run path
        }
#elif defined (_WIN32)
        if (GetModuleFileName(nullptr, buffer, sizeof(buffer)) != 0)
        {
            path = buffer;
        }
        else
        {
            // failed to determine run path
        }
#elif defined (__APPLE__)
        std::uint32_t size = sizeof(buffer);
        if(_NSGetExecutablePath(buffer, &size) == 0)
        {
            path = buffer;
        }
        else
        {
            // failed to determine run path
        }
#elif defined (__FreeBSD__)
        int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
        if (sysctl(mib, 4, buffer, sizeof(buffer), NULL, 0) != -1)
        {
            path = buffer;
        }
        else
        {
            // failed to determine run path
        }
#elif defined(__sun)
        path = getexecname();
#endif
        return path;
    }
    
    inline std::string prefix_path()
    {
        std::string path = executable_path();
#if defined (_WIN32)
        char separator = '\\';
#else
        char separator = '/';
#endif
        return path.substr(0, path.find_last_of("/\\")) + separator + ".." + separator;
    }
}

#endif
