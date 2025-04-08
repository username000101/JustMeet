#ifndef JUSTMEET_LOGIC_HANDLERS_NONCOMMANDMESSAGESHANDLER_HXX
#define JUSTMEET_LOGIC_HANDLERS_NONCOMMANDMESSAGESHANDLER_HXX

#include <tgbot/tgbot.h>

namespace justmeet {
    namespace logic {
        namespace handlers {
            void non_command_messages_handler(TgBot::Message::Ptr message);
        }
    }
}

#endif // JUSTMEET_LOGIC_HANDLERS_NONCOMMANDMESSAGESHANDLER_HXX
