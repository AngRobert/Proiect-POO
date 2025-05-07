#include "Enemy.h"
#include <SFML/Graphics.hpp> // For sf::RenderTarget
#include <random>
#include <iostream>

void Enemy::setEnemySize(const sf::Vector2f desiredSize) {
    sf::Vector2u const original_enemy_texture_size = enemy_texture->getSize();
    float const scaleX = desiredSize.x / static_cast<float>(original_enemy_texture_size.x);
    float const scaleY = desiredSize.y / static_cast<float>(original_enemy_texture_size.y);
    enemy_sprite.setScale(scaleX, scaleY);
}

Enemy::Enemy(const float speed_, const sf::Vector2f size_) :
    enemy_speed(speed_), enemy_size(size_) {}

Enemy::Enemy(const Enemy& other_enemy) = default;

Enemy::~Enemy() = default;

sf::FloatRect Enemy::getEnemyBounds() const {
    return this->enemy_sprite.getGlobalBounds();
}

void Enemy::spawn() {
    this->setEnemySize(this->enemy_size);
    this->enemy_sprite.setOrigin(this->enemy_sprite.getLocalBounds().width / 2,
                                this->enemy_sprite.getLocalBounds().height / 2);
    this->generateEnemyPosition();
}

void Enemy::updateEnemy() {
    this->moveEnemy();
}

Enemy& Enemy::operator=(const Enemy& other_enemy) {
    if (this == &other_enemy) {
        return *this;
    }
    enemy_speed = other_enemy.enemy_speed;
    enemy_size = other_enemy.enemy_size;
    enemy_sprite = other_enemy.enemy_sprite;
    enemy_texture = other_enemy.enemy_texture;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    os << "has " << enemy.enemy_speed << " speed, " << enemy.enemy_size.x << " " << enemy.enemy_size.y << " size, "
       << " difficulty\n";
    return os;
}

void Enemy::draw(sf::RenderTarget& target) const {
    target.draw(this->enemy_sprite);
}
