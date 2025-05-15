#include "Database/DBManager.hxx"

#include <algorithm>
#include <cstring>
#include <filesystem>
#include <ranges>
#include <string>
#include <optional>

#include <spdlog/spdlog.h>

#include "Utils/STLTypes.hxx"

using justmeet::db::DatabaseManager;
using justmeet::utils::cast_container;
using justmeet::utils::string_reader;

using cast_pref_langs_rt = std::vector<DatabaseManager::DatabaseUser::DatabaseUserLanguage>;
using cast_pref_ages_rt = std::vector<std::uint16_t>;
using cast_pref_genders_rt = std::vector<DatabaseManager::DatabaseUser::DatabaseUserGender>;
using cast_pref_cities_rt = std::vector<std::string>;
using cast_media_rt = std::vector<std::filesystem::path>;

cast_pref_ages_rt cast_preferred_ages(const std::string& str_preferred_ages) {
    cast_pref_ages_rt result;
    auto casted = string_reader<std::vector<std::string>>(str_preferred_ages);
    std::ranges::for_each(casted, [&result](auto&& value) {
        result.push_back(static_cast<std::uint16_t>(std::stoi(value)));
    });
    return result;
}

cast_pref_genders_rt cast_preferred_genders(const std::string& str_preferred_genders) {
    cast_pref_genders_rt result;
    auto casted = string_reader<std::vector<std::string>>(str_preferred_genders);
    std::for_each(casted.begin(), casted.end(), [&result](auto&& value) {
        result.push_back(static_cast<DatabaseManager::DatabaseUser::DatabaseUserGender>(std::stoi(value)));
    });
    return result;
}

cast_pref_langs_rt cast_preferred_languages(const std::string& str_preferred_languages) {
    cast_pref_langs_rt result;
    auto casted = string_reader<std::vector<std::string>>(str_preferred_languages);
    std::for_each(casted.begin(), casted.end(), [&result](auto&& value) {
        result.push_back(static_cast<DatabaseManager::DatabaseUser::DatabaseUserLanguage>(std::stoi(value)));
    });
    return result;
}

cast_pref_cities_rt cast_preferred_cities(const std::string& str_preferred_cities) {
    return string_reader<std::vector<std::string>>(str_preferred_cities);
}

cast_media_rt cast_media(const std::string& str_media) {
    cast_media_rt result;
    auto casted = string_reader<std::vector<std::string>>(str_media);
    std::for_each(casted.begin(), casted.end(), [&result](auto&& value) {
        result.push_back(std::filesystem::path(value));
    });
    return result;
}

std::optional<justmeet::db::DatabaseManager::DatabaseUser> justmeet::db::DatabaseManager::get_user(std::int64_t chat_id) {
    using v_genders = std::vector<DatabaseManager::DatabaseUser::DatabaseUserGender>;
    using v_langs = std::vector<DatabaseManager::DatabaseUser::DatabaseUserLanguage>;
    using v_str = std::vector<std::string>;
    using v_path = std::vector<std::filesystem::path>;

    if (!this->check_user(chat_id)) {
        this->logger_->warn("{} ==> {} ==> Can't find the user",
                            __PRETTY_FUNCTION__, chat_id);
        return std::nullopt;
    } else {
        auto reply = (redisReply*)redisCommand(this->redis_, "hgetall user:%s", std::to_string(chat_id).c_str());
        if (reply->type == REDIS_REPLY_ARRAY) {
            DatabaseManager::DatabaseUser result;
            for (std::size_t i = 0; i + 1 < reply->elements; i += 2) {
                if (std::strcmp(reply->element[i]->str, "tg_first_name") == 0)
                    result.tg_first_name = reply->element[i + 1]->str;
                else if (std::strcmp(reply->element[i]->str, "tg_last_name") == 0)
                    result.tg_last_name = reply->element[i + 1]->str;
                else if (std::strcmp(reply->element[i]->str, "name") == 0)
                    result.name = reply->element[i + 1]->str;
                else if (std::strcmp(reply->element[i]->str, "age") == 0)
                    result.age = reply->element[i + 1]->integer;
                else if (std::strcmp(reply->element[i]->str, "gender") == 0)
                    result.gender = static_cast<DatabaseManager::DatabaseUser::DatabaseUserGender>(reply->element[i + 1]->integer);
                else if (std::strcmp(reply->element[i]->str, "bio") == 0)
                    result.bio = reply->element[i + 1]->str;
                else if (std::strcmp(reply->element[i]->str, "language") == 0)
                    result.language = static_cast<DatabaseManager::DatabaseUser::DatabaseUserLanguage>(reply->element[i + 1]->integer);
                else if (std::strcmp(reply->element[i]->str, "city") == 0)
                    result.city = reply->element[i + 1]->str;
                else if (std::strcmp(reply->element[i]->str, "profile_state") == 0)
                    result.profile_state = static_cast<DatabaseManager::DatabaseUser::UserProfileState>(reply->element[i + 1]->integer);
                else if (std::strcmp(reply->element[i]->str, "preferred_ages") == 0)
                    result.preferred_ages = cast_preferred_ages(reply->element[i + 1]->str);
                else if (std::strcmp(reply->element[i]->str, "preferred_genders") == 0)
                    result.preferred_genders = cast_preferred_genders(reply->element[i + 1]->str);
                else if (std::strcmp(reply->element[i]->str, "preferred_languages") == 0)
                    result.preferred_languages = cast_preferred_languages(reply->element[i + 1]->str);
                else if (std::strcmp(reply->element[i]->str, "preferred_cities") == 0)
                    result.preferred_cities = cast_preferred_cities(reply->element[i + 1]->str);
                else if (std::strcmp(reply->element[i]->str, "media") == 0)
                    result.media = cast_media(reply->element[i + 1]->str);
            }
            return result;
        } else {
            this->logger_->warn("{} ==> {} ==> Unexpected reply: {}",
                                __PRETTY_FUNCTION__, chat_id, reply->type);
            return std::nullopt;
        }
    }
}
