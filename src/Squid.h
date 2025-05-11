#ifndef SQUID_H
#define SQUID_H
#include "Enemy.h"


class Squid : public Enemy {
private:
    void generateEnemyPosition() override;
    void moveEnemy(float deltaTime) override;
    void print(std::ostream& os) const override;

public:
    explicit Squid();
    Enemy * clone() const override;
};

#endif //SQUID_H