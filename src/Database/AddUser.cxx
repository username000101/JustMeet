#include "Database/DBManager.hxx"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

// #include "Utils/STLTypesReader.hxx"

bool justmeet::db::DatabaseManager::add_user(justmeet::db::DatabaseManager::DatabaseUser& user) {
    if (!this->redis_) {
        this->logger_->error("{} ==> The redis connection is closed(this->redis_ is null pointer)",
                             __PRETTY_FUNCTION__);
        return false;
    }

    if (this->check_user(user.chat_id)) {
        this->logger_->warn("{} ==> {} ==> The user is already exists(please use the update_user if you want to update user info)",
                            __PRETTY_FUNCTION__, user.chat_id);
        return true;
    } else {
        auto reply = (redisReply*)redisCommand(this->redis_, "hset %d "
                                                               "tg_first_name %s "
                                                               "tg_last_name %s "
                                                               "username %s "
                                                               "name %s "
                                                               "age %d "
                                                               "gender %d "
                                                               "bio %s "
                                                               "language %d "
                                                               "city %s",

                                                               user.chat_id,
                                                               user.tg_first_name.c_str(),
                                                               user.tg_last_name.c_str(),
                                                               user.username.c_str(),
                                                               user.name.c_str(),
                                                               user.age,
                                                               static_cast<int>(user.gender),
                                                               user.bio.c_str(),
                                                               static_cast<int>(user.language),
                                                               user.city.c_str());
        if (reply->type == REDIS_REPLY_INTEGER) {
            this->logger_->info("{} ==> {} ==> The user has been added",
                                __PRETTY_FUNCTION__, user.chat_id);
            return true;
        } else {
            this->logger_->error("{} ==> {} ==> Redis error, some info:"
                                 "\nreply->type is {}"
                                 "\nreply->str is '{}'",
                                 __PRETTY_FUNCTION__,
                                 user.chat_id,
                                 reply->type,
                                 reply->str);
            return false;
        }
    }
}
