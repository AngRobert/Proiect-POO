#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <utility>

#include "GameExceptions.h"

void Enemy::setEnemySize(const sf::Vector2f desiredSize) {
    sf::Vector2u const original_enemy_texture_size = enemy_texture->getSize();
    float const scaleX = desiredSize.x / static_cast<float>(original_enemy_texture_size.x);
    float const scaleY = desiredSize.y / static_cast<float>(original_enemy_texture_size.y);
    enemy_sprite.setScale(scaleX, scaleY);
}

void Enemy::generateDefaultEnemyPosition() {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distrib_fish_Y(35, 85);
    std::uniform_int_distribution<int> fish_position(0, 1);

    auto X = static_cast<float>(fish_position(generator));
    const auto Y = static_cast<float>(distrib_fish_Y(generator)) * 10.f;

    if (Y < 349.f) {
        throw InvalidEnemyPosition{"Fish will spawn above the arena or will clip into the walls!"};
    }
    if (Y > 851.f) {
        throw InvalidEnemyPosition{"Fish will spawn below the arena or clip into it!"};
    }
    // 375 = on the left side of the minigame arena, 1545 = right

    if (X == 0) {
        X = 375.f;
        this->enemy_pos = "left";
    }
    else {
        X = 1545.f;
        this->enemy_pos = "right";
        const sf::Vector2f current_scale = enemy_sprite.getScale();
        this->enemy_sprite.setScale(-current_scale.x, current_scale.y);
    }

    this->enemy_sprite.setPosition(X, Y);
}

Enemy::Enemy(const float speed_, const sf::Vector2f size_, std::string enemy_pos_) :
    enemy_speed(speed_), enemy_size(size_), enemy_pos(std::move(enemy_pos_)) {}

Enemy::Enemy(const Enemy& other_enemy) = default;

Enemy::~Enemy() = default;

sf::FloatRect Enemy::getEnemyBounds() const {
    return this->enemy_sprite.getGlobalBounds();
}

void Enemy::spawn() {
    this->setEnemySize(this->enemy_size);
    this->enemy_sprite.setOrigin(this->enemy_sprite.getLocalBounds().width / 2,
                                this->enemy_sprite.getLocalBounds().height / 2);
    try {
        this->generateEnemyPosition();
    } catch (const InvalidEnemyPosition& e) {
        std::cout << e.what() << std::endl;
    }
}

void Enemy::updateEnemy(const float deltaTime) {
    this->moveEnemy(deltaTime);
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
    os << "Enemy has " << enemy.enemy_speed << " speed, " << enemy.enemy_size.x << " " << enemy.enemy_size.y << " size";
    enemy.print(os);
    os << "\n";
    return os;
}

void Enemy::draw(sf::RenderTarget& target) const {
    target.draw(this->enemy_sprite);
}
