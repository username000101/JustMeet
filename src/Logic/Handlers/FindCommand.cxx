#include "Logic/Handlers/Commands.hxx"

#include <spdlog/spdlog.h>

#include "Logic/Executors/CreateProfile.hxx"
#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Runtime/Storage.hxx"

void justmeet::logic::handlers::commands::find(TgBot::Message::Ptr message) {
    using runtime_storage::bot;
    using runtime_storage::database;
    using executors::CreateProfileStep;

    auto reply_parameters = std::make_shared<TgBot::ReplyParameters>(message->messageId, message->chat->id);

    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();

    auto button_support = std::make_shared<TgBot::InlineKeyboardButton>();
    button_support->text = "Поддержка";
    button_support->url = "https://t.me/meetx_discussion";
    keyboard->inlineKeyboard.push_back({button_support});

    if (database->check_user(message->from->id)) {
        if (auto value = database->get_field(message->from->id, "current_profile_create_step"); value.has_value() && std::stoi(value.value()) == CreateProfileStep::END) {
            
        }
    }

    auto button_create_profile = std::make_shared<TgBot::InlineKeyboardButton>();
    button_create_profile->text = "Создать профиль";
    button_create_profile->callbackData = query::QRY_CREATE;
    keyboard->inlineKeyboard.push_back({button_create_profile});
    bot->getApi().sendMessage(message->chat->id, "Похоже, что ты ещё не создал профиль.", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters), keyboard);
}