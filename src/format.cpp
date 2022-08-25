#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

std::string Format::ElapsedTime(long long int seconds) {
  const long long int elapsedHours = seconds / 3600;
  const unsigned short elapsedMinutes = (seconds % 3600) / 60;
  const unsigned short elapsedSeconds = (seconds % 3600) % 60;

  std::ostringstream elapsedTime;
  elapsedTime << std::setfill('0') << std::setw(2) << elapsedHours << ':'
              << std::setfill('0') << std::setw(2) << elapsedMinutes << ':'
              << std::setfill('0') << std::setw(2) << elapsedSeconds;

  return elapsedTime.str();
}