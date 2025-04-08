#ifndef JUSTMEET_LOGIC_HANDLERS_GENERICQUERYHANDLER_HXX
#define JUSTMEET_LOGIC_HANDLERS_GENERICQUERYHANDLER_HXX

#include <string>

#include <tgbot/tgbot.h>

namespace justmeet {
    namespace logic {
        namespace handlers {
            namespace query {
                const std::string QRY_LOG = "query_log"; /* For making a log messages */
                const std::string QRY_REPORT = "query_report"; /* For making a report */
                const std::string QRY_NEXT = "query_next"; /* For viewing a next profile */
                const std::string QRY_BACK = "query_back"; /* For viewving a previous profile */
                const std::string QRY_LIKE = "query_like"; /* For mark users */
                const std::string QRY_CREATE = "query_create"; /* For creating a profile */

                using ExecutorSignature = void(*)(TgBot::CallbackQuery::Ptr);

                void add_executor(const std::string& command, ExecutorSignature executor);
                void remove_executor(const std::string& command);
                void generic_query_handler(TgBot::CallbackQuery::Ptr query);
            }
        }
    }
}

#endif // JUSTMEET_LOGIC_HANDLERS_NONCOMMANDMESSAGESHANDLER_HXX
