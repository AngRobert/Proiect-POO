#ifndef MINIGAME_H
#define MINIGAME_H

#include <SFML/Graphics.hpp>
#include <vector>
// #include "Fish.h"
#include "Frog.h"
#include "MinigamePlayer.h"

class Minigame {
private:
    int max_enemies;
    int minigame_difficulty;
    int frog_counter = 0;
    float enemy_spawn_timer;
    sf::Clock enemy_spawn_clock;
    sf::RectangleShape minigame_arena;
    bool minigame_active = false;
    bool minigame_failed = false;
    bool minigame_success = false;
    bool hasException = false;
    std::vector<std::unique_ptr<Enemy>> enemies;
    MinigamePlayer minigame_player;
    sf::Clock minigame_delta_clock;
    float minigame_delta_time;
    std::shared_ptr<sf::Font> minigame_font;
    sf::Text minigame_text;
    sf::Text minigame_error_text;

    void initMinigameText();
    void initMinigameFont();
    void initMinigameArena();
    void updateEnemies(float deltaTime);
    void renderEnemies(sf::RenderTarget& target) const;
    float updateMinigameDeltaTime();
    void handleMovementInput(float deltaTime);
    void handleEnemySpawns();
    void handleMinigameEnd(sf::Clock& minigame_timer);
    bool hasEnemyInsideArena() const;

public:
    explicit Minigame(int max_enemies_ = 12, int minigame_difficulty_ = 1, float enemy_spawn_timer_ = 0.25f);
    Minigame(const Minigame& other);
    Minigame& operator=(Minigame other);
    ~Minigame() = default;

    [[nodiscard]] bool isMinigameRunning() const;
    [[nodiscard]] bool isMinigameFailed() const;
    void updateMinigame(sf::Clock& minigame_timer);
    void renderMinigame(sf::RenderWindow& target) const;
    void static pollMinigameEvents(sf::RenderWindow& window, const sf::Event& event);
    void generateEnemy();

    friend std::ostream& operator<<(std::ostream& os, const Minigame& minigame);
    friend void swap(Minigame& lhs, Minigame& rhs) noexcept;
};
#endif // MINIGAME_H