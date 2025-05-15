#include "Logic/Executors/CreateProfile.hxx"

#include <memory>

#include <spdlog/spdlog.h>

#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Runtime/Storage.hxx"

void justmeet::logic::executors::create_profile(TgBot::CallbackQuery::Ptr query) {
    using runtime_storage::database;
    using runtime_storage::bot;
    using handlers::query::QRY_WARN;

    auto current_step = database->get_field(query->message->chat->id, "current_profile_create_step");
    if (!current_step.has_value()) {
        database->add_field(query->message->chat->id, "current_profile_create_step", std::to_string(NAME));
        return create_profile(query);
    }

    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
    auto button__for_what = std::make_shared<TgBot::InlineKeyboardButton>();
    button__for_what->callbackData = QRY_WARN + " Так надо.";
    button__for_what->text = "Зачем?";

    keyboard->inlineKeyboard.push_back({button__for_what});

    switch (std::stoi(current_step.value())) {
        case NAME:
            bot->getApi().editMessageText("И так, введи своё имя", query->message->chat->id, query->message->messageId, "", "", nullptr, keyboard);
            break;
        case AGE:
            bot->getApi().editMessageText("А теперь введи свой возраст", query->message->chat->id, query->message->messageId, "", "", nullptr, keyboard);
            break;
        case BIO:
            bot->getApi().editMessageText("Теперь напиши немного о себе(ограничением является лишь длина сообщения тг, так что.. пиши свободно, но старайся придерживаться главной мысли)", query->message->chat->id, query->message->messageId, "", "", nullptr, keyboard);
            break;
        case CITY:
            bot->getApi().editMessageText("А сейчас, пожалуйста, перечисли(через пробел) возраста, которые ты предпочитаешь(люди таких возрастов будут чаще попадаться)", query->message->chat->id, query->message->messageId, "", "", nullptr, keyboard);
            break;
        case PREFERRED_AGES:
            bot->getApi().editMessageText("А сейчас нужно перечислить предпочитаемые города(с переносом строки)(люди из этих городов будут попадаться тебе). Пример:\nМосква\nСанкт-Петербург\nНовосибирск", query->message->chat->id, query->message->messageId, "", "", nullptr, keyboard);
            break;
        case PREFERRED_CITIES:
            bot->getApi().editMessageText("В данный момент тебе нужно скинуть фото/видео, которые будут.. показываться вместе с остальной информацией(?)(если хочешь скинуть несколько - отправляй группой)", query->message->chat->id, query->message->messageId, "", "", nullptr, keyboard);
            break;
        default:
            bot->getApi().answerCallbackQuery(query->id, "Ничего не произошло :(", true);
            break;
    }

    database->add_field(query->message->chat->id, "last_msg_query_id", std::to_string(query->message->messageId));
}
