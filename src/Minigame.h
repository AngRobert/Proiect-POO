#ifndef MINIGAME_H
#define MINIGAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Enemy.h"
#include "MinigamePlayer.h"

class Minigame {
private:
    const int max_enemies;
    const int minigame_difficulty;
    sf::RectangleShape minigame_arena;
    bool minigame_active = false;
    std::vector<Enemy> enemies;
    MinigamePlayer minigame_player;
    sf::Clock minigame_delta_time;
    sf::Font minigame_font;
    sf::Text minigame_text;

    void initMinigameText();
    void initMinigameFont();
    void initMinigameArena();
    void renderEnemies(sf::RenderTarget& target) const;
    float updateMinigameDeltaTime();
    void handleMovementInput(float deltaTime);

public:
    explicit Minigame(int max_enemies_ = 1, int minigame_difficulty_ = 1, const std::vector<Enemy>& enemies_ = {});
    [[nodiscard]] bool isMinigameRunning() const;
    void updateMinigame();
    void renderMinigame(sf::RenderWindow& target) const;
    void pollMinigameEvents(sf::RenderWindow& window, const sf::Event& event, sf::Clock& minigame_timer);

    friend std::ostream& operator<<(std::ostream& os, const Minigame& minigame);
};
#endif // MINIGAME_H