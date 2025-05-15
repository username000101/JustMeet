#ifndef JUSTMEET_UTILS_STLTYPESREADER_HXX
#define JUSTMEET_UTILS_STLTYPESREADER_HXX

#include <concepts>
#include <sstream>
#include <string>

namespace justmeet {
    namespace utils {
        template <typename T> concept STLNonPairContainer = requires(T container, typename T::value_type v) {
            { container.push_back(v) };
        };

        template <STLNonPairContainer ContainerType> std::string container_reader(ContainerType& container){
            std::ostringstream stream;
            for (auto& element : container)
                stream << element << ";";
            return stream.str();
        }
        template <STLNonPairContainer ContainerType> std::string container_reader(ContainerType&& container) {
            return container_reader<ContainerType>(container);
        }

        inline std::vector<std::string> string_reader(const std::string& str) {
            std::istringstream stream(str);
            std::vector<std::string> result;
            std::string buf;
            while (std::getline(stream, buf, ';'))
                result.push_back(buf);
            return result;
        }

        template <STLNonPairContainer OldContainerType, STLNonPairContainer NewContainerType> NewContainerType cast_container
            (OldContainerType& old_container) {
            NewContainerType result;
            for (auto& element : old_container) {
                if (std::is_same_v<typename NewContainerType::value_type, typename OldContainerType::value_type>)
                    result.push_back(element);
                else
                    result.push_back(typename NewContainerType::value_type(element));
            }
            return result;
        }
        template <STLNonPairContainer OldContainerType, typename NewContainerType> NewContainerType cast_container
            (OldContainerType&& old_container) {
            return cast_container<OldContainerType, NewContainerType>(old_container);
        }

    }
}

#endif // JUSTMEET_UTILS_STLTYPESREADER_HXX
