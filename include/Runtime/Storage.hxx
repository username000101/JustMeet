#ifndef JUSTMEET_RUNTIME_STORAGE_HXX
#define JUSTMEET_RUNTIME_STORAGE_HXX

#include <Database/DBManager.hxx>

namespace justmeet {
    namespace runtime_storage {
        inline std::shared_ptr<db::DatabaseManager> database;
    }
}

#endif // JUSTMMET_RUNTIME_STORAGE_HXX
