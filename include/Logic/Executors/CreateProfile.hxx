#ifndef JUSTMEET_LOGIC_EXECUTORS_CREATE_PROFILE_HXX
#define JUSTMEET_LOGIC_EXECUTORS_CREATE_PROFILE_HXX

#include <tgbot/tgbot.h>

namespace justmeet {
    namespace logic {
        namespace executors {
            enum CreateProfileStep {
                NAME,
                AGE,
                BIO,
                CITY,
                PREFERRED_AGES,
                PREFERRED_CITIES,
                MEDIA,
                END,
            };
            void create_profile(TgBot::CallbackQuery::Ptr query);
        }
    }
}

#endif // JUSTMEET_LOGIC_EXECUTORS_CREATE_PROFILE_HXX