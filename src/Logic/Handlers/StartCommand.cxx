#include "Logic/Handlers/StartCommand.hxx"

#include <string>

#include <unicode/unistr.h>
#include <tgbot/tgbot.h>

#include "Runtime/Storage.hxx"


void justmeet::logic::handlers::commands::start(TgBot::Message::Ptr message) {
    using runtime_storage::bot;
    using runtime_storage::database;

    if (!message)
        return;


    auto reply_parameters = std::make_shared<TgBot::ReplyParameters>
        (message->messageId, message->chat->id);

    if (!database->check_user(message->from->id))
        bot->getApi().sendMessage(message->chat->id, "Hello, new user! This part is in work, please wait...", nullptr, reply_parameters);
    else {
        auto user = database->get_user(message->from->id);
        if (!user.has_value())
            bot->getApi().sendMessage(message->chat->id, "Hello, unknown user(maybe error?)! This part is in work, please wait...", nullptr, reply_parameters);
        else {
            auto user_value = user.value();
            bot->getApi().sendMessage(message->chat->id, "Hello, " + (user_value.name.empty() ? "user without name!" : user_value.name) + "! This part is in work, please wait...", nullptr, reply_parameters);
        }
    }
}
