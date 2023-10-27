#ifndef UTILITIES_LOGGER_H
#define UTILITIES_LOGGER_H

#include <string>
#include <iostream>
#include <conio.h>

namespace Logger {
#include <windows.h>

    class Logger{
    public:
        std::string name;
        Logger(std::string name) : name(name) {
            reset();
        };

        ~Logger() {
            reset();
        }

        void info(std::string message) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
            std::cout << build_message(message) << std::endl;
            reset();
        }

        void success(std::string message) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            std::cout << build_message(message) << std::endl;
            reset();
        }

        void debug(std::string message) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            std::cout << "DEBUG: " << message << std::endl;
            reset();
        }

        void warn(std::string message) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
            std::cout << build_message(message) << std::endl;
            reset();
        }

        void critical(std::string message) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            std::cout << build_message(message) << std::endl;
            reset();
        }

        void fatal(std::string message) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            std::cout << build_message(message) << std::endl;
            reset();
        }
    private:
        std::string build_message(std::string message) const {
            return "[" + name + "] " + message;
        }

        void reset() const {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);// black background, white foreground
        }
    };
}

#endif //UTILITIES_LOGGER_H
