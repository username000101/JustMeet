#include "Logic/Choice/Choice.hxx"

#include <functional>
#include <memory>

#include <tgbot/tgbot.h>

#include "Runtime/Storage.hxx"

TgBot::InlineKeyboardMarkup::Ptr justmeet::logic::choice::ChoiceManager::create_choice() {
    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();

    std::vector<TgBot::InlineKeyboardButton::Ptr> buttons_on_current_line_vec;
    for (auto& choice : this->choice_variants_map_) {
        if (buttons_on_current_line_vec.size() == 4) {
            keyboard->inlineKeyboard.push_back(buttons_on_current_line_vec);
            buttons_on_current_line_vec.clear();
        } else
            buttons_on_current_line_vec.push_back(choice.second.tg_kb_button_);
    }

    if (!buttons_on_current_line_vec.empty())
        keyboard->inlineKeyboard.push_back(buttons_on_current_line_vec);

    auto call_object = std::bind(&ChoiceManager::first_handler, this, std::placeholders::_1);
    runtime_storage::bot->getEvents().onCallbackQuery(call_object);
    return keyboard;
}
