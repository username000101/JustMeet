#include "Logic/Handlers/NonCommandMessagesHandler.hxx"

#include <array>
#include <algorithm>
#include <fstream>
#include <ranges>
#include <memory>

#include <spdlog/spdlog.h>

#include "Logic/Executors/CreateProfile.hxx"
#include "Utils/FS.hxx"
#include "Logic/Handlers/GenericQueryHadnler.hxx"
#include "Runtime/Storage.hxx"
#include "Utils/STLTypes.hxx"

using justmeet::runtime_storage::bot;

std::string intrnl__download_photo(std::vector<TgBot::PhotoSize::Ptr> photo) {
    if (!photo.empty())
        if (auto photo_ = photo.back(); photo_)
            return bot->getApi().downloadFile(bot->getApi().getFile(photo_->fileId)->filePath);
    return "";
}

std::string intrnl__download_video(TgBot::Video::Ptr video) {
    if (video)
        return bot->getApi().downloadFile(bot->getApi().getFile(video->fileId)->filePath);
    return "";
}

std::string intrnl__download_document(TgBot::Document::Ptr document) {
    if (document)
        return bot->getApi().downloadFile(bot->getApi().getFile(document->fileId)->filePath);
    return "";
}

std::string download_media(std::vector<TgBot::PhotoSize::Ptr> media) { return intrnl__download_photo(media); }
std::string download_media(TgBot::Video::Ptr media) { return intrnl__download_video(media); }
std::string download_media(TgBot::Document::Ptr media) { return intrnl__download_document(media); }


void justmeet::logic::handlers::non_command_messages_handler(TgBot::Message::Ptr message) {
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
            const std::string new_msg_text = "Здорово, а сейчас нужно перечислить предпочитаемые города(с переносом строки)(люди из этих городов будут попадаться тебе). Пример:\nМосква\nСанкт-Петербург\nНовосибирск";

            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::PREFERRED_CITIES));

            bot->getApi().deleteMessage(message->chat->id, message->messageId);

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
                    bot->getApi().editMessageText(errors_buffer + "\n\n" + new_msg_text, message->chat->id, std::stoi(value.value()));
                else bot->getApi().sendMessage(message->chat->id, errors_buffer + "\n\n" + new_msg_text, nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
                    if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                        bot->getApi().editMessageText(new_msg_text, message->chat->id, std::stoi(value.value()));
                    else
                        bot->getApi().sendMessage(message->chat->id, new_msg_text, nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
            break;
        }

        case CreateProfileStep::PREFERRED_CITIES: {
            database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::MEDIA));

            bot->getApi().deleteMessage(message->chat->id, message->messageId);

            std::vector<std::string> preferred_cities;
            std::string city_buffer;
            std::istringstream stream(message->text);
            while (std::getline(stream, city_buffer))
                preferred_cities.push_back(city_buffer);
            database->add_field(message->from->id, "preferred_cities", utils::container_reader<decltype(preferred_cities)>(preferred_cities));

            if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                bot->getApi().editMessageText("Атлична, в данный момент тебе нужно скинуть фото/видео, которые будут.. показываться вместе с остальной информацией(?)(если хочешь скинуть несколько - отправляй группой)", message->chat->id, std::stoi(value.value()));
            break;
        }

        case CreateProfileStep::MEDIA: {
            utils::create_directory(FILES_FOLDER + std::string("/") + std::to_string(message->from->id) + "/media");
            auto remove_all_fields = [&message]() {
                database->delete_field(message->from->id, "latest_media_group_id");
                database->delete_field(message->from->id, "latest_media_group_length");
            };

            std::string content;
            if (message->mediaGroupId.empty()) {
                if (!message->photo.empty())
                    content = download_media(message->photo);
                else if (message->video) {
                    if (message->video->duration > VIDEO_DURATION_LIMIT) {
                        if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                            bot->getApi().editMessageText("Видео слишком длинное(максимальная длина видео - " + std::to_string(VIDEO_DURATION_LIMIT) + " секунд)", message->chat->id, std::stoi(value.value()));
                        else
                            bot->getApi().sendMessage(message->chat->id, "Видео слишком длинное(максимальная длина видео - " + std::to_string(VIDEO_DURATION_LIMIT) + " секунд)", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
                        database->add_field(message->from->id, "current_profile_create_step", std::to_string(CreateProfileStep::END));
                        return;
                    }
                    content = download_media(message->video);
                } else if (message->document) {
                    constexpr std::array<const char*, 5> mime_types = {
                        "image/jpeg",
                        "image/png",
                        "image/gif",
                        "image/webp",
                        "video/mp4",
                    };
                    if (std::ranges::find(mime_types, message->document->mimeType) == mime_types.end()) {
                        if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                            bot->getApi().editMessageText("Тип файла не поддерживается", message->chat->id, std::stoi(value.value()));
                        else
                            bot->getApi().sendMessage(message->chat->id, "Тип файла не поддерживается", nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
                    } else content = download_media(message->document);
                }

                std::ofstream outfile(FILES_FOLDER + std::string("/") + std::to_string(message->from->id) + "/media/media_" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()));
                if (content.empty())
                    return;
                outfile.write(content.c_str(), content.size());
                if (auto value = database->get_field(message->chat->id, "last_msg_query_id"); value.has_value())
                    bot->getApi().editMessageText("Файл был успешно добавлен! Лимит: 1/" + std::to_string(MEDIA_LIMIT), message->chat->id, std::stoi(value.value()));
                else
                    bot->getApi().sendMessage(message->chat->id, "Файл был успешно добавлен! Лимит: 1/" + std::to_string(MEDIA_LIMIT), nullptr, (database->get_field(0, "safe_mode").has_value() ? nullptr : reply_parameters));
                return;
            }

            bot->getApi().deleteMessage(message->chat->id, message->messageId);
        }
    }
}
