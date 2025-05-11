#include "Game.h"
#include <iostream>

#include "GameExceptions.h"

Game::Game() : rhythm_circle("textures/rhythmcircle_texture.png", sf::Vector2f(960.f, 150.f)), window(nullptr), event(), minigame_timer(1000), points_counter(0), max_points(5) {
    initWindow();
    initFont();
    initPoints();
    initText(breath_text, "BREATH:", 1730.f, 400.f);
    initText(breath_value, "", 1730.f, 440.f);
    initText(score_text, "Points: ", 30.f, 30.f);
    initText(score_value, "", 160.f, 30.f);
    generateMinigameTimer();
}

Game::~Game() {
    delete window;
}

void Game::initFont() {
    if (!font.loadFromFile("textures/fonts/Comic_Sans_MS.ttf")) {
        throw ResourceLoadException{"Failed to load game text font!"};
    }
}

void Game::initText(sf::Text& text, const std::string& text_string, const float text_pos_x, const float text_pos_y) const {
    text.setFont(this->font);
    text.setPosition(text_pos_x, text_pos_y);
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

void Game::renderGame() const {
    window->clear(sf::Color(3, 177, 252, 255));
    renderPlayer();
    renderText();
    renderCircle();
    renderPoints();
    window->display();
}

void Game::pollGameEvents() {
    switch (event.type) {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::Escape == event.key.code) {
                window->close();
            } else if (!this->game_over_screen.isGameOver() && !minigameTime()) {
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

bool Game::minigameTime() const {
    return minigame_timer_clock.getElapsedTime().asSeconds() > static_cast<float>(minigame_timer);
}

float Game::updateDeltaTime() {
    return clock.restart().asSeconds();
}

void Game::update() {
    updateMousePosition();
    if (this->menu_screen.isMenuScreenActive()) {
        this->minigame_timer_clock.restart();
        this->menu_screen.update(this->mouse_position_window);
    }
    else if (this->minigameTime() && !current_minigame.isMinigameFailed()) {
        this->current_minigame.updateMinigame(this->minigame_timer_clock);
    }
    else if (player.isAlive() && !current_minigame.isMinigameFailed()) {
        updateText();
        updatePoints();
        rhythm_circle.updateCircle();
        player.updatePlayer(mouse_position_window, updateDeltaTime());
    }
    else {
        this->points_counter = 0; // AND OTHER PLAYER RELATED INFO that doesn't get reset in the player constructor? ???
        this->minigame_timer_clock.restart();
        this->game_over_screen.updateGameOver(this->mouse_position_window);
    }
}

void Game::render() const {
    if (this->menu_screen.isMenuScreenActive()) {
        this->menu_screen.render(*this->window);
    }
    else if (this->game_over_screen.isGameOver() || this->current_minigame.isMinigameFailed()) {
        this->game_over_screen.renderGameOver(*this->window);
    }
    else if (this->minigameTime()) {
        this->current_minigame.renderMinigame(*this->window);
    }
    else {
        this->renderGame();
    }
}

void Game::pollEvents() {
    while (window->pollEvent(event)) {
        if (this->menu_screen.isMenuScreenActive()) {
            this->menu_screen.pollMenuScreenEvents(*this->window, this->event, this->mouse_position_window);
        }
        else if (this->game_over_screen.isGameOver()) {
            this->game_over_screen.pollGameOverEvents(*this->window, this->event, this->mouse_position_window, this->player, this->current_minigame);
        }
        else if (current_minigame.isMinigameRunning()) {
            current_minigame.pollMinigameEvents(*window, event);
        }
        else {
            pollGameEvents();
        }
    }
}

void Game::generateMinigameTimer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(3, 4);
    minigame_timer = dist(gen);
}
