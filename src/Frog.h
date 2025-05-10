#ifndef FROG_H
#define FROG_H

#include "Enemy.h"

class Frog : public Enemy {
private:
    sf::Vector2f frog_x_range;
    sf::Vector2f start_pos;
    sf::Vector2f end_pos;
    float jump_duration = 2.f;
    float jump_timer = 0.f;
    bool is_jumping = false;
    float jump_height = 300.f;
    float jump_distance = 350.f;
    float jump_cooldown = 2.5f;
    float time_since_last_jump = 0.f;
    std::string initial_frog_pos;

    void generateEnemyPosition() override;
    void print(std::ostream& os) const override;

public:
    explicit Frog();
    Enemy* clone() const override;
    void moveEnemy(float deltaTime) override;
};



#endif //FROG_H
