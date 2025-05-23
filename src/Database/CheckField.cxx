#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>

bool justmeet::db::DatabaseManager::check_field(std::int64_t chat_id, const std::string& field) {
    const char* format = "hexists user:%s %s";

    const auto reply = static_cast<redisReply*>(redisCommand(this->redis_,
                                            format,
                                            std::to_string(chat_id).c_str(),
                                            field.c_str()));

    if (reply->type == REDIS_REPLY_NIL || (reply->type ==  REDIS_REPLY_INTEGER && reply->integer == 0)) {
        freeReplyObject(reply);
        return false;
    } else {
        freeReplyObject(reply);
        return true;
    }
}
