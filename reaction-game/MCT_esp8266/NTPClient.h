#ifndef MCT_ESP8266_NTPCLIENT_H
#define MCT_ESP8266_NTPCLIENT_H

#include "Arduino.h"
#include <ctime>

class NTPClient {
private:
    const char *TIME_SERVER = "pool.ntp.org";
    int myTimeZone = 1*3600; // +1h compared to UTC

    time_t now;
    // toStringAddZero()
    // this function adds a zero to a date string if its a single digit
    // for example if data = 1, then the function will return 01
    // this makes the date and time string's consistant size for display
    String toStringAddZero(int data);
public:
    NTPClient();
    String getTime();
};


#endif //MCT_ESP8266_NTPCLIENT_H
