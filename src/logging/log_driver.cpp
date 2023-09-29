#include <ProtosCloudServer/logging/log_driver.hpp>

namespace ProtosCloudServer::logging{

    void LogDriver::PutMessageEnd(){
        try {
            Put('\n');
        } catch (...) {
            InternalLoggingError("Failed to finish log msg");
        }
    }

    void LogDriver::LogMessage() const{
        const std::string_view message(msg_.data(), msg_.size());
        logger_.Log(level_, message);
    }

    void LogDriver::DoLog() noexcept {
        try {
            PutMessageEnd();
            LogMessage();
        } catch (...) {
            InternalLoggingError("Failed to flush log");
        }
    }

    void LogDriver::InternalLoggingError(std::string_view message) noexcept {
        try{
            std::cerr << "LogDriver: " << message << ". "
                      << boost::current_exception_diagnostic_information() << '\n';
        } catch (...) {
            // ignore
        }
    }

    LogDriver::~LogDriver() {
        DoLog();
    }

    void LogDriver::FlushStream() {
        logger_.Flush();
    }

    bool LogDriver::IsLimitReached() const noexcept {
        return msg_.size() >= kSizeLimit;
    }

    void LogDriver::Put(std::string_view value) {
        try{
            msg_.append(value);
        }catch (...) {
            InternalLoggingError("Failed to log std::string_view");
        }
    }

    void LogDriver::Put(char value) {
        try{
            msg_.push_back(value);
        }catch (...) {
            InternalLoggingError("Failed to append a std::char log std::char");
        }
    }

    template<typename T>
    void LogDriver::PutValue(T value) {
        fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{}"), value);
    }
//
//    void LogDriver::PutValue(float value) {
//        fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{}"), value);
//    }
//
//    void LogDriver::PutValue(double value) {
//        fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{}"), value);
//    }
//
//    void LogDriver::PutValue(long double value) {
//        fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{}"), value);
//    }
//
//    void LogDriver::PutUnsigned(unsigned long long int value) {
//        fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{}"), value);
//    }
//
//    void LogDriver::PutSigned(long long int value) {
//        fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{}"), value);
//    }
//
//    void LogDriver::PutBoolean(bool value) {
//        fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{}"), value);
//    }

    void LogDriver::PutRaw(std::string_view value_needs_no_escaping) {
        msg_.append(value_needs_no_escaping);
    }

    constexpr bool NeedsQuoteEscaping(char c) { return c == '\"' || c == '\\'; }

    void LogDriver::PutQuoted(std::string_view value) {
        constexpr size_t kQuotesSize = 2;

        const auto old_message_size = msg_.size();
        const auto allowed_size =
                std::max(kSizeLimit, old_message_size + kQuotesSize) -
                (old_message_size + kQuotesSize);
        size_t used_size = 0;

        Put('\"');

        for (const char c : value) {
            const bool needs_escaping = NeedsQuoteEscaping(c);
            const size_t escaped_size = needs_escaping ? 2 : 1;

            used_size += escaped_size;
            if (used_size > allowed_size) break;

            if (needs_escaping) {
                Put('\\');
            }
            Put(c);
        }

        if (used_size > allowed_size) {
            PutRaw("...");
        }

        Put('\"');
    }

    template<typename T>
    void LogDriver::PutRangeElement(const T &value) {
        if constexpr (std::is_constructible_v<std::string_view, T>) {
            *this << Quoted{value};
        } else {
            *this << value;
        }
    }

    template<typename T, typename U>
    void LogDriver::PutMapElement(const std::pair<const T, U> &value) {
        PutRangeElement(value.first);
        *this << ": ";
        PutRangeElement(value.second);
    }

    template<typename T>
    void LogDriver::PutRange(const T &range) {
        using std::begin;
        using std::end;

        constexpr std::string_view kSeparator = ", ";
        *this << '[';

        bool is_first = true;
        auto curr = begin(range);
        const auto end_iter = end(range);

        while (curr != end_iter) {
            if (IsLimitReached()) {
                break;
            }
            if (is_first) {
                is_first = false;
            } else {
                *this << kSeparator;
            }
            if constexpr (meta::is_mapping<T>::value) {
                PutMapElement(*curr);
            } else {
                PutRangeElement(*curr);
            }
            ++curr;
        }

        const auto extra_elements = std::distance(curr, end_iter);

        if (extra_elements != 0) {
            if (!is_first) {
                *this << kSeparator;
            }
            *this << "..." << extra_elements << " more";
        }

        *this << ']';
    }

    LogDriver &LogDriver::operator<<(std::string_view value) noexcept {
        try {
            Put(value);
        } catch (...) {
            InternalLoggingError("Failed to log std::string_view");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(char value) noexcept {
        try {
            Put(value);
        } catch (...) {
            InternalLoggingError("Failed to log char");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(float value) noexcept {
        try {
            PutValue(value);
        } catch (...) {
            InternalLoggingError("Failed to log float");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(double value) noexcept {
        try {
            PutValue(value);
        } catch (...) {
            InternalLoggingError("Failed to log double");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(long double value) noexcept {
        try {
            PutValue(value);
        } catch (...) {
            InternalLoggingError("Failed to log long double");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(unsigned long long int value) noexcept {
        try {
            PutValue(value);
        } catch (...) {
            InternalLoggingError("Failed to log unsigned");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(long long int value) noexcept {
        try {
            PutValue(value);
        } catch (...) {
            InternalLoggingError("Failed to log signed");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(bool value) noexcept {
        try {
            PutValue(value);
        } catch (...) {
            InternalLoggingError("Failed to log bool");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(Hex hex) noexcept {
        try {
            fmt::format_to(fmt::appender(msg_), FMT_COMPILE("0x{:016X}"), hex.value);
        } catch (...) {
            InternalLoggingError("Failed to extend log Hex");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(HexShort hex) noexcept {
        try {
            fmt::format_to(fmt::appender(msg_), FMT_COMPILE("{:X}"), hex.value);
        } catch (...) {
            InternalLoggingError("Failed to extend log HexShort");
        }
        return *this;
    }

    LogDriver &LogDriver::operator<<(Quoted value) noexcept {
        try {
            PutQuoted(value.string);
        } catch (...) {
            InternalLoggingError("Failed to log quoted string");
        }
        return *this;
    }

}// namespace ProtosCloudServer::logging