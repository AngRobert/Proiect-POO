#include "Game.h"
#include <iostream>

Game::Game() : rhythm_circle("textures/rhythmcircle_texture.png", sf::Vector2f(960.f, 150.f)), window(nullptr), event(), minigame_timer(1000), points_counter(0), max_points(5), end_game(false) {
    initWindow();
    initFont();
    initPoints();
    sf::Vector2f text_pos{1730.f, 400.f};
    initText(breath_text, text_pos, "BREATH:");
    text_pos.y = 440.f;
    initText(breath_value, text_pos, "");
    text_pos.x = 860.f;
    text_pos.y = 510.f;
    initText(game_over, text_pos, "GAME OVER !");
    text_pos.x = 30.f;
    text_pos.y = 30.f;
    initText(score_text, text_pos, "Points: ");
    text_pos.x += 130.f;
    initText(score_value, text_pos, "");
    generateMinigameTimer();
}

Game::~Game() {
    delete window;
}

void Game::initFont() {
    if (!font.loadFromFile("textures/fonts/Comic_Sans_MS.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
    }
}

void Game::initText(sf::Text& text, sf::Vector2f text_position, const std::string& text_string) const {
    text.setFont(font);
    text.setPosition(text_position);
    text.setCharacterSize(35);
    text.setFillColor(sf::Color::White);
    text.setString(text_string);
}

void Game::initWindow() {
    video_mode.height = 1080;
    video_mode.width = 1920;
    window = new sf::RenderWindow(video_mode, "SWIM!", sf::Style::Titlebar | sf::Style::Close);
    window->setFramerateLimit(60);
}

void Game::initPoints() {
    for (int i = 0; i < max_points; ++i) {
        points.emplace_back();
    }
}

void Game::updateMousePosition() {
    sf::Vector2i pixel_position = sf::Mouse::getPosition(*window);
    mouse_position_window = window->mapPixelToCoords(pixel_position);
}

void Game::updateText() {
    breath_value.setString(std::to_string(static_cast<int>(player.getBreath())));
    score_value.setString(std::to_string(points_counter));
}

void Game::updatePoints() {
    auto it = points.begin();
    while (it != points.end()) {
        if (player.isCollidingWithPoint(it->getPosition())) {
            it = points.erase(it);
            it = points.insert(it, Point());
            ++it;
            points_counter++;
        } else {
            ++it;
        }
    }
}

void Game::renderCircle() const {
    rhythm_circle.draw(*window);
}

void Game::renderText() const {
    window->draw(breath_text);
    window->draw(breath_value);
    window->draw(score_text);
    window->draw(score_value);
}

void Game::renderPlayer() const {
    player.draw(*window);
}

void Game::renderPoints() const {
    for (const auto& point : points) {
        point.draw(*window);
    }
}

void Game::pollGameEvents() {
    switch (event.type) {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::Escape == event.key.code) {
                window->close();
            } else if (!isGameOver() && !minigameTime()) {
                if (sf::Keyboard::W == event.key.code) {
                    player.doBreath();
                } else if (sf::Keyboard::Q == event.key.code || sf::Keyboard::E == event.key.code) {
                    player.doStroke(event.key.code);
                }
            }
            break;
        default:
            break;
    }
}

bool Game::isRunning() const {
    return window->isOpen();
}

bool Game::isGameOver() const {
    return end_game;
}

bool Game::minigameTime() const {
    return minigame_timer_clock.getElapsedTime().asSeconds() > static_cast<float>(minigame_timer);
}

float Game::updateDeltaTime() {
    return clock.restart().asSeconds();
}

void Game::renderGameOver() const {
    window->clear(sf::Color(255, 0, 0, 255));
    window->draw(game_over);
    window->display();
}

void Game::loadMinigame() {
    current_minigame.updateMinigame();
    current_minigame.renderMinigame(*window);
}

void Game::update() {
    if (player.isAlive()) {
        updateMousePosition();
        updateText();
        updatePoints();
        rhythm_circle.updateCircle();
        player.updatePlayer(mouse_position_window, updateDeltaTime());
    } else {
        end_game = true;
    }
}

void Game::render() const {
    window->clear(sf::Color(3, 177, 252, 255));
    renderPlayer();
    renderText();
    renderCircle();
    renderPoints();
    window->display();
}

void Game::pollEvents() {
    while (window->pollEvent(event)) {
        if (current_minigame.isMinigameRunning()) {
            current_minigame.pollMinigameEvents(*window, event, minigame_timer_clock);
        } else {
            pollGameEvents();
        }
    }
}

void Game::generateMinigameTimer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(10, 15);
    minigame_timer = dist(gen);
}
