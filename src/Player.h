#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class Player {
private:
    std::shared_ptr<sf::Texture> player_texture;
    sf::Sprite player_sprite;
    sf::Vector2f current_player_position;
    sf::Clock strokeTimer{};
    std::string previous_stroke;
    std::string current_stroke;
    int strokes_counter = 0;
    int fatigue_counter = 0;
    bool alive = true;
    bool next_stroke_is_breath = false;
    float player_movement_speed;
    float breath;

    void initPlayer();
    void initPlayerTexture();
    void movePlayer(sf::Vector2f normalized_direction, float deltaTime);
    void updateBreath();
    void rotatePlayer(sf::Vector2f normalized_direction);
    void checkOutOfBounds();
    bool isPlayerMoving() const;

public:
    explicit Player(float player_movement_speed_ = 0.f);
    [[nodiscard]] float getBreath() const;
    [[nodiscard]] bool isCollidingWithPoint(sf::Vector2f point_pos) const;
    void draw(sf::RenderTarget& target) const;
    void doBreath();
    void updatePlayer(sf::Vector2f location, float deltaTime);
    bool isAlive() const;
    void doStroke(sf::Keyboard::Key key);
    Player& operator=(const Player& other);

    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};


#endif //PLAYER_H
