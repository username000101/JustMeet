#include "Database/DBManager.hxx"

#include <cstdint>

#include <hiredis/hiredis.h>
#include <unicode/urename.h>

std::optional<std::string> justmeet::db::DatabaseManager::get_field(std::int64_t chat_id, const std::string& field) {
    const char* format = "hget %s %s";
    auto reply = (redisReply*)redisCommand(this->redis_,
                                           format,
                                           std::to_string(chat_id).c_str(),
                                             field.c_str());
    if (reply->type == REDIS_REPLY_NIL || reply->type == REDIS_REPLY_ERROR)
        return std::nullopt;
    else {
        switch (reply->type) {
            case REDIS_REPLY_STRING:
                return (reply->str ? reply->str : "char* == nullptr");
            case REDIS_REPLY_INTEGER:
                return std::to_string(reply->integer);
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
                return array_str;
            }
            case REDIS_REPLY_NIL:
                return std::nullopt;
            case REDIS_REPLY_ERROR:
                return std::nullopt;
            default:
                return std::nullopt;
        }
    }
}
