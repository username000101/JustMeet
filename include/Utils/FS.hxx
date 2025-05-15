#ifndef JUSTMEET_UTILS_FS_HXX
#define JUSTMEET_UTILS_FS_HXX

#include <filesystem>

namespace justmeet {
    namespace utils {
        inline void fscheck(const std::filesystem::path& path= FILES_FOLDER) {
            if (!std::filesystem::exists(path))
                std::filesystem::create_directories(path);
        }

        inline void create_directory(const std::filesystem::path& path) {
            std::filesystem::create_directories(path);
        }
    }
}

#endif // JJUSTMEET_UTILS_FS_HXX