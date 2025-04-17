#include "Database/DBManager.hxx"

bool justmeet::db::DatabaseManager::delete_field(std::int64_t chat_id, const std::string& name) {
    auto reply = (redisReply*)redisCommand(this->redis_,
                                           "hdel %s %s",
                                           std::to_string(chat_id).c_str(),
                                           name.c_str());
    if (reply->type == REDIS_REPLY_INTEGER) {
        this->logger_->log(spdlog::level::info,
                           "{} ==> Field of key {} has been deleted",
                           __PRETTY_FUNCTION__, chat_id);
        return true;
    } else {
        this->logger_->log(spdlog::level::warn,
                           "{} ==> Failed to delete field of key {}: redisReply::type is {}",
                           __PRETTY_FUNCTION__, chat_id, reply->type);
        return false;
    }
}
