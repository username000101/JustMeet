#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>

bool justmeet::db::DatabaseManager::check_field(std::int64_t chat_id, const std::string& field) {
    const char* format = "hexists %s %s";

    auto reply = (redisReply*)redisCommand(this->redis_,
                                            format,
                                            std::to_string(chat_id).c_str(),
                                            field.c_str());

    if (reply->type == REDIS_REPLY_NIL)
        return false;
    else
        return true;
}
