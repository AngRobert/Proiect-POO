#include "Pufferfish.h"

#include <iostream>

#include "GameExceptions.h"

void Pufferfish::generateEnemyPosition() {
    this->generateDefaultEnemyPosition();
    this->enemy_sprite.setScale(-this->enemy_sprite.getScale().x, this->enemy_sprite.getScale().y);
    this->initial_pufferfish_pos = this->enemy_sprite.getPosition();
}

Pufferfish::Pufferfish() : Enemy(300.f, sf::Vector2f(85.f, 85.f)) {
    this->enemy_texture = std::make_shared<sf::Texture>();
    if (!this->enemy_texture->loadFromFile("textures/pufferfish_texture.png")) {
        throw ResourceLoadException{"Failed to load pufferfish texture!"};
    }
    this->enemy_sprite.setTexture(*this->enemy_texture);
}

void Pufferfish::moveEnemy(const float deltaTime) {
    const sf::Vector2f current_pos = this->enemy_sprite.getPosition();
    if (this->shouldInflate(current_pos)) {
        this->isInflated = true;
        this->inflate_timer += deltaTime;

        if (this->inflate_timer < this->inflate_duration / 2.f) {
            this->inflate();
        }
        else if (this->inflate_timer < this->inflate_duration) {
            this->deflate();
        }
        else {
            this->isInflated = false;
            this->inflate_timer = 0.f;
            if (this->inflate_counter < 3) {
                this->inflate_counter += 1;
            }
        }
    }

    if (!this->isInflated) {
        if (this->enemy_pos == "left") {
            this->enemy_sprite.move(enemy_speed * deltaTime, 0.f);
        }
        else {
            this->enemy_sprite.move(-enemy_speed * deltaTime, 0.f);
        }
    }
}

void Pufferfish::inflate() {
    this->enemy_sprite.setScale(this->enemy_sprite.getScale().x * 1.013f, this->enemy_sprite.getScale().y * 1.013f);
}

void Pufferfish::deflate() {
    this->enemy_sprite.setScale(this->enemy_sprite.getScale().x * 0.987f, this->enemy_sprite.getScale().y * 0.987f);
}

bool Pufferfish::shouldInflate(const sf::Vector2f current_pos) const {
    return std::abs(current_pos.x - this->initial_pufferfish_pos.x) > 400.f * this->inflate_counter;
}

void Pufferfish::print(std::ostream &os) const {
    os << "Pufferfish started on the " << this->enemy_pos << " side of the arena";
}

Enemy * Pufferfish::clone() const {
    return new Pufferfish(*this);
}
