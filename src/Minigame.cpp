#include "Minigame.h"
#include <iostream>

Minigame::Minigame(int max_enemies_, int minigame_difficulty_, const std::vector<Enemy>& enemies_)
    : max_enemies(max_enemies_), minigame_difficulty(minigame_difficulty_), enemies(enemies_) {
    initMinigameFont();
    initMinigameText();
}

void Minigame::initMinigameText() {
    this->minigame_placeholder_text.setPosition(sf::Vector2f(500.f, 500.f));
    this->minigame_placeholder_text.setFont(minigame_font);
    this->minigame_placeholder_text.setCharacterSize(30);
    this->minigame_placeholder_text.setString(
        "YOU ARE NOW IN A MINIGAME!!!!!!!!! \n"
        "this is a big WIP.. press space to exit \n"
        "or B to spawn an enemy at a random location. \n"
        "Have great fun!"
    );
    this->minigame_placeholder_text.setFillColor(sf::Color::White);
}

void Minigame::initMinigameFont() {
    if (!this->minigame_font.loadFromFile("textures/fonts/papyrus.ttf")) {
        std::cerr << "Failed to load minigame font" << std::endl;
    }
}

void Minigame::renderEnemies(sf::RenderTarget& target) const {
    for (const auto& enemy : enemies) {
        enemy.draw(target);
    }
}

bool Minigame::isMinigameRunning() const {
    return this->minigame_active;
}

void Minigame::updateMinigame() {
    this->minigame_active = true;
}

void Minigame::renderMinigame(sf::RenderWindow& target) const {
    target.clear(sf::Color::Black);
    target.draw(minigame_placeholder_text);
    renderEnemies(target);
    target.display();
}

void Minigame::pollMinigameEvents(sf::RenderWindow& window, const sf::Event& event, sf::Clock& minigame_timer) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::Escape == event.key.code) {
                window.close();
            }
            if (sf::Keyboard::Space == event.key.code) {
                minigame_active = false;
                minigame_timer.restart();
            }
            else if (sf::Keyboard::B == event.key.code) {
                enemies.emplace_back();
            }
            break;
        default:
            break;
    }
}

std::ostream& operator<<(std::ostream& os, const Minigame& minigame) {
    os << "Minigame of difficulty " << minigame.minigame_difficulty << " has "
       << minigame.max_enemies << " max enemies\n";
    for (std::size_t i = 0; i < minigame.enemies.size(); ++i) {
        os << "Enemy " << i << " " << minigame.enemies[i];
    }
    return os;
}
