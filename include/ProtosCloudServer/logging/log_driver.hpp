#ifndef PROTOSCLOUDSERVER_LOG_DRIVER_HPP
#define PROTOSCLOUDSERVER_LOG_DRIVER_HPP

#include <fmt/format.h>
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>

#include "ProtosCloudServer/logging/fwd.hpp"
#include "ProtosCloudServer/logging/level.hpp"

namespace ProtosCloudServer::logging{
    struct NotDetected {};

    template <typename Default, typename AlwaysVoid,
            template <typename...> typename Trait, typename... Args>
    struct Detector {
        using value_t = std::false_type;
        using type = Default;
    };

    template <template <typename...> typename Trait, typename... Args>
    using DetectedType =
            typename Detector<NotDetected, void, Trait, Args...>::type;

    template <typename T>
    using OstreamWriteResult =
            decltype(std::declval<std::ostream&>()
                    << std::declval<const std::remove_reference_t<T>&>());

    template <typename T>
    inline constexpr bool kIsOstreamWritable =
            std::is_same_v<DetectedType<OstreamWriteResult, T>, std::ostream&>;


    inline constexpr std::size_t kBufferSize = 1500;
    inline constexpr char kValueSeparator = ':';
    constexpr inline char kPairsSeparator = '\t';

    class LogDriver final {
    public:
        LogDriver(LoggerRef logger, Level level) noexcept;
        LogDriver(const LoggerPtr& logger, Level level) noexcept;
        ~LogDriver();

        LogDriver(LogDriver&&) = delete;
        LogDriver(const LogDriver&) = delete;
        LogDriver& operator=(LogDriver&&) = delete;
        LogDriver& operator=(const LogDriver&) = delete;

        // Helper function that could be called on LogDriver&& to get LogDriver&.
        LogDriver& AsLvalue() noexcept { return *this; }

        bool IsLimitReached() const noexcept;

        template <typename T>
        LogDriver& operator<<(const T& value) {
            if constexpr (std::is_constructible_v<std::string_view, T>) {
                // noexcept if the conversion is noexcept
                *this << std::string_view{value};
            } else if constexpr (std::is_signed_v<T>) {
                using LongLong = long long;
                *this << LongLong{value};
            } else if constexpr (std::is_unsigned_v<T>) {
                using UnsignedLongLong = unsigned long long;
                *this << UnsignedLongLong{value};
            } else if constexpr (std::is_base_of_v<std::exception, T>) {
                *this << static_cast<const std::exception&>(value);
            } else if constexpr (kIsOstreamWritable<T>) {
                // may throw a non std::exception based exception
                Stream() << value;
                FlushStream();
            }
            return *this;
        }

        LogDriver& operator<<(char value) noexcept;
        LogDriver& operator<<(std::string_view value) noexcept;
        LogDriver& operator<<(float value) noexcept;
        LogDriver& operator<<(double value) noexcept;
        LogDriver& operator<<(long double value) noexcept;
        LogDriver& operator<<(unsigned long long value) noexcept;
        LogDriver& operator<<(long long value) noexcept;
        LogDriver& operator<<(bool value) noexcept;
        LogDriver& operator<<(const std::exception& value) noexcept;

//        LogDriver& operator<<(Hex hex) noexcept;
//        LogDriver& operator<<(HexShort hex) noexcept;
//        LogDriver& operator<<(Quoted value) noexcept;

    private:
        void DoLog() noexcept;
        static void InternalLoggingError(std::string_view message) noexcept;
        void PutFloatingPoint(float value);
        void PutFloatingPoint(double value);
        void PutFloatingPoint(long double value);
        void PutUnsigned(unsigned long long value);
        void PutSigned(long long value);
        void PutBoolean(bool value);
        void Put(std::string_view value);
        void Put(char value);

        void PutRaw(std::string_view value_needs_no_escaping);
        void PutException(const std::exception& ex);
        void PutQuoted(std::string_view value);

        template <typename T>
        void PutRangeElement(const T& value);

        template <typename T, typename U>
        void PutMapElement(const std::pair<const T, U>& value);

        template <typename T>
        void PutRange(const T& range);

        std::ostream& Stream();

        void FlushStream();


        impl::BaseLogger* logger_;
        const Level level_;
        fmt::basic_memory_buffer<char, kBufferSize> msg_;

        void PutMessageEnd();
        void LogMessage() const;
    };
}// namespace ProtosCloudServer::logging

#endif //PROTOSCLOUDSERVER_LOG_DRIVER_HPP
