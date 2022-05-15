#ifndef SERVER_FOR_TRACKER_CSVREADER_H
#define SERVER_FOR_TRACKER_CSVREADER_H


#include <vector>
#include <include/wx/string.h>
#include "ManualMeasurements.h"

//only reading format with 6 fields is supported
class CsvReader {

public:
    std::vector<ManualMeasurements> readManualMeasurementsFromFile(const wxString &wxString);

private:
    [[nodiscard]] ManualMeasurements exctractManualMeasurements(const std::string &line) const;

    [[nodiscard]] std::vector<int> getIndexesOfSeparators(const std::string &line) const;

    bool isSingleChar(const int currentIndex, const int nextIndex) const;

    bool isSingleChar(std::vector<int> currentIndex) const;

    std::string extractSingleChar(const std::string &lineFromFile, const int currentIndex, const int nextIndex) const;
    std::string extractSingleChar(const std::string &lineFromFile, const int currentIndex) const;

    bool isInputDataInLineCorrectlyStructurized(const std::string &inputLine) const;

    constexpr static char separator{','};
    constexpr static uint32_t supportedFormatOfCsvLine{5};
};

#endif //SERVER_FOR_TRACKER_CSVREADER_H
