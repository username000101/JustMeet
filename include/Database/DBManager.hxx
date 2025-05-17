#ifndef JUSTMEET_DATABASE_DBMANAGER_HXX
#define JUSTMEET_DATABASE_DBMANAGER_HXX

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <memory>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <hiredis/hiredis.h>

namespace justmeet {
    namespace db {
        class DatabaseManager {
        public:
            struct DatabaseUser { 
                enum DatabaseUserGender {
                    MALE   = 0,
                    FEMALE = 1,
                };

                enum UserProfileState {
                    NONE,
                    NAME,
                    AGE,
                    BIO,
                    CITY,
                    LANGUAGE,
                    GENDER,
                    PREFERRED_LANGUAGES,
                    PREFERRED_AGES,
                    PREFERRED_CITIES,
                    PREFERRED_GENDERS,
                };

                std::string tg_first_name;
                std::string tg_last_name;
                std::int64_t chat_id;
                std::string name;
                std::uint16_t age;
                std::string bio;
                DatabaseUserGender gender;
                std::string city;
                UserProfileState profile_state;

                std::vector<std::uint16_t> preferred_ages;
                std::vector<std::string> preferred_cities;
                std::vector<DatabaseUserGender> preferred_genders;

                std::vector<std::filesystem::path> media;
            };

            DatabaseManager() {
                this->logger_ = std::make_shared<spdlog::logger>("DatabaseManager",
                                                                 spdlog::sinks_init_list{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()});
                this->logger_->set_level(spdlog::level::trace);

                this->redis_ = redisConnect("localhost", 6379);
                if (!this->redis_) {
                    this->logger_->critical("{} ==> {} {}",
                                     __PRETTY_FUNCTION__,
                                     "Redis connection failed: ",
                                     this->redis_->errstr);
                    std::exit(1);
                }
            }

            ~DatabaseManager() {
                if (this->redis_)
                    redisFree(this->redis_);
            }

            std::optional<std::string> get_field(std::int64_t chat_id, const std::string& field);
            std::optional<DatabaseUser> get_user(std::int64_t chat_id);
            std::vector<DatabaseUser> find_users(const std::string& pattern);

            bool add_user(DatabaseUser& user);
            bool delete_user(std::int64_t chat_id);

            bool ban_user(std::int64_t chat_id);
            bool unban_user(std::int64_t chat_id);

            bool check_user(std::int64_t chat_id);

            bool add_field(std::int64_t chat_id, const std::string& name, const std::string& value);
            bool delete_field(std::int64_t chat_id, const std::string& name);
            bool check_field(std::int64_t chat_id, const std::string& field);
            bool check_is_user_viewed(std::int64_t chat_id, std::int64_t req_chat_id);

            template <typename... ArgType> redisReply* call_custom_redis_command(ArgType&&... args) {
                return static_cast<redisReply*>(redisCommand(this->redis_, std::forward<ArgType>(args)...));
            }

            private:
            std::shared_ptr<spdlog::logger> logger_;
            redisContext* redis_;
        };
    }
}

#endif // JUSTMEET_DATABASE_DBMANAGER_HXX
