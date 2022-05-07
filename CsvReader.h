//
// Created by Admin on 07.05.2022.
//

#ifndef SERVER_FOR_TRACKER_CSVREADER_H
#define SERVER_FOR_TRACKER_CSVREADER_H


#include <vector>
#include <include/wx/string.h>
#include "ManualMeasurements.h"

class CsvReader {

public:
    std::vector<ManualMeasurements> readManualMeasurementsFromFile(wxString &wxString);
};


#endif //SERVER_FOR_TRACKER_CSVREADER_H
