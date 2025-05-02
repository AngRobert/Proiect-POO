#include "Enemy.h"
#include <SFML/Graphics.hpp> // For sf::RenderTarget
#include <random>
#include <iostream>

void Enemy::generateEnemyPosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribWidth(50, 1870);
    std::uniform_int_distribution<> distribHeight(50, 1030);
    const auto X = static_cast<float>(distribWidth(gen));
    const auto Y = static_cast<float>(distribHeight(gen));
    this->enemy_sprite.setPosition(X, Y);
}

void Enemy::setEnemySize(float const desiredWidth, float const desiredHeight) {
    sf::Vector2u const original_enemy_texture_size = enemy_texture->getSize();
    float const scaleX = desiredWidth / static_cast<float>(original_enemy_texture_size.x);
    float const scaleY = desiredHeight / static_cast<float>(original_enemy_texture_size.y);
    enemy_sprite.setScale(scaleX, scaleY);
}

void Enemy::initEnemy() {
    this->enemy_texture = std::make_shared<sf::Texture>();
    this->enemy_texture->loadFromFile("textures/fish_texture.png");
    enemy_sprite.setTexture(*enemy_texture);
    this->generateEnemyPosition();
    setEnemySize(100.f, 50.f);
}

Enemy::Enemy(const int difficulty_, const int speed_, const int size_, std::string enemy_type_) :
    enemy_difficulty(difficulty_), enemy_speed(speed_), enemy_size(size_), enemy_type(std::move(enemy_type_)) {
    this->initEnemy();
}

Enemy::Enemy(const Enemy& other_enemy) :
    enemy_texture(other_enemy.enemy_texture),
    enemy_sprite(other_enemy.enemy_sprite),
    enemy_difficulty(other_enemy.enemy_difficulty),
    enemy_speed(other_enemy.enemy_speed),
    enemy_size(other_enemy.enemy_size) {}

Enemy::~Enemy() = default;

Enemy& Enemy::operator=(const Enemy& other_enemy) {
    if (this == &other_enemy) {
        return *this; // Handle self-assignment
    }
    enemy_difficulty = other_enemy.enemy_difficulty;
    enemy_speed = other_enemy.enemy_speed;
    enemy_size = other_enemy.enemy_size;
    enemy_sprite = other_enemy.enemy_sprite;
    enemy_texture = other_enemy.enemy_texture;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    os << "has " << enemy.enemy_speed << " speed, " << enemy.enemy_size << " size, "
       << enemy.enemy_difficulty << " difficulty\n";
    return os;
}

void Enemy::draw(sf::RenderTarget& target) const {
    target.draw(this->enemy_sprite);
}
