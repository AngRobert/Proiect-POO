#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Player.h"

class GameOverScreen {
private:
    sf::Font game_over_font;
    sf::Text game_over_text;
    Button play_again_button{"Play again!", sf::Vector2f(960.f, 600.f), sf::Vector2f(500.f, 100.f), sf::Color(230, 115, 159, 255)};
    bool game_over_active;

    void initGameOverFont();
    void initGameOverText();

public:

    explicit GameOverScreen();
    void renderGameOver(sf::RenderWindow& target) const;
    void updateGameOver(sf::Vector2f mouse_position);
    void pollGameOverEvents(sf::RenderWindow &window, const sf::Event &event, sf::Vector2f mouse_pos, Player& player);
    [[nodiscard]] bool isGameOver() const;

};



#endif //GAMEOVERSCREEN_H
