#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

#include "GameOverScreen.h"
#include "MenuScreen.h"
#include "Player.h"
#include "RhythmCircle.h"
#include "Minigame.h"
#include "Point.h"

class Game {
private:
    RhythmCircle rhythm_circle;
    std::vector<Point> points;
    sf::Font font;
    sf::Text breath_text;
    sf::Text breath_value;
    sf::Text score_text;
    sf::Text score_value;
    sf::RenderWindow* window;
    sf::Event event;
    sf::Clock clock;
    sf::Clock minigame_timer_clock;
    sf::VideoMode video_mode;
    sf::Vector2f mouse_position_window;
    Player player;
    Minigame current_minigame;
    MenuScreen menu_screen;
    GameOverScreen game_over_screen;
    int minigame_timer;
    int points_counter;
    int max_points;

    void initFont();
    void initText(sf::Text& text, const std::string& text_string, float text_pos_x, float text_pos_y) const;
    void initWindow();
    void initPoints();
    void updateMousePosition();
    void updateText();
    void updatePoints();
    void renderCircle() const;
    void renderText() const;
    void renderPlayer() const;
    void renderPoints() const;
    void renderGame() const;
    void pollGameEvents();

public:
    Game();
    ~Game();
    [[nodiscard]] bool isRunning() const;
    [[nodiscard]] bool minigameTime() const;
    float updateDeltaTime();
    void update();
    void render() const;
    void pollEvents();
    void generateMinigameTimer();
};

#endif // GAME_H