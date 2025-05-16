#include "Database/DBManager.hxx"

bool justmeet::db::DatabaseManager::delete_field(std::int64_t chat_id, const std::string& name) {
    const auto reply = static_cast<redisReply*>(redisCommand(this->redis_,
                                           "hdel user:%s %s",
                                           std::to_string(chat_id).c_str(),
                                           name.c_str()));
    if (reply->type == REDIS_REPLY_INTEGER) {
        this->logger_->info("{} ==> Field of key {} has been deleted",
                           __PRETTY_FUNCTION__, chat_id);
        return true;
    } else {
        this->logger_->warn("{} ==> Failed to delete field of key {}: redisReply::type is {}",
                           __PRETTY_FUNCTION__, chat_id, reply->type);
        return false;
    }
}
