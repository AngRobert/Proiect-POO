#include "Minigame.h"
#include <iostream>

Minigame::Minigame(int max_enemies_, int minigame_difficulty_, const std::vector<Enemy>& enemies_)
    : max_enemies(max_enemies_), minigame_difficulty(minigame_difficulty_), enemies(enemies_) {
    initMinigameFont();
    initMinigameText();
    initMinigameArena();
}

void Minigame::initMinigameText() {
    this->minigame_text.setFont(minigame_font);
    this->minigame_text.setCharacterSize(60);
    this->minigame_text.setString("DODGE!");
    this->minigame_text.setFillColor(sf::Color::White);
    this->minigame_text.setOrigin(this->minigame_text.getGlobalBounds().width / 2,
                                    this->minigame_text.getGlobalBounds().height / 2);
    this->minigame_text.setPosition(sf::Vector2f(960.f, 100.f));
}

void Minigame::initMinigameFont() {
    if (!this->minigame_font.loadFromFile("textures/fonts/papyrus.ttf")) {
        std::cerr << "Failed to load minigame font" << std::endl;
    }
}

void Minigame::initMinigameArena() {
    this->minigame_arena.setOutlineColor(sf::Color::White);
    this->minigame_arena.setOutlineThickness(5);
    this->minigame_arena.setFillColor(sf::Color::Black);
    this->minigame_arena.setSize(sf::Vector2f(1000.f, 600.f));
    this->minigame_arena.setOrigin(this->minigame_arena.getSize().x / 2,
                                    this->minigame_arena.getSize().y / 2);
    this->minigame_arena.setPosition(sf::Vector2f(960.f, 600.f));
}

void Minigame::renderEnemies(sf::RenderTarget& target) const {
    for (const auto& enemy : enemies) {
        enemy.draw(target);
    }
}

float Minigame::updateMinigameDeltaTime() {
    return this->minigame_delta_time.restart().asSeconds();
}

void Minigame::handleMovementInput(const float deltaTime) {
    sf::Vector2f direction(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x += 1;
    }

    this->minigame_player.updateMinigamePlayer(direction, deltaTime);
}

bool Minigame::isMinigameRunning() const {
    return this->minigame_active;
}

void Minigame::updateMinigame() {
    // this->updateMinigameDeltaTime();
    this->minigame_active = true;
    this->handleMovementInput(this->updateMinigameDeltaTime());
    // this->minigame_player.checkMinigamePlayerOutOfBounds(this->minigame_arena.getPosition());
}

void Minigame::renderMinigame(sf::RenderWindow& target) const {
    target.clear(sf::Color::Black);
    target.draw(minigame_text);
    target.draw(minigame_arena);
    this->minigame_player.draw(target);
    this->renderEnemies(target);
    target.display();
}

void Minigame::pollMinigameEvents(sf::RenderWindow& window, const sf::Event& event, sf::Clock& minigame_timer) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    window.close();
                break;
                case sf::Keyboard::Space:
                    minigame_active = false;
                minigame_timer.restart();
                break;
                default:
                    break;
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
