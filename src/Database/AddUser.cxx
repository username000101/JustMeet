#include "Database/DBManager.hxx"

#include <cassert>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "Utils/STLTypes.hxx"

bool justmeet::db::DatabaseManager::add_user(justmeet::db::DatabaseManager::DatabaseUser& user) {
    using utils::container_reader;
    using utils::string_reader;
    using utils::cast_container;
    using db::DatabaseManager;
    using v_int = std::vector<int>;
    using v_u16 = std::vector<std::uint16_t>;
    using v_str = std::vector<std::string>;
    using v_genders = std::vector<DatabaseManager::DatabaseUser::DatabaseUserGender>;
    using v_langs = std::vector<DatabaseManager::DatabaseUser::DatabaseUserLanguage>;
    using v_path = std::vector<std::filesystem::path>;

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
        auto preferred_ages = container_reader<v_u16>(user.preferred_ages);
        auto preferred_genders = container_reader<v_int>(cast_container<v_genders, v_int>(user.preferred_genders));
        auto preferred_languages = container_reader<v_int>(cast_container<v_langs, v_int>(user.preferred_languages));
        auto preferred_cities = container_reader<v_str>(user.preferred_cities);
        auto media = container_reader<v_str>(cast_container<v_path, v_str>(user.media));

        const char* format = "hset %s tg_first_name %s tg_last_name %s username %s name %s age %d gender %d bio %s language %d city %s profile_state %d preferred_ages %s preferred_genders %s preferred_languages %s preferred_cities %s media %s";

        redisReply* reply = (redisReply*)redisCommand(this->redis_,
                                                        format,
                                                        std::to_string(user.chat_id).c_str(),
                                                        user.tg_first_name.c_str(),
                                                        user.tg_last_name.c_str(),
                                                        user.username.c_str(),
                                                        user.name.c_str(),
                                                        user.age,
                                                        static_cast<int>(user.gender),
                                                        user.bio.c_str(),
                                                        static_cast<int>(user.language),
                                                        user.city.c_str(),
                                                        static_cast<int>(user.profile_state),
                                                        preferred_ages.c_str(),
                                                        preferred_genders.c_str(),
                                                        preferred_languages.c_str(),
                                                        preferred_cities.c_str(),
                                                        media.c_str());
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
