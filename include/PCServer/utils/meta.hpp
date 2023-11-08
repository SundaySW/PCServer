#pragma once

#include <type_traits>
#include <optional>
#include <iterator>

namespace PCServer {

    template<typename T>
    struct function_traits;

    template<typename R, typename Arg>
    struct function_traits<std::function<R(Arg)>> {
        using result_type = R;
        using arg_type = Arg;
    };

    template<typename R, typename ...Args>
    struct function_traits<std::function<R(Args...)>> {
        static const size_t nargs = sizeof...(Args);
        using result_type = R;

        template <size_t i>
        struct arg {
            using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
        };
    };
//    typename function_traits <T>::template arg<0>::type;

namespace logging {

namespace meta{
    template <typename T>
    struct is_pair : std::false_type
    { };

    template <typename T, typename U>
    struct is_pair <std::pair<T, U>> : std::true_type
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
} //namespace PCServer