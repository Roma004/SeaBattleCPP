#ifndef SEND_DATA
#define SEND_DATA

#include "../headers.hpp"


std::queue<json> gRecvDataQueue;
std::queue<json> gSentDataQueue;
std::mutex gReceivedDataMutex;
std::mutex gSentDataMutex;


void sendData(json data) {
    gSentDataMutex.lock();
    gSentDataQueue.push(data);
    gSentDataMutex.unlock();
}

bool ableToReceiveData() {
    return gRecvDataQueue.size() != 0;
}

json receiveData() {

    gReceivedDataMutex.lock();

    json data = gRecvDataQueue.front();
    gRecvDataQueue.pop();

    gReceivedDataMutex.unlock();

    return data;
}

#endif
