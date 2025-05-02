#ifndef MINIGAMEPLAYER_H
#define MINIGAMEPLAYER_H

#include <SFML/Graphics.hpp>

class MinigamePlayer {
private:
    sf::Texture minigame_player_texture;
    sf::Sprite minigame_player_sprite;
    float minigame_player_speed;

    void initMinigamePlayerTexture();
    void initMinigamePlayerSprite();
    void moveMinigamePlayer(const sf::Vector2f& direction, float deltaTime);
    sf::Vector2f normalize(const sf::Vector2f& direction);

public:
    explicit MinigamePlayer();
    void updateMinigamePlayer(const sf::Vector2f& direction, float deltaTime);
    void draw(sf::RenderWindow& target) const;
    void checkMinigamePlayerOutOfBounds();
};


#endif //MINIGAMEPLAYER_H
