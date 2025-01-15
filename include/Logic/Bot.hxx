    #ifndef JUSTMEET_LOGIC_BOT_HXX
#define JUSTMEET_LOGIC_BOT_HXX

#include <spdlog/spdlog.h>
#include <tgbot/tgbot.h>


namespace justmeet {
    namespace logic {
        void bot_main(const std::string& token = TG_BOT_TOKEN);
    }
}

#endif // JUSTMEET_LOGIC_BOT_HXX
