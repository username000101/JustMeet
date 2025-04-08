#include "Logic/Handlers/GenericQueryHadnler.hxx"

#include <sstream>
#include <unordered_map>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

std::unordered_map<std::string, justmeet::logic::handlers::query::ExecutorSignature> executors;

void justmeet::logic::handlers::query::add_executor(const std::string& command, justmeet::logic::handlers::query::ExecutorSignature executor) {
    executors[command] = executor;
}

void justmeet::logic::handlers::query::remove_executor(const std::string& command) {
    executors[command] = nullptr;
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

    if (executors.count(command) > 0 && executors.at(command)) {
        logger->log(spdlog::level::debug,
                    "{} ==> Calling the executor \"{}\"",
                    __PRETTY_FUNCTION__, command);
        std::thread executor(executors.at(command), query);
        executor.detach();
    } else {
        logger->log(spdlog::level::warn,
                    "{} ==> Not found valid executor for command \"{}\"",
                    __PRETTY_FUNCTION__, command);
    }
}
