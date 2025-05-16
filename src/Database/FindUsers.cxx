#include "Database/DBManager.hxx"

#include <spdlog/spdlog.h>

std::vector<justmeet::db::DatabaseManager::DatabaseUser> justmeet::db::DatabaseManager::find_users(const std::string& pattern) {
    std::vector<DatabaseUser> users;

    const char* format = "ft.search users %s RETURN 0";
    const auto reply = static_cast<redisReply*>(redisCommand(this->redis_,
                                           format,
                                           pattern.c_str()));
    if (reply->type == REDIS_REPLY_NIL || reply->type == REDIS_REPLY_ERROR) {
        freeReplyObject(reply);
        return {};
    }
    if (reply->elements > 0) {
        for (std::uint32_t i = 0; i < reply->elements; ++i) {
            if (reply->element[i]->type == REDIS_REPLY_STRING) {
                if (!reply->element[i]->str) {
                    this->logger_->warn("{} ==> reply->element[{}]->str is (nullptr(NULL))({})",
                                        __PRETTY_FUNCTION__, i, reply->element[i]->str);
                    continue;
                }
                std::string str_cpy = reply->element[i]->str;
                this->logger_->debug("{} ==> Converting key {} to int",
                                     __PRETTY_FUNCTION__, str_cpy.substr(5));
                auto user = get_user(std::stol(str_cpy.substr(5)));
                if (user.has_value())
                    users.push_back(user.value());
                else
                    this->logger_->warn("{} ==> Failed to add user {}: std::optional<DatabaseUser> is std::nullopt",
                                        __PRETTY_FUNCTION__, reply->element[i]->str);
            }
        }
        freeReplyObject(reply);
        return users;
    } else {
        freeReplyObject(reply);
        return {};
    }
}