#include "RhythmCircle.h"
#include <SFML/Graphics.hpp> // For sf::RenderTarget

#include "GameExceptions.h"

void RhythmCircle::resetCircle() {
    // Resets the circle timer and its scale
    this->circle_timer.restart();
    this->outer_circle.setScale(0.8f, 0.8f);
}

RhythmCircle::RhythmCircle(const std::string& circle_texture_filename, const sf::Vector2f circle_pos) {
    if (!this->circle_texture.loadFromFile(circle_texture_filename)) {
        throw ResourceLoadException{"Failed to load rhythm circle texture!"};
    }
    this->circle_sprite.setTexture(circle_texture);
    this->circle_sprite.setOrigin(this->circle_sprite.getGlobalBounds().width / 2.f,
                                  this->circle_sprite.getGlobalBounds().height / 2.f);
    this->outer_circle.setOrigin(this->outer_circle.getGlobalBounds().width / 2.f,
                                  this->outer_circle.getGlobalBounds().height / 2.f);
    this->circle_sprite.setPosition(circle_pos);
    this->circle_sprite.setScale(1.f, 1.f);
    this->outer_circle.setOutlineColor(sf::Color(64, 255, 0, 255));
    this->outer_circle.setOutlineThickness(8.f);
    this->outer_circle.setFillColor(sf::Color(3, 177, 252, 0));
    this->outer_circle.setPosition(circle_pos);
}

void RhythmCircle::draw(sf::RenderTarget& target) const {
    target.draw(outer_circle); // Draw the outer circle
    target.draw(circle_sprite); // Draw the sprite over the circle
}

void RhythmCircle::updateCircle() {
    if (this->circle_timer.getElapsedTime().asSeconds() > 1.2f) {
        this->resetCircle(); // Reset the circle if the timer exceeds 1.2 seconds
    }
    this->outer_circle.setScale(this->outer_circle.getScale().x * 0.988012f, this->outer_circle.getScale().y * 0.988012f);
}
