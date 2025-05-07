#include "Logic/Bot.hxx"

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tgbot/tgbot.h>

#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Logic/Handlers/NonCommandMessagesHandler.hxx"
#include "Logic/Handlers/Commands.hxx"
#include "Runtime/Storage.hxx"

void justmeet::logic::bot_main(const std::string& token) {
    static auto logger = std::make_shared<spdlog::logger>
        ("Bot", spdlog::sinks_init_list{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()});
    runtime_storage::bot = std::make_shared<TgBot::Bot>(token);

    runtime_storage::bot->getEvents().onNonCommandMessage(handlers::non_command_messages_handler);
    runtime_storage::bot->getEvents().onCallbackQuery(handlers::query::generic_query_handler);
    runtime_storage::bot->getEvents().onCommand("start", handlers::commands::start);


    try {
        TgBot::TgLongPoll lpoll(*runtime_storage::bot);
        auto bot_info = runtime_storage::bot->getApi().getMe();
        logger->info("{} ==> The bot is running: {},{}",
                     __PRETTY_FUNCTION__, bot_info->firstName, bot_info->username);
        while (true)
            lpoll.start();
    } catch (TgBot::TgException& tgerr) {
        logger->critical("{} ==> Exception received: {}",
                         __PRETTY_FUNCTION__, tgerr.what());
    }
}
