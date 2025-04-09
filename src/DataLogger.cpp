#include "DataLogger.h"
#include <sstream>

DataLogger::DataLogger() {}

DataLogger::~DataLogger() {}

void DataLogger::logEvent(const std::string &eventType, const std::string &details)
{
    std::ostringstream oss;
    oss << "[" << eventType << "] " << details;
    events.push_back(oss.str());
}

std::vector<std::string> DataLogger::getEvents() const
{
    return events;
}
