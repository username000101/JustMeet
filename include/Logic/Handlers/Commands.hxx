#ifndef JUSTMEET_LOGIC_HANDLERS_STARTCOMMAND_HXX
#define JUSTMEET_LOGIC_HANDLERS_STARTCOMMAND_HXX

#include <spdlog/spdlog.h>
#include <tgbot/tgbot.h>

namespace justmeet {
    namespace logic {
        namespace handlers {
            namespace commands {
                void start(TgBot::Message::Ptr message);
                void help(TgBot::Message::Ptr message);
                void execute(TgBot::Message::Ptr message);
                void find(TgBot::Message::Ptr message);
            }
        }
    }
}

#endif // JUSTMEET_LOGIC_HANDLERS_STARTCOMMAND_HXX
