#include "MinigamePlayer.h"

#include <iostream>
#include <cmath>

MinigamePlayer::MinigamePlayer() : minigame_player_speed(250.f) {
    this->initMinigamePlayerTexture();
    this->initMinigamePlayerSprite();
}

bool MinigamePlayer::isPlayerHit(const sf::FloatRect enemy_bounds) const {
    return this->minigame_player_sprite.getGlobalBounds().intersects(enemy_bounds);
}

void MinigamePlayer::reset() {
    this->minigame_player_sprite.setPosition(960.f, 700.f);
}

void MinigamePlayer::updateMinigamePlayer(const sf::Vector2f& direction, const float deltaTime) {
    this->moveMinigamePlayer(direction, deltaTime);
    this->checkMinigamePlayerOutOfBounds();
}

void MinigamePlayer::initMinigamePlayerTexture() {
    minigame_player_texture = std::make_shared<sf::Texture>();
    if (!this->minigame_player_texture->loadFromFile("textures/minigame_player_texture.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }
}

void MinigamePlayer::initMinigamePlayerSprite() {
    this->minigame_player_sprite.setTexture(*this->minigame_player_texture);
    this->minigame_player_sprite.setOrigin(this->minigame_player_sprite.getGlobalBounds().width / 2,
                                            this->minigame_player_sprite.getGlobalBounds().height / 2);
    this->minigame_player_sprite.setPosition(sf::Vector2f(960.f, 700.f));
    this->minigame_player_sprite.setScale(0.5f, 0.5f);
}

void MinigamePlayer::moveMinigamePlayer(const sf::Vector2f& direction, const float deltaTime) {
    if (direction.x != 0 || direction.y != 0) {
        const sf::Vector2f normalized_direction = this->normalize(direction);
        minigame_player_sprite.move(normalized_direction * minigame_player_speed * deltaTime);
    }
}

sf::Vector2f MinigamePlayer::normalize(const sf::Vector2f &direction) {
    if (const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y); length != 0) {
        return sf::Vector2f(direction.x / length, direction.y / length);
    }
    return sf::Vector2f(0.f, 0.f);
}

void MinigamePlayer::draw(sf::RenderWindow &target) const {
    target.draw(this->minigame_player_sprite);
}

void MinigamePlayer::checkMinigamePlayerOutOfBounds() {
    const sf::Vector2f minigame_player_pos = this->minigame_player_sprite.getPosition();
    const sf::FloatRect minigame_player_bounds = this->minigame_player_sprite.getGlobalBounds();

    const float clampedX = std::clamp(minigame_player_pos.x, 460.f + minigame_player_bounds.width / 2, 1460.f  - minigame_player_bounds.width / 2);
    const float clampedY = std::clamp(minigame_player_pos.y, 300.f  + minigame_player_bounds.height / 2, 900.f  - minigame_player_bounds.height / 2);

    this->minigame_player_sprite.setPosition(clampedX, clampedY);
}
