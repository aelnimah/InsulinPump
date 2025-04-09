#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <string>
#include <vector>

class DataLogger {
private:
    std::vector<std::string> events;
public:
    DataLogger();
    ~DataLogger();

    void logEvent(const std::string &eventType, const std::string &details);
    std::vector<std::string> getEvents() const;
};

#endif
