#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>
#include <unicode/urename.h>

std::optional<std::string> justmeet::db::DatabaseManager::get_field(std::int64_t chat_id, const std::string& field) {
    spdlog::debug("{} ==> Getting field from key {}: {}",
                  __PRETTY_FUNCTION__, chat_id, field);
    const char* format = "hget user:%s %s";
    const auto reply = static_cast<redisReply*>(redisCommand(this->redis_,
                                           format,
                                           std::to_string(chat_id).c_str(),
                                             field.c_str()));
    if (reply->type == REDIS_REPLY_NIL || reply->type == REDIS_REPLY_ERROR) {
        freeReplyObject(reply);
        return std::nullopt;
    } else {
        switch (reply->type) {
            case REDIS_REPLY_STRING: {
                std::string str_cpy = (reply->str ? reply->str : "char* == nullptr");
                freeReplyObject(reply);
                return str_cpy;
            }
            case REDIS_REPLY_INTEGER: {
                auto int_cpy = (reply->integer ? std::to_string(reply->integer) : "0");
                freeReplyObject(reply);
                return int_cpy;
            }
            case REDIS_REPLY_ARRAY: {
                std::string array_str;
                for (auto element = 0; element != reply->elements; ++element)
                    if (reply->element[element] && reply->element[element + 1]) {
                        array_str.append("[");
                        switch (reply->element[element]->type) {
                            case REDIS_REPLY_STRING:
                                array_str.append(std::string(reply->element[element]->str));
                                break;
                            case REDIS_REPLY_INTEGER:
                                array_str.append(std::to_string(reply->element[element]->integer));
                                break;
                            default:
                                array_str.append("UNKNOWN");
                                break;
                        }
                        array_str.append("](");

                        switch (reply->element[element + 1]->type) {
                            case REDIS_REPLY_STRING:
                                array_str.append(std::string(reply->element[element + 1]->str));
                                break;
                            case REDIS_REPLY_INTEGER:
                                array_str.append(std::to_string(reply->element[element + 1]->integer));
                                break;
                            default:
                                array_str.append("UNKNOWN");
                        }
                        array_str.append(")");
                    }
                freeReplyObject(reply);
                return array_str;
            }
            case REDIS_REPLY_NIL:
                freeReplyObject(reply);
                return std::nullopt;
            case REDIS_REPLY_ERROR:
                freeReplyObject(reply);
                return std::nullopt;
            default:
                freeReplyObject(reply);
                return std::nullopt;
        }
    }
    return std::nullopt;
}
