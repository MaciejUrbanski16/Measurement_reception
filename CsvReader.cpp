#include <fstream>
#include <iostream>
#include <boost/range/algorithm/count.hpp>
#include "CsvReader.h"

std::vector<ManualMeasurements> CsvReader::readManualMeasurementsFromFile(const wxString &pathToFile) {
    const std::string inputPath{pathToFile};
    std::ifstream inputMeasurementsFromFile(inputPath);
    std::vector<ManualMeasurements> manualMeasurementsFromFile{};

    std::string inputLine{};
    while(inputMeasurementsFromFile >> inputLine)
    {
        if(isInputDataInLineCorrectlyStructurized(inputLine))
        {
            std::cout<<"DEB  input Line from file: " << inputLine <<std::endl;
            ManualMeasurements manualMeasurementsReadFromFile = exctractManualMeasurements(inputLine);
            manualMeasurementsFromFile.push_back(manualMeasurementsReadFromFile);
        }
        else{
            std::cerr<< "WRN Current input line from file is not supported. Only line with six csv fields can be processed!\n";
        }
    }

    inputMeasurementsFromFile.close();

    return manualMeasurementsFromFile;
}

ManualMeasurements CsvReader::exctractManualMeasurements(const std::string &line) const {

   std::vector<int> indexesOfSeparators = getIndexesOfSeparators(line);
   for(const auto i : indexesOfSeparators)
   {
       std::cout<<"DEB index: " << i << std::endl;
   }
   int actualPositionInString{0};
  std::string startTime, stopTime, totalTime, distance, averagedVelocity, distanceInPeriod;
   if(isSingleChar(0, indexesOfSeparators.at(0)))
   {
       startTime = line.substr(0, line.find(separator));
   }
   else{
       startTime = extractSingleChar(line, 0, indexesOfSeparators.at(0));
   }
   if(isSingleChar(indexesOfSeparators.at(0), indexesOfSeparators.at(1)))
   {
       stopTime = line.substr(indexesOfSeparators.at(0) + 1, line.find(separator));
   }
   else{
       stopTime = extractSingleChar(line, indexesOfSeparators.at(0), indexesOfSeparators.at(1));
   }
    if(isSingleChar(indexesOfSeparators.at(1), indexesOfSeparators.at(2)))
    {
        totalTime = line.substr(indexesOfSeparators.at(1) + 1, line.find(separator));
    }
    else{
        totalTime = extractSingleChar(line, indexesOfSeparators.at(1), indexesOfSeparators.at(2));
    }
    if(isSingleChar(indexesOfSeparators.at(2), indexesOfSeparators.at(3)))
    {
        distance = line.substr(indexesOfSeparators.at(2) + 1, line.find(separator));
    }
    else{
        distance = extractSingleChar(line, indexesOfSeparators.at(2), indexesOfSeparators.at(3));
    }
    if(isSingleChar(indexesOfSeparators.at(3), indexesOfSeparators.at(4)))
    {
       averagedVelocity = line.substr(indexesOfSeparators.at(3) + 1, line.find(separator));
    }
    else{
        averagedVelocity = extractSingleChar(line, indexesOfSeparators.at(3), indexesOfSeparators.at(4));
    }
    if(isSingleChar(indexesOfSeparators))
    {
        distanceInPeriod = line.substr(indexesOfSeparators.at(4) + 1, line.find('\n'));
    }
    else{
        distanceInPeriod = extractSingleChar(line, indexesOfSeparators.at(4));
    }

    ManualMeasurements manualMeasurements{startTime, stopTime, totalTime, distance, averagedVelocity, distanceInPeriod};
    std::cout << "DEB READ measurements: " << manualMeasurements <<std::endl;

    return  manualMeasurements;
}

std::vector<int> CsvReader::getIndexesOfSeparators(const std::string &line) const {

    std::vector<int> indexesOfSeparators{};
    for(auto index = 0; index < line.size(); index++)
    {
        if(line.at(index) == separator)
        {
           indexesOfSeparators.push_back(index);
        }
    }
    return indexesOfSeparators;
}

bool CsvReader::isSingleChar(const int currentIndex, const int nextIndex) const {
    if(nextIndex > currentIndex)
    {
        return nextIndex - currentIndex > 2;
    }
    else{
        std::cerr<< "ERR Indexes are invalid: currentIndex: " << currentIndex << "nextIndex: " << nextIndex << std::endl;
        return false;
    }
}

bool CsvReader::isSingleChar(std::vector<int> currentIndex) const {
    if(currentIndex.at(currentIndex.size() - 1) > currentIndex.at(currentIndex.size() - 2))
    {
        return  currentIndex.at(currentIndex.size() - 2) - currentIndex.at(currentIndex.size() - 1) < 2;
    }
    else{
        std::cerr<<"Invalid range of indexes: back-1 : " << currentIndex.at(currentIndex.size() - 2) <<
        "back: " << currentIndex.at(currentIndex.size() - 1);
        return false;
    }
}

std::string CsvReader::extractSingleChar(const std::string &lineFromFile, const int currentIndex, const int nextIndex) const {

    if(nextIndex - currentIndex == 1)
    {
        std::cout<<"The field is empty" <<std::endl;
        return "";
    }
    else if(nextIndex - currentIndex == 2)
    {
        std::string singleCharValue{lineFromFile.at(currentIndex + 1)};
        return singleCharValue;
    }
   if(nextIndex - currentIndex > 2)
   {
       std::cerr<<"ERR There is no single char between indexes: " << currentIndex << ", " << nextIndex <<std::endl;
       return "";
   }
   return "";
}

std::string CsvReader::extractSingleChar(const std::string &lineFromFile, const int currentIndex) const {

    std::string singleCharValue{lineFromFile.at(currentIndex + 1)};
    return singleCharValue;
}

bool CsvReader::isInputDataInLineCorrectlyStructurized(const std::string &inputLine) const {

    return boost::count(inputLine, separator) == supportedFormatOfCsvLine;
}
