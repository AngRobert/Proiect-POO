#ifndef MINIGAME_H
#define MINIGAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Fish.h"
#include "MinigamePlayer.h"

class Minigame {
private:
    int max_enemies;
    int minigame_difficulty;
    float enemy_spawn_timer;
    sf::Clock enemy_spawn_clock;
    sf::Clock minigame_end_timeout_clock;
    sf::RectangleShape minigame_arena;
    bool minigame_active = false;
    bool minigame_failed = false;
    bool minigame_success = false;
    std::vector<std::unique_ptr<Enemy>> enemies;
    MinigamePlayer minigame_player;
    sf::Clock minigame_delta_time;
    sf::Font minigame_font;
    sf::Text minigame_text;

    void initMinigameText();
    void initMinigameFont();
    void initMinigameArena();
    void updateEnemies();
    void renderEnemies(sf::RenderTarget& target) const;
    float updateMinigameDeltaTime();
    void handleMovementInput(float deltaTime);
    void handleEnemySpawns();
    void handleMinigameEnd(sf::Clock& minigame_timer);

public:
    explicit Minigame(int max_enemies_ = 10, int minigame_difficulty_ = 1, float enemy_spawn_timer_ = 0.5f);
    Minigame(const Minigame& other);
    Minigame& operator=(const Minigame& other);
    ~Minigame() = default;

    [[nodiscard]] bool isMinigameRunning() const;
    [[nodiscard]] bool isMinigameFailed() const;
    void updateMinigame(sf::Clock& minigame_timer);
    void renderMinigame(sf::RenderWindow& target) const;
    void pollMinigameEvents(sf::RenderWindow& window, const sf::Event& event, sf::Clock& minigame_timer);
    void generateEnemy();

    friend std::ostream& operator<<(std::ostream& os, const Minigame& minigame);
};
#endif // MINIGAME_H