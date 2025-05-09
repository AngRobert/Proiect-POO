#include "Fish.h"
#include <memory>
#include <random>

void Fish::generateEnemyPosition() {
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distrib_fish_Y(this->fish_y_range.x, this->fish_y_range.y);
    std::uniform_int_distribution<int> fish_position(0, 1);


    auto X = static_cast<float>(fish_position(generator));
    const auto Y = static_cast<float>(distrib_fish_Y(generator)) * 10.f;

    // 375 = on the left side of the minigame arena, 1545 = right

    if (X == 0) {
        X = 375.f;
        this->fish_pos = "left";
    }
    else {
        X = 1545.f;
        this->fish_pos = "right";
        const sf::Vector2f current_scale = enemy_sprite.getScale();
        this->enemy_sprite.setScale(-current_scale.x, current_scale.y);
    }

    this->enemy_sprite.setPosition(X, Y);
}

void Fish::moveEnemy(float deltaTime) {
    if (this->fish_pos == "left") {
        this->enemy_sprite.move(enemy_speed, 0.f);
    }
    else {
        this->enemy_sprite.move(-enemy_speed, 0.f);
    }
}

Fish::Fish() : Enemy(7.f, sf::Vector2f(150.f, 75.f)), fish_y_range(sf::Vector2i(35, 85)), fish_pos("") {
    this->enemy_texture = std::make_shared<sf::Texture>();
    this->enemy_texture->loadFromFile("textures/fish_texture.png");
    this->enemy_sprite.setTexture(*this->enemy_texture);
}

Enemy * Fish::clone() const {
    return new Fish(*this);
}
