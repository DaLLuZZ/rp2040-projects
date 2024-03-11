#include "NTPClient.h"

NTPClient::NTPClient(){
    configTime(myTimeZone, 0, TIME_SERVER);
}

String NTPClient::toStringAddZero(int data)
{
    String st = "";
    if (data < 10)
    {
        st = "0" + String(data);
    }
    else
    {
        st = String(data);
    }
    return st;
}

String NTPClient::getTime() {
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now);

    int year = timeinfo->tm_year + 1900;
    int month = timeinfo->tm_mon;
    int day = timeinfo->tm_mday;
    int hour = timeinfo->tm_hour;
    int mins = timeinfo->tm_min;
    int sec = timeinfo->tm_sec;
    int day_of_week = timeinfo->tm_wday;

    String format = String(year) + "-" + toStringAddZero(month+1) + "-" + toStringAddZero(day) + "T" + toStringAddZero(hour) + ":" + toStringAddZero(mins) + ":" + toStringAddZero(sec);
    return format;
}