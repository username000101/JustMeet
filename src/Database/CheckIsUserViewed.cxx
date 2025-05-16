#include "Database/DBManager.hxx"

#include <hiredis/hiredis.h>

bool justmeet::db::DatabaseManager::check_is_user_viewed(std::int64_t chat_id, std::int64_t req_chat_id) {
    const auto is_exists_struct = static_cast<redisReply*>(redisCommand(this->redis_, "exists viewed:%s",
                                                                        std::to_string(chat_id).c_str()));
    if (is_exists_struct->type == REDIS_REPLY_INTEGER) {
        if (is_exists_struct->integer < 1) {
            redisCommand(this->redis_, "sadd viewed:%s %s",
                         std::to_string(chat_id).c_str(),
                         std::to_string(req_chat_id).c_str());
            freeReplyObject(is_exists_struct);
            return false;
        }
    }
    freeReplyObject(is_exists_struct);
    const auto is_exists_user = static_cast<redisReply*>(redisCommand(this->redis_, "sismember viewed:%s %s",
                                                                      std::to_string(chat_id).c_str(),
                                                                      std::to_string(req_chat_id).c_str()));
    if (is_exists_user->type == REDIS_REPLY_INTEGER) {
        if (is_exists_user->integer == 1) {
            freeReplyObject(is_exists_user);
            return true;
        }
        freeReplyObject(is_exists_user);
        return false;
    }
    freeReplyObject(is_exists_user);
    return false;
}