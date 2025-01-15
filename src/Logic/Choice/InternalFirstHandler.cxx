#include "Logic/Choice/Choice.hxx"

#include <exception>
#include <sstream>
#include <string>

#include <tgbot/tgbot.h>

void justmeet::logic::choice::ChoiceManager::first_handler(TgBot::CallbackQuery::Ptr query) {
    if (!query) {
        this->logger_->warn("{} ==> Called with an empty query",
                            __PRETTY_FUNCTION__);
        return;
    }

    std::istringstream query_payload(query->data);
    std::string buf;
    std::string command;
    std::uint32_t choice_id = 0;
    while (query_payload >> buf)
        if (command.empty())
            command = buf;
        else if (choice_id == 0) {
            try {
                choice_id = std::stol(buf);
            } catch (std::exception& exc) {
                this->logger_->error("{} ==> {} ==> Couldn't convert the second argument of query payload to int: {}",
                                     __PRETTY_FUNCTION__, query->id, query->data);
                return;
            }

            if (this->choice_variants_map_.count(choice_id) > 0)
                this->second_handler(query, this->choice_variants_map_.at(choice_id));
            else {
                this->logger_->error("==> {} ==> {} ==> The query payload refers to a non-existent choice_id: {}",
                                     __PRETTY_FUNCTION__, query->id, choice_id);
                return;
            }
        } else
            continue; // Just skip other data
}
