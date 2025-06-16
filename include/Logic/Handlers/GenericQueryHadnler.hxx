#ifndef JUSTMEET_LOGIC_HANDLERS_GENERICQUERYHANDLER_HXX
#define JUSTMEET_LOGIC_HANDLERS_GENERICQUERYHANDLER_HXX

#include <string>

#include <tgbot/tgbot.h>

namespace justmeet {
    namespace logic {
        namespace handlers {
            namespace query {
                constexpr std::string intrnl__QRY_GENDER_DEFINE = "qry_gdr";
                constexpr std::string QRY_WARN = "qry_wrn"; /* For make warnings */
                constexpr std::string QRY_LOG = "qry_log"; /* For making a log messages */
                constexpr std::string QRY_REPORT = "qry_rep"; /* For making a report */
                constexpr std::string QRY_NEXT = "qry_nxt"; /* For viewing a next profile */
                constexpr std::string QRY_BACK = "qry_bck"; /* For viewving a previous profile */
                constexpr std::string QRY_LIKE = "qry_lke"; /* For mark users */
                constexpr std::string QRY_CREATE = "qry_crt"; /* For creating a profile */
                constexpr std::string QRY_SET = "qry_set"; /* For setting values in the database */
                constexpr std::string QRY_GET = "qry_get"; /* For getting values from the database */
                constexpr std::string QRY_DELETE = "qry_del"; /* For deleting values from the database */

                using ExecutorSignature = void(*)(TgBot::CallbackQuery::Ptr);

                void add_executor(const std::string& command, ExecutorSignature executor);
                void remove_executor(const std::string& command);
                void generic_query_handler(TgBot::CallbackQuery::Ptr query);
            }
        }
    }
}

#endif // JUSTMEET_LOGIC_HANDLERS_GENERICQUERYHANDLER_HXX
