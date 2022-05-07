#include <fstream>
#include "CsvWriter.h"

void CsvWriter::writeManualMeasurements(const wxString &pathToSaveMeasurements, const ManualMeasurements manualMeasurements) {

    std::string path = static_cast<const char *>(pathToSaveMeasurements);
    std::ofstream myfile(path, std::ios::app | std::ios::out);

    myfile << manualMeasurements.startTime <<","<<manualMeasurements.stopTime<<","
           <<manualMeasurements.distance<<","<<manualMeasurements.totalTime<<","<<
           manualMeasurements.distanceInPeriod<<","<<manualMeasurements.averagedVelocity<<"\n";

    myfile.close();
}
