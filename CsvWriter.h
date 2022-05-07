//
// Created by Admin on 07.05.2022.
//

#ifndef SERVER_FOR_TRACKER_CSVWRITER_H
#define SERVER_FOR_TRACKER_CSVWRITER_H


#include <include/wx/string.h>
#include "ManualMeasurements.h"

class CsvWriter {

public:
    void writeManualMeasurements(const wxString &pathToSaveMeasurements,
                                 const ManualMeasurements measurements);
};


#endif //SERVER_FOR_TRACKER_CSVWRITER_H
