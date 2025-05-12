#ifndef JUSTMEET_UTILS_RESTART_HXX
#define JUSTMEET_UTILS_RESTART_HXX

#if defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif // define(_WIN32)

#include <string>
#include <vector>


namespace justmeet {
    namespace utils {
        inline void restart() {
#if defined(_WIN32)
            STARTUPINFO strtup = { sizeof(STARTUPINFO) };
            PROCESS_INFORMATION proc_info;
            CreateProcess(NULL, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &strtup, &proc_info);
            CloseHandle(proc_info.hProcess);
            CloseHandle(proc_info.hThread);
#else
            std::vector<char> pname_buf(1024);
            ssize_t length = readlink("/proc/self/exe", pname_buf.data(), pname_buf.size() - 1);
            if (length != -1) {
                pname_buf[length] = '\0';
                execl(std::string(pname_buf.data()).c_str(), std::string(pname_buf.data()).c_str(), reinterpret_cast<char*>(NULL));
            }
#endif // defined(_WIN32)
        }
    }
}

#endif // JJUSTMEET_UTILS_RESTART_HXX