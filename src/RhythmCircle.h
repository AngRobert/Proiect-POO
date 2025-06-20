#ifndef RHYTHM_CIRCLE_H
#define RHYTHM_CIRCLE_H

#include <SFML/Graphics.hpp> // For sf::Texture, sf::Sprite, sf::CircleShape
#include <string>             // For std::string

//clasa Singleton

class RhythmCircle {
private:
    sf::Texture circle_texture;
    sf::Sprite circle_sprite;
    sf::CircleShape outer_circle{150.f};
    sf::Clock circle_timer;

    void resetCircle();
    explicit RhythmCircle(const std::string& circle_texture_filename = "", sf::Vector2f circle_pos = sf::Vector2f(0, 0));

public:

    RhythmCircle(const RhythmCircle& other) = delete;
    RhythmCircle& operator=(const RhythmCircle& other) = delete;

    static RhythmCircle& getInstance(const std::string& circle_texture_filename = "", sf::Vector2f circle_pos = sf::Vector2f(0, 0)) {
        static RhythmCircle rhythm_circle{circle_texture_filename, circle_pos};
        return rhythm_circle;
    }

    void draw(sf::RenderTarget& target) const;
    void updateCircle();
};
#endif // RHYTHM_CIRCLE_H
