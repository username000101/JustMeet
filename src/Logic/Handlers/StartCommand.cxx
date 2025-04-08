#include "Logic/Handlers/StartCommand.hxx"

#include <string>

#include <unicode/unistr.h>
#include <tgbot/tgbot.h>

#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Runtime/Storage.hxx"

void handler(TgBot::CallbackQuery::Ptr query, TgBot::InlineKeyboardButton::Ptr kbdb) {
    justmeet::runtime_storage::bot->getApi().sendMessage(query->message->chat->id, "Hello World!", nullptr, std::make_shared<TgBot::ReplyParameters>(query->message->messageId, query->message->chat->id));
}


void justmeet::logic::handlers::commands::start(TgBot::Message::Ptr message) {
    using runtime_storage::bot;
    using runtime_storage::database;

    if (!message)
        return;

    auto button1 = std::make_shared<TgBot::InlineKeyboardButton>();
    button1->text = "Push Me!!! XD";
    button1->callbackData = query::QRY_LOG + " Hello World!";

    auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
    keyboard->inlineKeyboard.push_back({button1});

    auto reply_parameters = std::make_shared<TgBot::ReplyParameters>
        (message->messageId, message->chat->id);

    if (!database->check_user(message->from->id))
        bot->getApi().sendMessage(message->chat->id, "Привет, " + message->from->firstName + "! Эта часть ещё в разработке...", nullptr, reply_parameters, keyboard);
    else {
        auto user = database->get_user(message->from->id);
        if (!user.has_value())
            bot->getApi().sendMessage(message->chat->id, "Привет, незвестный пользователь(вероятно, ошибка redis)! Эта часть ещё в разработке...", nullptr, reply_parameters);
        else {
            auto user_value = user.value();
            bot->getApi().sendMessage(message->chat->id, "Привет, " + (user_value.name.empty() ? "пользователь без имени" : user_value.name) + "! Эта часть ещё в разработке...", nullptr, reply_parameters);
        }
    }
}
