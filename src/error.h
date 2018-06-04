#ifndef EXCEPT_H
#define EXCEPT_H

#include <exception>
#include <string>

using namespace std;

class Error : public exception {
    const char* message;

public:
    Error(const char* msg) {
        message = msg;
    }

    virtual const char* what() const throw() {
        return message;
    }
};

class ErrorCreatingThread : public Error {
public:
    ErrorCreatingThread()
    : Error("Could not create a thread") {}
};

class ErrorJoiningThread : public Error {
public:
    ErrorJoiningThread()
    : Error("Could not join a thread") {}
};

class ErrorOpeningFile : public Error {
public:
    ErrorOpeningFile()
    : Error("Could not open file") {}
};

#endif // EXCEPT_H