#include "Minigame.h"
#include <iostream>

Minigame::Minigame(const int max_enemies_, const int minigame_difficulty_, const float enemy_spawn_timer_)
    : max_enemies(max_enemies_), minigame_difficulty(minigame_difficulty_), enemy_spawn_timer(enemy_spawn_timer_) {
    this->initMinigameFont();
    this->initMinigameText();
    this->initMinigameArena();
}

Minigame::Minigame(const Minigame &other) :
    max_enemies(other.max_enemies),
    minigame_difficulty(other.minigame_difficulty),
    enemy_spawn_timer(other.enemy_spawn_timer),
    enemy_spawn_clock(other.enemy_spawn_clock),
    minigame_arena(other.minigame_arena),
    minigame_active(other.minigame_active),
    minigame_failed(other.minigame_failed),
    minigame_player(other.minigame_player),
    minigame_delta_time(other.minigame_delta_time),
    minigame_font(other.minigame_font),
    minigame_text(other.minigame_text)
{
    for (const auto& enemy : other.enemies) {
        enemies.push_back(std::unique_ptr<Enemy>(enemy->clone()));
    }
}

Minigame & Minigame::operator=(const Minigame &other) {
    if (this != &other) {
        max_enemies = other.max_enemies;
        minigame_difficulty = other.minigame_difficulty;
        minigame_arena = other.minigame_arena;
        minigame_active = other.minigame_active;
        minigame_failed = other.minigame_failed;
        minigame_player = other.minigame_player;

        if (!minigame_font.loadFromFile("textures/fonts/papyrus.ttf")) {
            std::cerr << "Failed to load minigame font" << std::endl;
        }

        minigame_text = other.minigame_text;
        minigame_text.setFont(minigame_font);
        enemies.clear();
        for (const auto& enemy : other.enemies) {
            enemies.push_back(std::unique_ptr<Enemy>(enemy->clone()));
        }
    }
    return *this;
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

void Minigame::updateEnemies() {
    for (const auto& enemy : enemies) {
        enemy->updateEnemy();
        if (this->minigame_player.isPlayerHit(enemy->getEnemyBounds())) {
            minigame_failed = true;
        }
    }
}

void Minigame::renderEnemies(sf::RenderTarget& target) const {
    sf::RenderTexture enemyLayer;
    enemyLayer.create(minigame_arena.getSize().x, minigame_arena.getSize().y);
    enemyLayer.clear(sf::Color::Transparent);

    const sf::View enemyView(sf::FloatRect(
        minigame_arena.getPosition().x - minigame_arena.getSize().x / 2,
        minigame_arena.getPosition().y - minigame_arena.getSize().y / 2,
        minigame_arena.getSize().x,
        minigame_arena.getSize().y));
    enemyLayer.setView(enemyView);

    for (const auto& enemy : enemies) {
        enemy->draw(enemyLayer);
    }

    enemyLayer.display();

    sf::Sprite enemySprite(enemyLayer.getTexture());
    enemySprite.setPosition(minigame_arena.getPosition().x - minigame_arena.getSize().x / 2,
                            minigame_arena.getPosition().y - minigame_arena.getSize().y / 2);
    target.draw(enemySprite);
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

void Minigame::handleEnemySpawns() {
    if (minigame_end_timeout_clock.getElapsedTime().asSeconds() > 5.f && this->enemies.size() == this->max_enemies) {
        this->minigame_success = true;
    }
    if (this->enemy_spawn_clock.getElapsedTime().asSeconds() > this->enemy_spawn_timer && this->enemies.size() < this->max_enemies - 1) {
        this->enemy_spawn_clock.restart();
        this->generateEnemy();
    }
    else if (this->enemies.size() == this->max_enemies - 1) {
        minigame_end_timeout_clock.restart();
        this->generateEnemy();
    }
}

void Minigame::handleMinigameEnd(sf::Clock& minigame_timer) {
    if (minigame_success) {
        minigame_active = false;
        minigame_success = false;
        this->enemies.clear();
        this->minigame_player.reset();
        minigame_timer.restart();
    }
}

bool Minigame::isMinigameRunning() const {
    return this->minigame_active;
}

bool Minigame::isMinigameFailed() const {
    return this->minigame_failed;
}

void Minigame::updateMinigame(sf::Clock& minigame_timer) {
    this->minigame_active = true;
    this->minigame_failed = false;
    this->updateEnemies();
    this->handleMovementInput(this->updateMinigameDeltaTime());
    this->handleEnemySpawns();
    this->handleMinigameEnd(minigame_timer);
}

void Minigame::renderMinigame(sf::RenderWindow& target) const {
    target.clear(sf::Color::Black);
    target.draw(minigame_text);
    target.draw(minigame_arena);
    this->renderEnemies(target);
    this->minigame_player.draw(target);
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
                    this->enemies.clear();
                    this->minigame_player.reset();
                    minigame_timer.restart();
                    break;
                case sf::Keyboard::B:
                    this->generateEnemy();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void Minigame::generateEnemy() {
    auto enemy = std::make_unique<Fish>();
    enemy->spawn();
    enemies.push_back(std::move(enemy));
}

std::ostream& operator<<(std::ostream& os, const Minigame& minigame) {
    os << "Minigame of difficulty " << minigame.minigame_difficulty << " has "
       << minigame.max_enemies << " max enemies\n";
    for (std::size_t i = 0; i < minigame.enemies.size(); ++i) {
        os << "Enemy " << i << " " << minigame.enemies[i];
    }
    return os;
}
