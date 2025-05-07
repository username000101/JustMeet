#include "Logic/Handlers/NonCommandMessagesHandler.hxx"

#include <memory>

#include <spdlog/spdlog.h>

#include "Logic/Executors/CreateProfile.hxx"
#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Runtime/Storage.hxx"

void justmeet::logic::handlers::non_command_messages_handler(TgBot::Message::Ptr message) {
    using runtime_storage::bot;
    using runtime_storage::database;
    using query::QRY_SET;
    using query::QRY_CREATE;
    using query::QRY_WARN;
    using executors::CreateProfileStep;

    auto reply_parameters = std::make_shared<TgBot::ReplyParameters>
    (message->messageId, message->chat->id);

    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();

    auto button__disable_this_notify = std::make_shared<TgBot::InlineKeyboardButton>();
    button__disable_this_notify->text = "Отключить это уведомление";
    button__disable_this_notify->callbackData = QRY_WARN + " Ещё не работает(";
    keyboard->inlineKeyboard.push_back({button__disable_this_notify});

    auto current_step = database->get_field(message->from->id, "current_profile_create_step");
    if (!current_step.has_value()) {
        auto button__create_profile = std::make_shared<TgBot::InlineKeyboardButton>();
        button__create_profile->text = "Создать профиль";
        button__create_profile->callbackData = QRY_CREATE;
        keyboard->inlineKeyboard.push_back({button__create_profile});
        bot->getApi().sendMessage(message->chat->id, "У тебя нет текущих задач, хочешь что-нибудь сделать/отключить это сообщение?", nullptr, reply_parameters, keyboard);
        return;
    }

    switch (std::stoi(current_step.value())) {
        default: {
            spdlog::warn("Unknown switch((justmeet::logic::executors::CreateProfileStep)): {}", current_step.value());
            bot->getApi().editMessageText("Ого, такого варианта(switch((justmeet::logic::executors::CreateProfileStep)))", message->chat->id, message->messageId);
            break;
        }
        case CreateProfileStep::NAME: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::AGE));
            database->add_field(message->from->id, "name", message->text);
            bot->getApi().editMessageText("Отлично, " + message->text + ", приятно познакомиться. Теперь напиши свой возраст", message->chat->id, message->messageId, "", "", nullptr);
            break;
        }
        case CreateProfileStep::AGE: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::BIO));
            database->add_field(message->from->id, "age", message->text);
        }
    }
}
