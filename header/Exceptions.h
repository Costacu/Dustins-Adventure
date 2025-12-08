#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#pragma once
#include <exception>
#include <string>
#include <utility>

class GameException : public std::exception {
protected:
    std::string msg_;

public:
    explicit GameException(std::string msg)
        : msg_(std::move(msg)) {}

    const char* what() const noexcept override {
        return msg_.c_str();
    }
};

class FileLoadError : public GameException {
public:
    explicit FileLoadError(const std::string& filename)
        : GameException("Failed to load file: " + filename) {}
};

class EngineInitError : public GameException {
public:
    EngineInitError()
        : GameException("Engine Kaput") {}
};

class GameStateError : public GameException {
public:
    explicit GameStateError(const std::string& message)
        : GameException("Game state error: " + message) {}
};

class InvalidPositionError : public GameException {
public:
    InvalidPositionError(float x, float y)
        : GameException(
              "Invalid position: (" +
              std::to_string(x) + ", " +
              std::to_string(y) + ")"
          ) {}
};

#endif // EXCEPTIONS_H
