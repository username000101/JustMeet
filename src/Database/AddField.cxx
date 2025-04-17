#include "Database/DBManager.hxx"

bool justmeet::db::DatabaseManager::add_field(std::int64_t chat_id, const std::string& name, const std::string& value) {

    auto reply = (redisReply*)redisCommand(this->redis_,
                                           "hset %s %s %s",
                                             std::to_string(chat_id).c_str(),
                                             name.c_str(),
                                             value.c_str());

    if (reply->type == REDIS_REPLY_INTEGER) {
        this->logger_->log(spdlog::level::info,
                           "{} ==> Added field to key {}: {}",
                           __PRETTY_FUNCTION__, chat_id, value);
        return true;
    } else {
        this->logger_->log(spdlog::level::warn,
                           "{} ==> Failed to add field to key {}: redisReply::type is {}",
                           __PRETTY_FUNCTION__, chat_id, reply->type);
        return false;
    }
}
