#ifndef MINIGAMEPLAYER_H
#define MINIGAMEPLAYER_H

#include <SFML/Graphics.hpp>
#include <memory>

class MinigamePlayer {
private:
    std::shared_ptr<sf::Texture> minigame_player_texture;
    sf::Sprite minigame_player_sprite;
    float minigame_player_speed;

    void initMinigamePlayerTexture();
    void initMinigamePlayerSprite();
    void moveMinigamePlayer(const sf::Vector2f& direction, float deltaTime);

    static sf::Vector2f normalize(const sf::Vector2f& direction);

public:
    explicit MinigamePlayer();
    [[nodiscard]] bool isPlayerHit(sf::FloatRect enemy_bounds) const;
    void reset();
    void updateMinigamePlayer(const sf::Vector2f& direction, float deltaTime);
    void draw(sf::RenderWindow& target) const;
    void checkMinigamePlayerOutOfBounds();
};


#endif //MINIGAMEPLAYER_H
