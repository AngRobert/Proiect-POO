#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


class Enemy;
class Minigame;
// DE IMPLEMENTAT:
// clasele urmatoare:
// Minigame(contine elemente din clasele minigameurilor), etc.
class Game {
private:
    sf::RenderWindow window;
    sf::VideoMode video_mode;
    std::vector<Minigame> minigames;
public:
    //Constructori / Destructori:
    Game() = default;
    ~Game() = default;
};



class Player {};

class Enemy {
private:
    int enemy_difficulty;
    int enemy_speed;
    int enemy_size;
    std::string enemy_type;
public:
    explicit Enemy(const int difficulty_ = 1, int speed_ = 0, int size_ = 1, const std::string& enemy_type_ = "null") :
        enemy_difficulty(difficulty_), enemy_speed(speed_), enemy_size(size_), enemy_type(enemy_type_) {};
    Enemy(const Enemy& other_enemy) :
        enemy_difficulty(other_enemy.enemy_difficulty), enemy_speed(other_enemy.enemy_speed),
        enemy_size(other_enemy.enemy_size) {}
    ~Enemy() = default;
    Enemy& operator=(const Enemy& other_enemy) {
        enemy_difficulty = other_enemy.enemy_difficulty;
        enemy_speed = other_enemy.enemy_speed;
        enemy_size = other_enemy.enemy_size;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
        os << "has " << enemy.enemy_speed << "speed, " << enemy.enemy_size << "size, "
        << enemy.enemy_difficulty << "difficulty\n";
        return os;
    }
};

class Minigame {
private:
    const int max_enemies;
    const int minigame_difficulty;
    std::vector<Enemy> enemies;
public:
    explicit Minigame(const int max_enemies_ = 1, const int minigame_difficulty_ = 1, const std::vector<Enemy>& enemies_) :
    max_enemies(max_enemies_), minigame_difficulty(minigame_difficulty_), enemies(enemies_) {}
    friend std::ostream& operator<<(std::ostream& os, const Minigame& minigame) {
        os << "Minigame of difficulty " << minigame.minigame_difficulty << "has "
        << minigame.max_enemies << "max enemies\n";
        for (int i = 0; i < minigame.enemies.size(); i++) {
            os << "Enemy " << i << " " << minigame.enemies[i];
        }
        return os;
    }
};

int main() {

    return 0;
}
