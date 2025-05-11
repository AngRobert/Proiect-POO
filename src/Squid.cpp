#include "Squid.h"
#include <random>

#include "GameExceptions.h"

void Squid::generateEnemyPosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_x(25, 70);
    std::uniform_int_distribution<> dist_y(0, 1);

    const auto X = static_cast<float>(dist_x(gen)) * 20.f;
    auto Y = dist_y(gen);

    if (X < 499.f || X > 1400.f) {
        throw InvalidEnemyPosition{"Enemy will spawn outside of the arena or will clip into the walls!"};
    }

    // 220 = top of the arena, 975 = bottom of arena
    if (Y == 0) {
       this->enemy_pos = "top";
        Y = 220;
    }
    else {
        this->enemy_pos = "bottom";
        this->enemy_sprite.setScale(this->enemy_sprite.getScale().x, -this->enemy_sprite.getScale().y);
        Y = 975;
    }

    this->enemy_sprite.setPosition(X, static_cast<float>(Y));

}

void Squid::moveEnemy(float deltaTime) {
    if (this->enemy_pos == "bottom") {
        this->enemy_sprite.move(0.f, -this->enemy_speed * deltaTime);
    }
    else {
        this->enemy_sprite.move(0.f, this->enemy_speed * deltaTime);
    }

}

void Squid::print(std::ostream &os) const {
    os << "Squid started at position: ";
}

Squid::Squid() : Enemy(300.f, sf::Vector2f(54.f, 145.f)) {
    this->enemy_texture = std::make_shared<sf::Texture>();
    if (!this->enemy_texture->loadFromFile("textures/squid_texture.png")) {
        throw ResourceLoadException{"Failed to load squid texture"};
    }
    this->enemy_sprite.setTexture(*this->enemy_texture);
}

Enemy * Squid::clone() const {
    return new Squid(*this);
}
