#include "Logic/Choice/Choice.hxx"

#include <tgbot/tgbot.h>

#include "Runtime/Storage.hxx"

void justmeet::logic::choice::ChoiceManager::second_handler(TgBot::CallbackQuery::Ptr query, ChoiceUnit& unit) {
    if (!query) {
        this->logger_->warn("{} ==> Called with an empty query",
                            __PRETTY_FUNCTION__);
        return;
    }
    if (unit.on_click) {
        unit.on_click(query, unit.tg_kb_button_);
        this->sync_units_lists(true);

        runtime_storage::bot->getApi().editMessageText("Updating the information. It may take a few seconds...", query->message->chat->id,
                                                       query->message->messageId);

        runtime_storage::bot->getApi().editMessageText(query->message->text, query->message->chat->id,
                                                       query->message->messageId, "",
                                                       "", nullptr,
                                                       this->create_choice());
    }
}
