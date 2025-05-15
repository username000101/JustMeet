#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>

bool justmeet::db::DatabaseManager::ban_user(std::int64_t chat_id) {
    if (!this->redis_) {
        this->logger_->error("{} ==> The redis connection is closed(this->redis_ is null pointer)",
                             __PRETTY_FUNCTION__);
        return false;
    }

    if (this->check_user(chat_id)) {
        auto reply = (redisReply*)redisCommand(this->redis_, "hget user:%s banned", std::to_string(chat_id).c_str());
        if (reply->type == REDIS_REPLY_NIL) {
            auto reply2 = (redisReply*)redisCommand(this->redis_, "hset user:%s banned 1", std::to_string(chat_id).c_str());
            if (reply2->type == REDIS_REPLY_INTEGER && reply2->integer > 0) {
                this->logger_->info("{} ==> {} ==> Banned user",
                                __PRETTY_FUNCTION__, chat_id);
                return true;
            } else {
                this->logger_->error("{} ==> {} ==> Redis error: {}",
                                     __PRETTY_FUNCTION__,
                                     chat_id,
                                     this->redis_->errstr);
                return false;
            }
        } else {
            this->logger_->warn("{} ==> {} ==> The user is already banned",
                                __PRETTY_FUNCTION__, chat_id);
            return true;
        }
    } else {
        this->logger_->error("{} ==> {} ==> Can't find the user",
                             __PRETTY_FUNCTION__, chat_id);
        return false;
    }
}
