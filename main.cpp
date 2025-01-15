#include <iostream>

#include <hiredis/hiredis.h>

#include "Logic/Bot.hxx"
#include "Database/DBManager.hxx"
#include "Runtime/Storage.hxx"

int main() {
    using justmeet::db::DatabaseManager;
    using justmeet::runtime_storage::bot;
    using justmeet::runtime_storage::database;
    database = std::make_shared<justmeet::db::DatabaseManager>();
    justmeet::logic::bot_main();
    return 0;
}
