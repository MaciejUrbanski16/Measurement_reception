//
// Created by Admin on 03.03.2022.
//

#ifndef SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
#define SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H


#include <array>

class RemoteDataHandler {
public:
    explicit RemoteDataHandler(const std::array<char, 25> &buffer) : buffer(buffer) {}

private:
    std::array<char, 25> buffer{};
};


#endif //SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
