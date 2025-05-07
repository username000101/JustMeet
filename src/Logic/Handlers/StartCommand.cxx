#include "Logic/Handlers/Commands.hxx"

#include <string>

#include <tgbot/tgbot.h>

#include "Logic/Executors/CreateProfile.hxx"
#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Runtime/Storage.hxx"

void justmeet::logic::handlers::commands::start(TgBot::Message::Ptr message) {
    using runtime_storage::bot;
    using runtime_storage::database;
    using executors::CreateProfileStep;

    if (!message)
        return;

    auto reply_parameters = std::make_shared<TgBot::ReplyParameters>
        (message->messageId, message->chat->id);

    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();

    auto button_support = std::make_shared<TgBot::InlineKeyboardButton>();
    button_support->text = "Поддержка";
    button_support->url = "https://t.me/meetx_discussion";
    keyboard->inlineKeyboard.push_back({button_support});

    if (auto value = database->get_field(message->from->id, "current_profile_create_step"); !value.has_value() || (value.has_value() && std::stoi(value.value()) != CreateProfileStep::END)) {
        auto button_create_profile = std::make_shared<TgBot::InlineKeyboardButton>();
        button_create_profile->text = "Создать профиль";
        button_create_profile->callbackData = query::QRY_CREATE;

        keyboard->inlineKeyboard.push_back({button_create_profile});
        bot->getApi().sendMessage(message->chat->id, "Привет, " + message->from->firstName + "! Эта часть ещё в разработке...", nullptr, reply_parameters, keyboard);
    } else {
        auto user = database->get_user(message->from->id);
        if (!user.has_value())
            bot->getApi().sendMessage(message->chat->id, "Привет, незвестный пользователь(вероятно, ошибка redis)! Эта часть ещё в разработке...", nullptr, reply_parameters, keyboard);
        else {
            const auto& user_value = user.value();
            bot->getApi().sendMessage(message->chat->id, "Привет, " + (user_value.name.empty() ? "пользователь без имени" : user_value.name) + "! Эта часть ещё в разработке...", nullptr, reply_parameters, keyboard);
        }
    }
}
