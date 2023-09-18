#ifndef VERSION_H
#define VERSION_H

#include <string>

#define VERSION_MAJOR 0
#define VERSION_MINOR 2
#define VERSION_PATCH 0

const std::string
getVersionString()
{
  return "v" + std::to_string(VERSION_MAJOR) + "." +
         std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_PATCH);
}

#endif // VERSION_H
