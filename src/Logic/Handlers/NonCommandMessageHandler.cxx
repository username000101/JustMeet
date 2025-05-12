#include "Logic/Handlers/NonCommandMessagesHandler.hxx"

#include <algorithm>
#include <ranges>
#include <memory>

#include <spdlog/spdlog.h>

#include "Logic/Executors/CreateProfile.hxx"
#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Runtime/Storage.hxx"
#include "Utils/STLTypes.hxx"

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
        bot->getApi().sendMessage(message->chat->id, "У тебя нет текущих задач, хочешь что-нибудь сделать/отключить это сообщение?", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters), keyboard);
        return;
    }

    switch (std::stoi(current_step.value())) {
        default: {
            spdlog::warn("Unknown switch((justmeet::logic::executors::CreateProfileStep)): {}", current_step.value());
            if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                bot->getApi().editMessageText("Ого, такого варианта(switch((justmeet::logic::executors::CreateProfileStep))) нет", message->chat->id, std::stoi(value.value()));
            else
                bot->getApi().sendMessage(message->chat->id, "Ого, такого варианта(switch((justmeet::logic::executors::CreateProfileStep))) нет", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
            break;
        }
        case CreateProfileStep::NAME: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::AGE));
            database->add_field(message->from->id, "name", message->text);

            bot->getApi().deleteMessage(message->chat->id, message->messageId);

            if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                bot->getApi().editMessageText("Отлично, " + message->text + ", приятно познакомиться. Теперь напиши свой возраст", message->chat->id, std::stoi(value.value()));
            else
                bot->getApi().sendMessage(message->chat->id, "Отлично, " + message->text + ", приятно познакомиться. Теперь напиши свой возраст", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
            break;
        }
        case CreateProfileStep::AGE: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::BIO));
            database->add_field(message->from->id, "age", message->text);

            bot->getApi().deleteMessage(message->chat->id, message->messageId);

            if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                bot->getApi().editMessageText("Ого, а сейчас можешь написать о себе(ограничением является лишь длина сообщения тг, так что.. пиши свободно, но старайся придерживаться главной мысли)", message->chat->id, std::stoi(value.value()));
            else
                bot->getApi().sendMessage(message->chat->id, "Ого, а сейчас можешь написать о себе(ограничением является лишь длина сообщения тг, так что.. пиши свободно, но старайся придерживаться главной мысли)", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
            break;
        }
        case CreateProfileStep::BIO: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::CITY));
            database->add_field(message->from->id, "bio", message->text);

            bot->getApi().deleteMessage(message->chat->id, message->messageId);

            if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                bot->getApi().editMessageText("Охты, ну а теперь напиши свой город(в идеале точное название, например: не Спб, а Санкт-Питербург, или что-то в этом роде)", message->chat->id, std::stoi(value.value()));
            else
                bot->getApi().sendMessage(message->chat->id, "Охты, ну а теперь напиши свой город(в идеале точное название, например: не Спб, а Санкт-Питербург, или что-то в этом роде)", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
            break;
        }
        case CreateProfileStep::CITY: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::PREFERRED_AGES));
            database->add_field(message->from->id, "city", message->text);

            bot->getApi().deleteMessage(message->chat->id, message->messageId);

            if (auto value = database->get_field(message->chat->id,  "last_msg_query_id"); value.has_value())
                bot->getApi().editMessageText("Окей, учтём. А сейчас, пожалуйста, перечисли(через пробел) возраста, которые ты предпочитаешь(люди таких возрастов будут чаще попадаться)", message->chat->id, std::stoi(value.value()));
            else
                bot->getApi().sendMessage(message->chat->id, "Окей, учтём. А сейчас, пожалуйста, перечисли(через пробел) возраста, которые ты предпочитаешь(люди таких возрастов будут чаще попадаться)", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
            break;
        }

        case CreateProfileStep::PREFERRED_AGES: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::PREFERRED_CITIES));
            std::vector<int> preferred_ages;
            std::string age_buffer, errors_buffer;
            std::istringstream stream(message->text);
            while (std::getline(stream, age_buffer, ' ')) {
                try {
                    preferred_ages.push_back(std::stoi(age_buffer));
                } catch (std::invalid_argument& casterr) {
                    errors_buffer += "Ошибка в числе \"" + age_buffer + "\": похоже, что это не число(оно будет пропущено)\n";
                }
            }

            database->add_field(message->from->id, "preferred_ages", utils::container_reader<decltype(preferred_ages)>(preferred_ages));
            if (!errors_buffer.empty())
                if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                    bot->getApi().editMessageText(errors_buffer, message->chat->id, std::stoi(value.value()));
                else bot->getApi().sendMessage(message->chat->id, errors_buffer, nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
            break;
        }
    }
}
