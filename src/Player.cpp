#include "Player.h"
#include <iostream>
#include <cmath>

#include "GameExceptions.h"

void Player::initPlayer() {
    this->player_sprite.setTexture(*this->player_texture);
    this->player_sprite.setOrigin(player_sprite.getGlobalBounds().width / 2.f, 0.f);
    this->current_player_position.x = 500.f;
    this->current_player_position.y = 500.f;
    this->player_sprite.setPosition(this->current_player_position.x, this->current_player_position.y);
    sf::Vector2u const original_player_texture_size = player_texture->getSize();
    float const scaleX = 100 / static_cast<float>(original_player_texture_size.x);
    float const scaleY = 300 / static_cast<float>(original_player_texture_size.y);
    player_sprite.setScale(scaleX, scaleY);
}

void Player::initPlayerTexture() {
    this->player_texture = std::make_shared<sf::Texture>();
    if (!player_texture->loadFromFile("textures/player_texture.png")) {
        throw ResourceLoadException{"Failed to load player texture!"};
    }
}

void Player::movePlayer(sf::Vector2f const normalized_direction, const float deltaTime) {
    sf::Vector2f const movement = normalized_direction * deltaTime * player_movement_speed;
    player_sprite.move(movement);
}

void Player::updateBreath() {
    this->breath -= 0.05f; //   3 / sec

    // checks if player should be alive
    if (this->breath < 0) {
        this->alive = false;
    }

    if (this->isPlayerMoving()) {
        this->breath -= static_cast<float>(this->strokes_counter) * 0.0833333f; // 5 / sec
    }
}

void Player::rotatePlayer(sf::Vector2f const normalized_direction) {
    float angle_deg = std::atan2(normalized_direction.y, normalized_direction.x) * 180.f / 3.141592653589f;
    angle_deg += 90.f;

    if (angle_deg < 0) {
        angle_deg += 360.f;
    }

    constexpr int num_directions = 32;
    constexpr float angle_step = 360.f / num_directions;
    float new_angle = std::round(angle_deg / angle_step) * angle_step;
    if (new_angle >= 360.f) {
        new_angle -= 360;
    }
    this->player_sprite.setRotation(new_angle);
}

void Player::checkOutOfBounds() {
    //Handles the out-of-bounds issue

    const sf::Vector2f playerPos = this->player_sprite.getPosition();
    const float clampedX = std::clamp(playerPos.x, 25.f, 1920.f);
    const float clampedY = std::clamp(playerPos.y, 25.f, 1080.f);
    this->player_sprite.setPosition(clampedX, clampedY);
}

bool Player::isPlayerMoving() const {
    return this->player_movement_speed > 50.f;
}

Player::Player(const float player_movement_speed_) : player_movement_speed(player_movement_speed_), breath(100.f) {
    this->initPlayerTexture();
    this->initPlayer();
}

float Player::getBreath() const {
    return this->breath;
}

bool Player::isCollidingWithPoint(const sf::Vector2f point_pos) const {
    return this->player_sprite.getGlobalBounds().contains(point_pos);
}

void Player::draw(sf::RenderTarget& target) const {
    target.draw(this->player_sprite);
}

void Player::doBreath() {
    this->next_stroke_is_breath = true;
}

void Player::updatePlayer(const sf::Vector2f location, const float deltaTime) {
    // location = mouse position

    this->updateBreath();
    if (strokeTimer.getElapsedTime().asSeconds() > 2.f && this->isPlayerMoving()) {
        std::cout << "Two seconds have passed since the last stroke" << "\n";
        // this->strokes_counter = 0;
        this->player_movement_speed = 50.f;
    }

    const sf::Vector2f current_position = this->player_sprite.getPosition();
    const sf::Vector2f direction = location - current_position;
    const float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 10.0f) {
        const sf::Vector2f normalized_direction = direction / distance;
        // Moves the player towards the mouse cursor.
        this->movePlayer(normalized_direction, deltaTime);
        // Rotates the player sprite to "look" towards the mouse cursor.
        this->rotatePlayer(normalized_direction);
    }
    this->checkOutOfBounds();
}

bool Player::isAlive() const {
    return this->alive;
}

void Player::doStroke(const sf::Keyboard::Key key) {
    if (this->strokeTimer.getElapsedTime().asSeconds() >= 1.0f) {       // IF THE STROKE IS "VALID" (NOT SPAMMED)
        this->strokeTimer.restart();

        if (key == sf::Keyboard::Q) {
            this->current_stroke = "left";
        }
        else {
            this->current_stroke = "right";
        }

        if (this->next_stroke_is_breath) {
            this->breath += 35.f;
            next_stroke_is_breath = false;
            this->player_movement_speed -= 40.f;
            this->strokes_counter = 0;
        }

        this->strokes_counter++;
        if (!this->isPlayerMoving()) {           // IF THE PLAYER STOPPED MOVING BY NOT STROKING IN 2 SECONDS
            this->player_movement_speed = 150.f;    // THE SPEED RESETS TO NORMAL
        }
        else {
            if (this->current_stroke == this->previous_stroke) {
                // SPEED SHOULDN'T CHANGE, FATIGUE INCREASES
                this->fatigue_counter++;
            }
            else {
                // SPEED GROWS, SO DOES THE RATE AT WHICH THE BREATH GOES DOWN
                this->fatigue_counter = 0;
                this->player_movement_speed += 20.f;
            }
        }
        this->previous_stroke = this->current_stroke;
    }
}

Player & Player::operator=(const Player &other) {
    this->player_texture = other.player_texture;
    this->player_sprite = other.player_sprite;
    this->current_player_position = other.current_player_position;
    this->strokeTimer = other.strokeTimer;
    this->previous_stroke = other.previous_stroke;
    this->current_stroke = other.current_stroke;
    this->strokes_counter = other.strokes_counter;
    this->fatigue_counter = other.fatigue_counter;
    this->alive = other.alive;
    this->next_stroke_is_breath = other.next_stroke_is_breath;
    this->player_movement_speed = other.player_movement_speed;
    this->breath = other.breath;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    std::cout << "Player started at location " << player.current_player_position.x << "," << player.current_player_position.y
              << ", has speed: " << player.player_movement_speed << std::endl;
    return os;
}
