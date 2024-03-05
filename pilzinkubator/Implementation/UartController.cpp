//
// Created by steffen on 13.02.24.
//

#include "UartController.h"
#include "Helper.h"

UartController::UartController() {
    posix_io::inst.register_stdin ( uart );
    posix_io::inst.register_stdout( uart );
    posix_io::inst.register_stderr( uart );

    uart.uartAttachIrq([this](char c) { this->OnInterrupt(c); });
}

void UartController::OnInterrupt(char c) {
    if(c == '\n' || c == '\r')
    {
        std::cout << std::endl;
        InterpretCommand();
        buffer.clear();
    }
    else if(c == '\177' && buffer.size() > 0)
    {
        buffer.pop_back();
        std::cout << "\b";
        std::cout << " ";
        std::cout << "\b";
    }
    else if(c != '\177')
    {
        buffer.push_back(c);
        std::cout << c;
    }
    fflush(stdout);
    fflush(stderr);
    fflush(stdin);
}

void UartController::InterpretCommand() {
    std::string params[2] = {"", ""};
    int p = 0;
    for(char c : buffer)
    {
        if(c == ' ')
            p = 1;
        else
        {
            // To Lower Case
            params[p] += p == 0 ? (c < 91 ? c + 32 : c) : c;
        }
    }

    if(p == 0)
    {
        // No Parameter
        if(voidCommands.contains(params[0]))
        {
            voidCommands[params[0]]();
        }
        else if(boolCommands.contains(params[0]))
        {
            if(boolCommands[params[0]]()) {
                std::cout << "On" << std::endl;
            }
            else
            {
                std::cout << "Off" << std::endl;
            }
        }
        else if(doubleCommands.contains(params[0]))
        {
            double res = doubleCommands[params[0]]();
            std::cout << Helper::doubleToString(res).c_str() << std::endl;
        }
        else
        {
            std::cerr << "Command not found!" << std::endl;
        }
    }
    else
    {
        // Has Parameter
        if(doubleSetCommands.contains(params[0]))
        {
            double parsedDouble = std::stod(params[1]);
            doubleSetCommands[params[0]](parsedDouble);
        }
        else
        {
            std::cerr << "Command not found!" << std::endl;
        }
    }
}

template<> void UartController::registerCommand(const std::string& cmd, const std::function<void()> func) {
    if(voidCommands.contains(cmd))
    {
        std::cout << "Command bereits in Verwendung: " << cmd.c_str() << std::endl;
    }
    voidCommands[cmd] = func;
}

template<> void UartController::registerCommand(const std::string& cmd, const std::function<bool()> func) {
    if(boolCommands.contains(cmd))
    {
        std::cout << "Command bereits in Verwendung: " << cmd.c_str() << std::endl;
    }
    boolCommands[cmd] = func;
}

template<> void UartController::registerCommand(const std::string& cmd, const std::function<double()> func) {
    if(doubleCommands.contains(cmd))
    {
        std::cout << "Command bereits in Verwendung: " << cmd.c_str() << std::endl;
    }
    doubleCommands[cmd] = func;
}

template<> void UartController::registerCommand(const std::string& cmd, const std::function<void(double)> func) {
    if(doubleSetCommands.contains(cmd))
    {
        std::cout << "Command bereits in Verwendung: " << cmd.c_str() << std::endl;
    }
    doubleSetCommands[cmd] = func;
}

void UartController::helpCommand() {
    std::cout << "Liste aller verfügbaren Befehle:" << std::endl << std::endl;
    for(auto kv : voidCommands) {
        std::cout << "- " << kv.first << "()" << std::endl;
    }
    for(auto kv : boolCommands) {
        std::cout << "- " << kv.first << "() -> bool" << std::endl;
    }
    for(auto kv : doubleCommands) {
        std::cout << "- " << kv.first << "() -> double" << std::endl;
    }
    for(auto kv : doubleSetCommands) {
        std::cout << "- " << kv.first << "(double)" << std::endl;
    }
    std::cout << std::endl;
}

void UartController::writeToOutput(const char *text) {

}


