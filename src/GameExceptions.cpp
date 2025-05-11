#include "GameExceptions.h"

ResourceLoadException::ResourceLoadException(const std::string &message) : GameException("Resource Error: " + message) {}

InvalidEnemyPosition::InvalidEnemyPosition(const std::string &message) : GameException(message) {}

InvalidControlSchemeException::InvalidControlSchemeException(const std::string &message) : GameException(message) {}

InvalidMinigameDifficulty::InvalidMinigameDifficulty(const int difficulty) : GameException("Minigame difficulty " + std::to_string(difficulty) +
                                                                                                    " will spawn no enemies!") {}

