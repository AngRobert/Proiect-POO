#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp> // For sf::Texture, sf::Sprite
#include <memory>

class Enemy {
private:
    virtual void generateEnemyPosition() = 0;
    virtual void moveEnemy(float deltaTime) = 0;

protected:
    std::shared_ptr<sf::Texture> enemy_texture;
    sf::Sprite enemy_sprite;
    float enemy_speed;
    sf::Vector2f enemy_size;

public:
    explicit Enemy(float speed_ = 0.f, sf::Vector2f size_ = sf::Vector2f(0, 0));
    Enemy(const Enemy& other_enemy);
    virtual ~Enemy();
    virtual Enemy* clone() const = 0;
    [[nodiscard]] sf::FloatRect getEnemyBounds() const;

    void spawn();
    void setEnemySize(sf::Vector2f desiredSize);
    void updateEnemy(float deltaTime);

    Enemy& operator=(const Enemy& other_enemy);
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

    void draw(sf::RenderTarget& target) const;
};

#endif // ENEMY_H
