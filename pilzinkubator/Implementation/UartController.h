//
// Created by steffen on 13.02.24.
//

#ifndef IMPLEMENTATION_UARTCONTROLLER_H
#define IMPLEMENTATION_UARTCONTROLLER_H

#include "yahal_String.h"
#include "uart_rp2040.h"
#include "posix_io.h"
#include <string>
#include <functional>
#include <vector>
#include <map>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <cstdio>
#include <sstream>

class UartController {

    uart_rp2040 uart;
    std::vector<char> buffer;

    std::map<const std::string, function<void()>> voidCommands;
    std::map<const std::string, function<bool()>> boolCommands;
    std::map<const std::string, function<double()>> doubleCommands;
    std::map<const std::string, function<void(double)>> doubleSetCommands;

    void OnInterrupt(char c);
    void InterpretCommand();


public:
    UartController();

    template<typename T>
    void registerCommand(const std::string& cmd, const T func) {
        std::cout << "Not implemented!" << std::endl;
    }

    void start() { uart.uartEnableIrq(); }
    void stop() { uart.uartDisableIrq(); }

    void writeToOutput(const char* text);
    void helpCommand();
};


#endif //IMPLEMENTATION_UARTCONTROLLER_H
