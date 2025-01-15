#ifndef JUTMEET_LOGIC_CHOICE_CHOICE_HXX
#define JUTMEET_LOGIC_CHOICE_CHOICE_HXX

#include <cstdint>
#include <unordered_map>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <tgbot/tgbot.h>

namespace justmeet {
    namespace logic {
        namespace choice {
            struct ChoiceUnit {
                using ChoiceOnClickHandler = void(*)(TgBot::CallbackQuery::Ptr query, TgBot::InlineKeyboardButton::Ptr keyboard_button);

                std::uint32_t choice_id;
                ChoiceOnClickHandler on_click;
                TgBot::InlineKeyboardButton::Ptr tg_kb_button_;
            };

            class ChoiceManager {
            public:
                ChoiceManager(std::vector<ChoiceUnit>& variants)
                    : choice_variants_(variants), logger_(std::make_shared<spdlog::logger>
                              ("ChoiceManager", spdlog::sinks_init_list{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()})) {
                    this->sync_units_lists();
                }

                ChoiceManager(std::vector<ChoiceUnit>&& variants)
                    : choice_variants_(variants), logger_(std::make_shared<spdlog::logger>
                              ("ChoiceManager", spdlog::sinks_init_list{std::make_shared<spdlog::sinks::stdout_color_sink_mt>()})) {
                    this->sync_units_lists();
                }

                TgBot::InlineKeyboardMarkup::Ptr create_choice();
            private:
                std::vector<ChoiceUnit> choice_variants_;
                std::unordered_map<std::uint32_t, ChoiceUnit> choice_variants_map_;

                void first_handler(TgBot::CallbackQuery::Ptr query);
                void second_handler(TgBot::CallbackQuery::Ptr query, ChoiceUnit& choice);

                inline void sync_units_lists(bool reverse = false) {
                    if (reverse) {
                        this->choice_variants_.clear();
                        for (auto& choice_unit : this->choice_variants_map_)
                            this->choice_variants_.push_back(choice_unit.second);
                    } else {
                        this->choice_variants_map_.clear();
                        for (auto& choice_unit : this->choice_variants_)
                            this->choice_variants_map_[choice_unit.choice_id] = choice_unit;
                    }
                }

                std::shared_ptr<spdlog::logger> logger_;
            };
        }
    }
}

#endif // JUTMEET_LOGIC_CHOICE_CHOICE_HXX
