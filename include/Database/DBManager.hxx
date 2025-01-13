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

                enum DatabaseUserLanguage {
                    Russian = 0,
                    English = 1,
                    French  = 2,
                    Deutch  = 3,
                };

                std::string tg_first_name;
                std::string tg_last_name;

                std::string username;
                std::int64_t chat_id;

                std::string name;
                std::uint16_t age;
                std::string bio;
                DatabaseUserGender gender;
                DatabaseUserLanguage language;
                std::string city;

                std::vector<std::uint16_t> preferred_ages;
                std::vector<DatabaseUserLanguage> preferred_languages;
                std::vector<std::string> preferred_cities;
                std::vector<DatabaseUserGender> preferred_genders;

                std::vector<std::filesystem::path> media;
            };

            struct DatabaseUserPreview {
                DatabaseUserPreview(std::int64_t chat_id, DatabaseManager* db) : user_chat_id_(chat_id) {}

                std::optional<DatabaseUser> get() { return this->db_ptr_->get_user(this->user_chat_id_); }
                bool remove() { return this->db_ptr_->delete_user(this->user_chat_id_); }
            private:
                DatabaseManager* db_ptr_;
                std::int64_t user_chat_id_;
            };

            enum DatabaseOperation { GET, DELETE, ADD, UPDATE, BAN, UNBAN, CHECK, };

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

            std::optional<DatabaseUser> get_user(std::int64_t chat_id);
            std::optional<DatabaseUser> update_user(DatabaseUser user);

            bool add_user(DatabaseUser& user);
            bool delete_user(std::int64_t chat_id);

            bool ban_user(std::int64_t chat_id);
            bool unban_user(std::int64_t chat_id);

            bool check_user(std::int64_t chat_id);

            /*DatabaseUserPreview operator[](std::int64_t chat_id) {
                if (!this->connection_.is_open())
                    throw std::runtime_error("Attempt to use DatabaseManager when connection is closed");
                return {chat_id, this};
            }*/
                template <typename... ArgType> redisReply* call_custom_redis_command(ArgType&&... args) {
                return (redisReply*)redisCommand(this->redis_, std::forward<ArgType>(args)...);
            }

            private:
            std::shared_ptr<spdlog::logger> logger_;
            redisContext* redis_;
        };
    }
}

#endif // JUSTMEET_DATABASE_DBMANAGER_HXX
