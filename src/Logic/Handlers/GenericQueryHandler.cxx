#include "Logic/Handlers/GenericQueryHadnler.hxx"

#include <sstream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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

    if (command == QRY_LOG) {
        std::string logmsg;
        std::string logmsg_buf;
        while (stream >> logmsg_buf)
            logmsg += logmsg_buf + " ";

        logger->info("{} ==> [QRY_LOG] [{}]: {}",
                     __PRETTY_FUNCTION__, query->from->id, logmsg);
        return;
    } else {
        logger->warn("{} ==> [{}] [{}]: Unknown command",
                     __PRETTY_FUNCTION__, command, query->from->id);
        return;
    }
}
