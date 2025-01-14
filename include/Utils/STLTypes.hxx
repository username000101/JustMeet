#ifndef JUSTMEET_UTILS_STLTYPESREADER_HXX
#define JUSTMEET_UTILS_STLTYPESREADER_HXX

#include <sstream>
#include <string>

namespace justmeet {
    namespace utils {
        template <typename T> struct is_pair : std::false_type {};
        template <typename T1, typename T2> struct is_pair<std::pair<T1, T2>>
            : std::true_type {};

        template <typename T> constexpr bool is_pair_v = is_pair<T>::value;

        template <typename ContainerType> class ContainerReader {
        public:
            std::string operator()(ContainerType& container) {
                std::ostringstream stream;
                bool first = true;
                stream << "[";
                for (auto& element : container) {
                    if (first) {
                        stream << "\"" << element << "\"";
                        first = false;
                        continue;
                    }
                    stream << ", \"" << element << "\"";
                }
                stream << "]";
                return stream.str();
            }
        };
    }
}

#endif // JUSTMEET_UTILS_STLTYPESREADER_HXX
