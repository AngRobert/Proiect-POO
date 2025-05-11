#include "GameOverScreen.h"
#include "GameExceptions.h"

void GameOverScreen::initGameOverFont() {
    if (!this->game_over_font.loadFromFile("textures/fonts/Comic_Sans_MS.ttf")) {
        throw ResourceLoadException{"Failed to load game over font !"};
    }
}

void GameOverScreen::initGameOverText() {
    this->game_over_text.setFont(this->game_over_font);
    this->game_over_text.setString("Game Over!");
    this->game_over_text.setCharacterSize(60);
    this->game_over_text.setFillColor(sf::Color::White);
    this->game_over_text.setOrigin(this->game_over_text.getGlobalBounds().width / 2,
                                    this->game_over_text.getGlobalBounds().height / 2);
    this->game_over_text.setPosition(960.f, 450.f);
}

GameOverScreen::GameOverScreen() : game_over_active(false) {
    this->initGameOverFont();
    this->initGameOverText();
}

void GameOverScreen::renderGameOver(sf::RenderWindow &target) const {
    target.clear(sf::Color::Red);
    target.draw(this->game_over_text);
    play_again_button.renderButton(target);
    target.display();
}

void GameOverScreen::updateGameOver(const sf::Vector2f mouse_position) {
    this->game_over_active = true;
    this->play_again_button.updateButton(mouse_position);
}

void GameOverScreen::pollGameOverEvents(sf::RenderWindow &window, const sf::Event &event, const sf::Vector2f mouse_pos, Player& player, Minigame& minigame) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            break;
        case sf::Event::MouseButtonPressed:
            if (play_again_button.isHovered(mouse_pos) && event.mouseButton.button == sf::Mouse::Left) {
                const Player temp_player;
                player = temp_player;

                const Minigame temp_minigame;
                minigame = temp_minigame;

                this->game_over_active = false;
            }
        default:
            break;
    }
}

bool GameOverScreen::isGameOver() const {
    return this->game_over_active;
}



