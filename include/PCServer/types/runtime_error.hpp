#pragma once

#include <memory>

namespace PCServer{
/**
 * @brief This object represents an runtime error.
 *
 * @ingroup types
 */
class RuntimeError {
public:
    using Ptr = std::shared_ptr<RuntimeError>;
    /**
    * @brief Enum of possible types of a chat.
    */
    enum class Type{
        ClientConnectionError, ServerConnectionError
    };
    /**
     * @brief Unique identifier for this error
     */
    std::int32_t id;

    /**
     * @brief Type of error”
     */
    Type type;

    /**
    * @brief Optional. Specific message while error occur
    */
    std::string message;
};

} //namespace PCServer
