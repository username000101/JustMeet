#include "Logic/Handlers/GenericQueryHadnler.hxx"

#include <algorithm>
#include <ranges>
#include <sstream>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Runtime/Storage.hxx"
#include "Logic/Executors/CreateProfile.hxx"
#include "Logic/Handlers/NonCommandMessagesHandler.hxx"
#include "Runtime/Storage.hxx"

std::unordered_map<std::string, justmeet::logic::handlers::query::ExecutorSignature> executors = {
    {justmeet::logic::handlers::query::QRY_WARN, [] (TgBot::CallbackQuery::Ptr query) {
            justmeet::runtime_storage::bot->getApi().answerCallbackQuery(query->id, query->data.substr(justmeet::logic::handlers::query::QRY_WARN.length() + 1), true);
        }
    },

    {justmeet::logic::handlers::query::QRY_LOG, [] (TgBot::CallbackQuery::Ptr query) {
            spdlog::log(spdlog::level::info,
                     "{} ==> {}",
                     __PRETTY_FUNCTION__, query->data.substr(justmeet::logic::handlers::query::QRY_LOG.length() + 1));
        }
    },

    {justmeet::logic::handlers::query::QRY_CREATE, justmeet::logic::executors::create_profile},

    {justmeet::logic::handlers::query::QRY_REPORT, [] (TgBot::CallbackQuery::Ptr query) {
#ifndef REPORT_HANDLER_CHAT_ID
            spdlog::log(spdlog::level::warn,
                        "{} ==> The report handler is not defined; skipping report from {}({})",
                        __PRETTY_FUNCTION__, query->from->id, query->message->messageId);
            justmeet::runtime_storage::bot->getApi().answerCallbackQuery(query->id, "The report handler is not defined", true);
#else
            /* Unimplemented */
#endif
        }
    },

    {justmeet::logic::handlers::query::QRY_SET, [] (TgBot::CallbackQuery::Ptr query) {

        }
    },

    {justmeet::logic::handlers::query::intrnl__QRY_GENDER_DEFINE, [] (TgBot::CallbackQuery::Ptr query) {
            using justmeet::runtime_storage::database;
            using justmeet::db::DatabaseManager;
            using justmeet::logic::executors::CreateProfileStep;

            std::string command_buf, value_buf;
            std::istringstream stream(query->data);
            stream >> command_buf >> value_buf;
            if (value_buf == "male")
                database->add_field(query->from->id, "gender", std::to_string(DatabaseManager::DatabaseUser::DatabaseUserGender::MALE));
            else
                database->add_field(query->from->id, "gender", std::to_string(DatabaseManager::DatabaseUser::DatabaseUserGender::FEMALE));
            database->add_field(query->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::GENDER));
            database->add_field(query->from->id, "gender_defined_no_delete_msg", "1");
            query->message->from->id = query->message->chat->id;
            justmeet::logic::handlers::non_command_messages_handler(query->message);
        }
    },
};

void justmeet::logic::handlers::query::add_executor(const std::string& command, justmeet::logic::handlers::query::ExecutorSignature executor) {
    ::executors[command] = executor;
}

void justmeet::logic::handlers::query::remove_executor(const std::string& command) {
    ::executors[command] = nullptr;
}

void justmeet::logic::handlers::query::generic_query_handler(TgBot::CallbackQuery::Ptr query) {
    static auto logger = std::make_shared<spdlog::logger>
        ("QueryHandler", spdlog::sinks_init_list{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()});

    if (query->data.empty()) {
        logger->info("{} ==> Called with an empty payload(from: {})",
                     __PRETTY_FUNCTION__, query->from->id);
        return;
    }

    std::istringstream stream(query->data);
    std::string command;
    stream >> command;

    if (::executors.count(command) > 0 && ::executors.at(command)) {
        logger->log(spdlog::level::debug,
                    "{} ==> Calling the executor \"{}\"",
                    __PRETTY_FUNCTION__, command);
        std::thread executor(::executors.at(command), query);
        executor.detach();
    } else {
        logger->log(spdlog::level::warn,
                    "{} ==> Not found valid executor for command \"{}\"",
                    __PRETTY_FUNCTION__, command);
    }
}
