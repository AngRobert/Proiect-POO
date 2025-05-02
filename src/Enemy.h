#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp> // For sf::Texture, sf::Sprite
#include <string>             // For std::string
#include <memory>

class Enemy {
private:
    std::shared_ptr<sf::Texture> enemy_texture;
    sf::Sprite enemy_sprite;
    int enemy_difficulty;
    int enemy_speed;
    int enemy_size;
    std::string enemy_type;

    void generateEnemyPosition();
    void setEnemySize(float desiredWidth, float desiredHeight);
    void initEnemy();

public:
    explicit Enemy(int difficulty_ = 1, int speed_ = 0, int size_ = 1, std::string enemy_type_ = "null");
    Enemy(const Enemy& other_enemy);
    ~Enemy();

    Enemy& operator=(const Enemy& other_enemy);
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

    void draw(sf::RenderTarget& target) const;
};

#endif // ENEMY_H
