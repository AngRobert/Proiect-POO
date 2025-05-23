#include "Minigame.h"
#include "Fish.h"
#include <iostream>
#include <random>
#include "GameExceptions.h"
#include "Pufferfish.h"
#include "Squid.h"


Minigame::Minigame(const int max_enemies_, const int minigame_difficulty_, const float enemy_spawn_timer_)
    : max_enemies(max_enemies_), minigame_difficulty(minigame_difficulty_), enemy_spawn_timer(enemy_spawn_timer_), minigame_delta_time(0.f) {
    this->initMinigameFont();
    this->initMinigameText();
    this->initMinigameArena();
}

Minigame::Minigame(const Minigame &other)
    : max_enemies(other.max_enemies),
      minigame_difficulty(other.minigame_difficulty),
      frog_counter(other.frog_counter),
      enemy_spawn_timer(other.enemy_spawn_timer),
      enemy_spawn_clock(other.enemy_spawn_clock),
      minigame_arena(other.minigame_arena),
      minigame_active(other.minigame_active),
      minigame_failed(other.minigame_failed),
      minigame_success(other.minigame_success),
      minigame_player(other.minigame_player),
      minigame_delta_clock(other.minigame_delta_clock),
      minigame_delta_time(other.minigame_delta_time),
      minigame_font(other.minigame_font),
      minigame_text(other.minigame_text)
{
    for (const auto& enemy : other.enemies) {
        enemies.push_back(std::unique_ptr<Enemy>(enemy->clone()));
    }
}


Minigame & Minigame::operator=(Minigame other) {
    swap(*this, other);
    return *this;
}

void Minigame::initMinigameText() {
    this->minigame_text.setFont(*minigame_font);
    this->minigame_text.setCharacterSize(60);
    this->minigame_text.setString("DODGE!");
    this->minigame_text.setFillColor(sf::Color::White);
    this->minigame_text.setOrigin(this->minigame_text.getGlobalBounds().width / 2,
                                    this->minigame_text.getGlobalBounds().height / 2);
    this->minigame_text.setPosition(sf::Vector2f(960.f, 100.f));

    this->minigame_error_text.setFont(*minigame_font);
    this->minigame_error_text.setCharacterSize(40);
    this->minigame_error_text.setString("PRESS WASD TO MOVE!");
    this->minigame_error_text.setOrigin(this->minigame_error_text.getGlobalBounds().width / 2,
                                    this->minigame_error_text.getGlobalBounds().height / 2);
    this->minigame_error_text.setPosition(sf::Vector2f(960.f, 150.f));
}

void Minigame::initMinigameFont() {
    this->minigame_font = std::make_shared<sf::Font>();
    if (!this->minigame_font->loadFromFile("textures/fonts/papyrus.ttf")) {
        throw ResourceLoadException{"Failed to load minigame font!"};
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

void Minigame::updateEnemies(const float deltaTime) {
    int frog_count = 0;
    for (const auto& enemy : enemies) {
        enemy->updateEnemy(deltaTime);
        if (this->minigame_player.isPlayerHit(enemy->getEnemyBounds())) {
            minigame_failed = true;
        }
        if (dynamic_cast<Frog*>(enemy.get()) != nullptr) {
            frog_count++;
        }
    }
    this->frog_counter = frog_count;
}

void Minigame::renderEnemies(sf::RenderTarget& target) const {
    sf::RenderTexture enemyLayer;
    enemyLayer.create(static_cast<unsigned int>(minigame_arena.getSize().x), static_cast<unsigned int>(minigame_arena.getSize().y));
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
    return this->minigame_delta_clock.restart().asSeconds();
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
    const auto maxEnemies = static_cast<std::size_t>(this->max_enemies);
    if (!this->hasEnemyInsideArena() && this->enemies.size() == maxEnemies) {
        this->minigame_success = true;
        if (this->minigame_difficulty < 3) {
            this->minigame_difficulty ++;
        }
    }
    if (this->enemy_spawn_clock.getElapsedTime().asSeconds() > this->enemy_spawn_timer && this->enemies.size() < maxEnemies) {
        this->enemy_spawn_clock.restart();
        try {
            this->generateEnemy();
        } catch (const InvalidMinigameDifficulty &e) {
            this->minigame_success = true;
            std::cout << e.what() << std::endl;
        }
    }
}

void Minigame::handleMinigameEnd(sf::Clock& minigame_timer) {
    if (minigame_success) {
        minigame_active = false;
        minigame_success = false;
        this->frog_counter = 0;
        this->enemies.clear();
        this->minigame_player.reset();
        minigame_timer.restart();
    }
}

bool Minigame::hasEnemyInsideArena() const {
    const sf::FloatRect arenaBounds = minigame_arena.getGlobalBounds();
    for (const auto& enemy : enemies) {
        if (arenaBounds.intersects(enemy->getEnemyBounds())) {
            return true;
        }
    }
    return false;
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
    minigame_delta_time = this->updateMinigameDeltaTime();
    try {
        this->handleMovementInput(minigame_delta_time);
        this->hasException = false;
    } catch (const InvalidControlSchemeException& e) {
        std::cout << e.what() << std::endl;
        this->hasException = true;
    }
    this->updateEnemies(minigame_delta_time);
    this->handleEnemySpawns();
    this->handleMinigameEnd(minigame_timer);
}

void Minigame::renderMinigame(sf::RenderWindow& target) const {
    target.clear(sf::Color::Black);
    if (!this->hasException) {
        target.draw(minigame_text);
    }
    else {
        target.draw(this->minigame_error_text);
    }
    target.draw(minigame_arena);
    this->renderEnemies(target);
    this->minigame_player.draw(target);
    target.display();
}

void Minigame::pollMinigameEvents(sf::RenderWindow& window, const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::Escape:
                    window.close();
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib_enemy(0, 3);
    const int enemy_type = distrib_enemy(gen);
    std::unique_ptr<Enemy> e;
    switch (minigame_difficulty) {
        case 1:
            this->max_enemies = 20;
            if (enemy_type == 0 || enemy_type == 1) {
                e = std::make_unique<Squid>();
            }
            else {
                e = std::make_unique<Fish>();
            }
            break;
        case 2:
            this->max_enemies = 18;
            if ((enemy_type == 1 || enemy_type == 2) && frog_counter < 4) {
                e = std::make_unique<Frog>();
            }
            else {
                e = std::make_unique<Fish>();
            }
            break;
        case 3:
            this->max_enemies = 16;
            if (enemy_type == 2) {
                e = std::make_unique<Pufferfish>();
            }
            else if (enemy_type == 1 && frog_counter < 4) {
                e = std::make_unique<Frog>();
            }
            else {
                e = std::make_unique<Fish>();
            }
            break;
        default:
            throw InvalidMinigameDifficulty{minigame_difficulty};
    }
    e->spawn();
    enemies.push_back(std::move(e));
}

std::ostream& operator<<(std::ostream& os, const Minigame& minigame) {
    os << "Minigame of difficulty " << minigame.minigame_difficulty << " has "
       << minigame.max_enemies << " max enemies\n";
    for (std::size_t i = 0; i < minigame.enemies.size(); ++i) {
        os << "Enemy " << i << " " << *minigame.enemies[i];
    }
    return os;
}

void swap(Minigame &lhs, Minigame &rhs) noexcept {
        using std::swap;
        swap(lhs.max_enemies, rhs.max_enemies);
        swap(lhs.minigame_difficulty, rhs.minigame_difficulty);
        swap(lhs.frog_counter, rhs.frog_counter);
        swap(lhs.enemy_spawn_timer, rhs.enemy_spawn_timer);
        swap(lhs.enemy_spawn_clock, rhs.enemy_spawn_clock);
        swap(lhs.minigame_arena, rhs.minigame_arena);
        swap(lhs.minigame_active, rhs.minigame_active);
        swap(lhs.minigame_failed, rhs.minigame_failed);
        swap(lhs.minigame_success, rhs.minigame_success);
        swap(lhs.enemies, rhs.enemies);
        swap(lhs.minigame_player, rhs.minigame_player);
        swap(lhs.minigame_delta_clock, rhs.minigame_delta_clock);
        swap(lhs.minigame_delta_time, rhs.minigame_delta_time);
        swap(lhs.minigame_font, rhs.minigame_font);
        swap(lhs.minigame_text, rhs.minigame_text);
    }