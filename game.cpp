#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : direction(RIGHT), score(0), gameOver(false) {
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

bool Game::isSnakeAt(const Point& p) const {
    for (const auto& segment : snake) {
        if (segment.x == p.x && segment.y == p.y) {
            return true;
        }
    }
    return false;
}

void Game::render(sf::RenderWindow& window) {
    // Очищаем окно чёрным цветом
    window.clear(sf::Color::Black);
    
    // Рисуем игровое поле (сетка)
    for (int y = 0; y < FIELD_SIZE; ++y) {
        for (int x = 0; x < FIELD_SIZE; ++x) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
            // В SFML 3.0 setPosition принимает Vector2f
            cell.setPosition(sf::Vector2f(x * CELL_SIZE + 1, y * CELL_SIZE + 1));
            cell.setFillColor(sf::Color(30, 30, 30)); // Тёмно-серый фон
            window.draw(cell);
        }
    }
    
    // Рисуем еду
    sf::RectangleShape foodShape(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
    foodShape.setPosition(sf::Vector2f(food.x * CELL_SIZE + 2, food.y * CELL_SIZE + 2));
    foodShape.setFillColor(sf::Color::Red);
    window.draw(foodShape);
    
    // Рисуем змейку
    for (size_t i = 0; i < snake.size(); ++i) {
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
        segment.setPosition(sf::Vector2f(snake[i].x * CELL_SIZE + 2, snake[i].y * CELL_SIZE + 2));
        
        // Голова змейки зелёная, тело - светло-зелёное
        if (i == 0) {
            segment.setFillColor(sf::Color::Green);
        } else {
            segment.setFillColor(sf::Color(100, 200, 100));
        }
        
        window.draw(segment);
    }
}

void Game::update() {
    // Проверяем, прошло ли достаточно времени для следующего движения
    if (moveClock.getElapsedTime().asSeconds() >= moveInterval) {
        moveSnake();
        moveClock.restart(); // Перезапускаем часы
    }
}

void Game::moveSnake() {
    // Вычисляем новую позицию головы
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
    
    // 1. Проверка столкновения со стеной
    if (newHead.x < 0 || newHead.x >= FIELD_SIZE || 
        newHead.y < 0 || newHead.y >= FIELD_SIZE) {
        gameOver = true;
        return;
    }
    
    // 2. Проверка столкновения с собственным хвостом
    if (isSnakeAt(newHead)) {
        gameOver = true;
        return;
    }
    
    // 3. Проверяем, съели ли еду
    bool ateFood = (newHead.x == food.x && newHead.y == food.y);
    
    // Вставляем новую голову в начало вектора
    snake.insert(snake.begin(), newHead);
    
    if (ateFood) {
        // Если съели еду — увеличиваем счёт и генерируем новую еду
        // Хвост НЕ удаляем, змейка растёт
        score++;
        spawnFood();
    } else {
        // Если не съели — удаляем хвост (длина остаётся прежней)
        snake.pop_back();
    }
}

void Game::handleInput(const sf::Event& event) {
    // Проверяем, было ли нажата клавиша
    if (event.is<sf::Event::KeyPressed>()) {
        const auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent) {
            // Меняем направление в зависимости от нажатой клавиши
            // Змейка не может развернуться на 180 градусов
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