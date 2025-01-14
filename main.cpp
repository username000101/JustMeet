#include <iostream>

#include <hiredis/hiredis.h>

#include "Runtime/Storage.hxx"
#include "Database/DBManager.hxx"

int main() {
    using justmeet::db::DatabaseManager;
    using justmeet::runtime_storage::database;
    database = std::make_shared<justmeet::db::DatabaseManager>();
    return 0;
}
