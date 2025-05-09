#include "Frog.h"
#include <iostream>
#include <random>

void Frog::generateEnemyPosition() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib_x(13, 35);
    std::uniform_int_distribution<int> distrib_y(0,1);

    const auto X = static_cast<float>(distrib_x(gen)) * 40.f;
    auto Y = static_cast<float>(distrib_y(gen));
    sf::Vector2f current_scale = this->enemy_sprite.getScale();

    if (X < 960.f) {
        this->initial_frog_pos = "left";
        this->enemy_sprite.setScale(-current_scale.x, current_scale.y);
    }
    else {
        this->initial_frog_pos = "right";
    }
    // Y = 350.f top of arena, Y = 850.f bottom of arena
    if (Y == 0) {
        Y = 350.f;
        this->jump_height = -this->jump_height; // jumps downwards if on top of the arena
        current_scale = this->enemy_sprite.getScale();
        this->enemy_sprite.setScale(current_scale.x, -current_scale.y);
    }
    else {
        Y = 850.f;
    }

    this->enemy_sprite.setPosition(X, Y);
}

Frog::Frog() : Enemy(300.f, sf::Vector2f(100.f, 85.f)){
    this->enemy_texture = std::make_shared<sf::Texture>();
    if (!this->enemy_texture->loadFromFile("textures/frog_texture.png")) {
        std::cerr << "Failed to load frog texture";
    }
    this->enemy_sprite.setTexture(*this->enemy_texture);
}

void Frog::moveEnemy(const float deltaTime) {
    time_since_last_jump += deltaTime;
    std::cout << deltaTime << std::endl;
    if (!this->is_jumping && time_since_last_jump >= jump_cooldown) {
        this->start_pos = this->enemy_sprite.getPosition();
        if (this->initial_frog_pos == "left") {
            this->end_pos = sf::Vector2f(start_pos.x + jump_distance, start_pos.y);
        }
        else {
            this->end_pos = sf::Vector2f(start_pos.x - jump_distance, start_pos.y);
        }
        this->jump_timer = 0.f;
        this->is_jumping = true;
        this->time_since_last_jump = 0.f;
    }

    if (is_jumping) {
        jump_timer += deltaTime;
        float t = jump_timer / jump_duration;
        if (t >= 1.f) {
            t = 1.f;
            is_jumping = false;
        }

        const float x = start_pos.x + (end_pos.x - start_pos.x) * t;
        const float y =start_pos.y + (end_pos.y - start_pos.y) * t - (4 * jump_height * t * (1-t));
        this->enemy_sprite.setPosition(x, y);
    }
}


Enemy * Frog::clone() const {
    return new Frog(*this);
}

