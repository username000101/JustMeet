#include <iostream>

#include <Runtime/Storage.hxx>
#include <Database/DBManager.hxx>
#include <Utils/STLTypesReader.hxx>

#include <hiredis/hiredis.h>

using namespace std;

#define ISNIL(reply) (reply->type == REDIS_REPLY_NIL ? true : false)

int main()
{
    using justmeet::db::DatabaseManager;
    using justmeet::runtime_storage::database;
    database = std::make_shared<justmeet::db::DatabaseManager>();

    justmeet::db::DatabaseManager::DatabaseUser dbuser;
    dbuser.chat_id = 12345678;
    dbuser.tg_first_name = "Andrew";
    dbuser.tg_last_name = "Ivanov";
    dbuser.username = "andrewivanov123321";
    dbuser.age = 32;
    dbuser.bio = "Just a test user(?)";
    dbuser.gender = DatabaseManager::DatabaseUser::MALE;
    dbuser.city = "Tokyo";
    dbuser.language = DatabaseManager::DatabaseUser::English;
    database->add_user(dbuser);

    database->ban_user(12345678);
    database->unban_user(12345678);
    return 0;
}
