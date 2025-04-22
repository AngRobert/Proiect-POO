#ifndef RHYTHM_CIRCLE_H
#define RHYTHM_CIRCLE_H

#include <SFML/Graphics.hpp> // For sf::Texture, sf::Sprite, sf::CircleShape
#include <string>             // For std::string

class RhythmCircle {
private:
    sf::Texture circle_texture;
    sf::Sprite circle_sprite;
    sf::CircleShape outer_circle{150.f};
    sf::Clock circle_timer;

    void resetCircle(); // Declared here, defined in RhythmCircle.cpp

public:
    explicit RhythmCircle(const std::string& circle_texture_filename = "", sf::Vector2f circle_pos = sf::Vector2f(0, 0));
    void draw(sf::RenderTarget& target) const; // Draw the circle
    void updateCircle(); // Update circle behavior
};

#endif // RHYTHM_CIRCLE_H
