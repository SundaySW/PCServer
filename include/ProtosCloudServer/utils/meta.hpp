#ifndef PROTOSCLOUDSERVER_META_HPP
#define PROTOSCLOUDSERVER_META_HPP

#include <type_traits>
#include <optional>
#include <iterator>

namespace ProtosCloudServer {

namespace logging {

namespace meta{
    template <typename T>
    struct is_pair : std::false_type
    { };

    template <typename T, typename U>
    struct is_pair<std::pair<T, U>> : std::true_type
    { };

    template <typename T>
    constexpr bool is_pair_v = is_pair<T>::value;

    template<typename, typename = void>
    struct is_mapping : std::false_type
    { };

    template <typename Container>
    struct is_mapping<Container,
            std::enable_if_t<is_pair_v<typename std::iterator_traits<typename Container::iterator>::value_type>>
            >
    : std::true_type
    { };

} //namespace meta
} //namespace logging
} //namespace ProtosCloudServer


#endif //PROTOSCLOUDSERVER_META_HPP