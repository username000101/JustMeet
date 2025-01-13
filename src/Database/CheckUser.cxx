#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>

bool justmeet::db::DatabaseManager::check_user(std::int64_t chat_id) {
    if (!this->redis_) {
        this->logger_->error("{} ==> The redis connection is closed(this->redis_ is null pointer)",
                             __PRETTY_FUNCTION__);
        return false;
    }

    auto reply = (redisReply*)redisCommand(this->redis_, "hgetall %d", chat_id);
    if (reply->type == REDIS_REPLY_NIL)
        return false;
    else {
        if (reply->type == REDIS_REPLY_ARRAY) {
            if (reply->elements == 0)
                return false;
            else
                return true;
        } else {
            this->logger_->warn("{} ==> Unexpected reply type",
                                __PRETTY_FUNCTION__);
            return false;
        }
    }
}
