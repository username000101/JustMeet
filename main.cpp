#include <iostream>

#include "Logic/Bot.hxx"
#include "Database/DBManager.hxx"
#include "Utils/FS.hxx"
#include "Runtime/Storage.hxx"

int main() {
    using justmeet::db::DatabaseManager;
    using justmeet::runtime_storage::bot;
    using justmeet::runtime_storage::database;
    database = std::make_shared<DatabaseManager>();
    justmeet::utils::fscheck();
    justmeet::logic::bot_main();
    return 0;
}
