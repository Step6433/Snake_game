#include "game.h"
#include <cstdlib>
#include <ctime>

Game::Game() : direction(RIGHT), score(0), gameOver(false), moveInterval(0.2f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    snake.push_back({5, 5});
    spawnFood();
}

void Game::spawnFood() {
    do {
        food.x = std::rand() % FIELD_SIZE;
        food.y = std::rand() % FIELD_SIZE;
    } while (isSnakeAt(food));
}

bool Game::isSnakeAt(const Point& point) const {
    for (const auto& segment : snake) {
        if (segment.x == point.x && segment.y == point.y) {
            return true;
        }
    }
    return false;
}

void Game::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    for (int y = 0; y < FIELD_SIZE; ++y) {
        for (int x = 0; x < FIELD_SIZE; ++x) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
            cell.setPosition(sf::Vector2f(x * CELL_SIZE + 1, y * CELL_SIZE + 1));
            cell.setFillColor(sf::Color(30, 30, 30));
            window.draw(cell);
        }
    }
    
    sf::RectangleShape foodShape(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
    foodShape.setPosition(sf::Vector2f(food.x * CELL_SIZE + 2, food.y * CELL_SIZE + 2));
    foodShape.setFillColor(sf::Color::Red);
    window.draw(foodShape);
    
    for (size_t i = 0; i < snake.size(); ++i) {
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
        segment.setPosition(sf::Vector2f(snake[i].x * CELL_SIZE + 2, snake[i].y * CELL_SIZE + 2));
        
        if (i == 0) {
            segment.setFillColor(sf::Color::Green);
        } else {
            segment.setFillColor(sf::Color(100, 200, 100));
        }
        
        window.draw(segment);
    }
    
    sf::Font font;
    if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        sf::Text scoreText(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(10, 555));
        window.draw(scoreText);
    }
}

void Game::update() {
    if (moveClock.getElapsedTime().asSeconds() >= moveInterval) {
        moveSnake();
        moveClock.restart();
    }
}

void Game::moveSnake() {
    Point newHead = snake[0];
    
    switch (direction) {
        case UP:
            newHead.y--;
            break;
        case DOWN:
            newHead.y++;
            break;
        case LEFT:
            newHead.x--;
            break;
        case RIGHT:
            newHead.x++;
            break;
    }
    
    if (newHead.x < 0 || newHead.x >= FIELD_SIZE || 
        newHead.y < 0 || newHead.y >= FIELD_SIZE) {
        gameOver = true;
        return;
    }
    
    if (isSnakeAt(newHead)) {
        gameOver = true;
        return;
    }
    
    bool ateFood = (newHead.x == food.x && newHead.y == food.y);
    
    snake.insert(snake.begin(), newHead);
    
    if (ateFood) {
        score++;
        spawnFood();
    } else {
        snake.pop_back();
    }
}

void Game::handleInput(const sf::Event& event) {
    if (event.is<sf::Event::KeyPressed>()) {
        const auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent) {
            if (keyEvent->code == sf::Keyboard::Key::Up && direction != DOWN) {
                direction = UP;
            }
            else if (keyEvent->code == sf::Keyboard::Key::Down && direction != UP) {
                direction = DOWN;
            }
            else if (keyEvent->code == sf::Keyboard::Key::Left && direction != RIGHT) {
                direction = LEFT;
            }
            else if (keyEvent->code == sf::Keyboard::Key::Right && direction != LEFT) {
                direction = RIGHT;
            }
        }
    }
}

bool Game::isGameOver() const {
    return gameOver;
}

int Game::getScore() const {
    return score;
}