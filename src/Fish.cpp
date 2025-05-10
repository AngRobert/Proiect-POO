#include "Fish.h"
#include <memory>
#include <random>

void Fish::generateEnemyPosition() {
    this->generateDefaultEnemyPosition();
}

void Fish::print(std::ostream &os) const {
    os << "Fish started from the " << this->enemy_pos << " side of the arena";
}

void Fish::moveEnemy(const float deltaTime) {
    if (this->enemy_pos == "left") {
        this->enemy_sprite.move(enemy_speed * deltaTime, 0.f);
    }
    else {
        this->enemy_sprite.move(-enemy_speed * deltaTime, 0.f);
    }
}

Fish::Fish() : Enemy(350.f, sf::Vector2f(150.f, 75.f)) {
    this->enemy_texture = std::make_shared<sf::Texture>();
    this->enemy_texture->loadFromFile("textures/fish_texture.png");
    this->enemy_sprite.setTexture(*this->enemy_texture);
}

Enemy * Fish::clone() const {
    return new Fish(*this);
}
