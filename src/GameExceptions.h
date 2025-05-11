#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H
#include <stdexcept>
#include <memory>
#include <string>

class GameException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ResourceLoadException : public GameException {
public:
    explicit ResourceLoadException(const std::string& message);
};

class InvalidEnemyPosition : public GameException {
private:
public:
    explicit InvalidEnemyPosition(const std::string& message);
};

class InvalidMinigameDifficulty : public GameException {
public:
    explicit InvalidMinigameDifficulty(int difficulty);
};

class InvalidControlSchemeException : public GameException {
public:
    explicit InvalidControlSchemeException(const std::string& message);
};

#endif //GAMEEXCEPTIONS_H