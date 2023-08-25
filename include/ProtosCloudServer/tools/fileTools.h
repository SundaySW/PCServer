#ifndef PROTOSCLOUDSERVER_FILETOOLS_H
#define PROTOSCLOUDSERVER_FILETOOLS_H

#include "export.h"

#include <string>

/**
 * @ingroup tools
 */
namespace fileTools {

/**
 * Reads whole file to string.
 * @param filePath Path to a file
 * @throws exception of type std::ifstream::failure if reading fails
 * @return string with file contents
 */
PROTOSCLOUDSERVER_API
std::string read(const std::string& filePath);

/**
 * Save file to disk.
 * @param filePath Path to a file
 * @throws exception of type std::ifstream::failure if writing fails
*/
PROTOSCLOUDSERVER_API
void write(const std::string& content, const std::string& filePath);

}

#endif //PROTOSCLOUDSERVER_FILETOOLS_H