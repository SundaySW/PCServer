#ifndef PROTOSCLOUDSERVER_LOG_DRIVER_HPP
#define PROTOSCLOUDSERVER_LOG_DRIVER_HPP

#include <fmt/format.h>
#include "fmt/compile.h"
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>

#include "ProtosCloudServer/logging/fwd.hpp"
#include "ProtosCloudServer/logging/level.hpp"
#include <ProtosCloudServer/logging/impl/base_logger.hpp>
#include "ProtosCloudServer/logging/null_logger.hpp"
#include "ProtosCloudServer/utils/meta.hpp"

namespace ProtosCloudServer::logging{

    namespace impl {
    struct HexBase {
        std::uint64_t value;
        template <typename Unsigned, typename = std::enable_if_t<std::is_unsigned_v<Unsigned>>>
        explicit constexpr HexBase(Unsigned value) noexcept : value(value)
        {
            static_assert(sizeof(Unsigned) <= sizeof(value));
        }
        template <typename T>
        explicit HexBase(T* pointer) noexcept
            : HexBase(reinterpret_cast<std::uintptr_t>(pointer))
        {
            static_assert(sizeof(std::uintptr_t) <= sizeof(value));
        }
    };
    }  // namespace impl

    /// Formats value in a hex mode with the fixed length representation.
    struct Hex final : impl::HexBase {
        using impl::HexBase::HexBase;
    };

    /// Formats value in a hex mode with the shortest representation, just like
    /// std::to_chars does.
    struct HexShort final : impl::HexBase {
        using impl::HexBase::HexBase;
    };

    /// Formats a string as quoted, escaping the '\' and '"' symbols.
    struct Quoted final {
        std::string_view string;
    };

    inline constexpr std::size_t kBufferSize = 1500;
    inline constexpr size_t kSizeLimit = 10000;
    inline constexpr char kValueSeparator = ':';
    constexpr inline char kPairsSeparator = '\t';

    class LogDriver final {
    public:
        LogDriver(impl::BaseLogger &logger, Level level) noexcept
                :logger_(logger), level_(level)
        {}
        LogDriver(const LoggerPtr &logger, Level level) noexcept
                :logger_(logger ? *logger : logging::GetNullLogger()), level_(level)
        {}
        ~LogDriver();

        LogDriver(LogDriver&&) = delete;
        LogDriver(const LogDriver&) = delete;
        LogDriver& operator=(LogDriver&&) = delete;
        LogDriver& operator=(const LogDriver&) = delete;

        // Helper function that could be called on LogDriver&& to get LogDriver&.
        LogDriver& AsLvalue() noexcept { return *this; }

        [[nodiscard]] bool IsLimitReached() const noexcept;

        template <typename T>
        LogDriver& operator<<(const T& value) {
            if constexpr (std::is_constructible_v<std::string_view, T>) {
                *this << std::string_view{value};
            } else if constexpr (std::is_signed_v<T>) {
                using LongLong = long long;
                *this << LongLong{value};
            } else if constexpr (std::is_unsigned_v<T>) {
                using UnsignedLongLong = unsigned long long;
                *this << UnsignedLongLong{value};
            } else if constexpr (std::is_base_of_v<std::exception, T>) {
                *this << static_cast<const std::exception&>(value);
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
        LogDriver& operator<<(Hex hex) noexcept;
        LogDriver& operator<<(HexShort hex) noexcept;
        LogDriver& operator<<(Quoted value) noexcept;

    private:
        impl::BaseLogger& logger_;
        const Level level_;
        fmt::basic_memory_buffer<char, kBufferSize> msg_;

        void DoLog() noexcept;
        static void InternalLoggingError(std::string_view message) noexcept;

        void Put(std::string_view value);
        void Put(char value);

        template<typename T>
        void PutValue(T value);

        void PutRaw(std::string_view value_needs_no_escaping);
        void PutQuoted(std::string_view value);

        template <typename T>
        void PutRangeElement(const T& value);

        template <typename T, typename U>
        void PutMapElement(const std::pair<const T, U>& value);

        template <typename T>
        void PutRange(const T& range);

        void FlushStream();
        void PutMessageEnd();
        void LogMessage() const;
    };
}// namespace ProtosCloudServer::logging

#endif //PROTOSCLOUDSERVER_LOG_DRIVER_HPP