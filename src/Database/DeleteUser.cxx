#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>

bool justmeet::db::DatabaseManager::delete_user(std::int64_t chat_id) {
    if (!this->redis_) {
        this->logger_->error("{} ==> The redis connection is closed(this->redis_ is null pointer)",
                             __PRETTY_FUNCTION__);
        return false;
    }

    if (this->check_user(chat_id)) {
        const auto reply = static_cast<redisReply*>(redisCommand(this->redis_, "del user:%s", std::to_string(chat_id).c_str()));
        if (reply->type == REDIS_REPLY_INTEGER)
            if (reply->integer < 1) /* Error(?) */ {
                this->logger_->error("{} ==> Redis error: ",
                                     __PRETTY_FUNCTION__, this->redis_->errstr);
                return false;
            }
        return true;
    } else
        return true;
}
