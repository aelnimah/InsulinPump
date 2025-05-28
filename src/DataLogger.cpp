#include "DataLogger.h"
#include <sstream>

DataLogger::DataLogger() {}
DataLogger::~DataLogger() {}

// Appends a formatted log entry to the event list
void DataLogger::logEvent(const std::string &eventType, const std::string &details)
{
    std::ostringstream oss;
    oss << "[" << eventType << "] " << details;
    events.push_back(oss.str());
}

// Returns a copy of all events (read-only)
std::vector<std::string> DataLogger::getEvents() const
{
    return events;
}
