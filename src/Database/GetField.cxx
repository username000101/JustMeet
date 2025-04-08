#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>

std::optional<std::string> justmeet::db::DatabaseManager::get_field(std::int64_t chat_id, const std::string& field) {
    const char* format = "hget %s %s";
    auto reply = (redisReply*)redisCommand(this->redis_,
                                           format,
                                           std::to_string(chat_id).c_str(),
                                             field.c_str());
    if (reply->type == REDIS_REPLY_NIL)
        return std::nullopt;
    else {
        if (reply->element[0] && std::strcmp(reply->element[0]->str, field.c_str()) == 0 && reply->element[1])
            return reply->element[1]->str;
        else
            return std::nullopt;
    }
}
