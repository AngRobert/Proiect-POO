#ifndef PUFFERFISH_H
#define PUFFERFISH_H

#include "Enemy.h"

class Pufferfish : public Enemy {
private:
    int inflate_counter = 1;
    bool isInflated = false;
    float inflate_timer = 0.f;
    float inflate_duration = 3.f;
    sf::Vector2f initial_pufferfish_pos;

    void generateEnemyPosition() override;
    void moveEnemy(float deltaTime) override;
    void inflate();
    void deflate();

    [[nodiscard]] bool shouldInflate(sf::Vector2f current_pos) const;


public:
    explicit Pufferfish();
    Enemy * clone() const override;

};

#endif //PUFFERFISH_H
