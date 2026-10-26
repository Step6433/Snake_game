#include "game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Конструктор игры
Game::Game() : direction(RIGHT), score(0), gameOver(false) {
    // Инициализируем генератор случайных чисел
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Змейка стартует в центре поля (5, 5), длина 1
    snake.push_back({5, 5});
    
    // Сразу создаем первую еду
    spawnFood();
}

// Генерация еды на свободной клетке
void Game::spawnFood() {
    do {
        food.x = std::rand() % FIELD_SIZE;
        food.y = std::rand() % FIELD_SIZE;
    } while (isSnakeAt(food)); // Повторяем, если сгенерировали координаты на змейке
}

// Проверка, находится ли змейка в данной точке
bool Game::isSnakeAt(const Point& p) const {
    for (const auto& segment : snake) {
        if (segment.x == p.x && segment.y == p.y) {
            return true;
        }
    }
    return false;
}

// Отрисовка игры
void Game::render(sf::RenderWindow& window) {
    // Очищаем окно чёрным цветом
    window.clear(sf::Color::Black);
    
    // Рисуем игровое поле (сетка)
    for (int y = 0; y < FIELD_SIZE; ++y) {
        for (int x = 0; x < FIELD_SIZE; ++x) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
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
    
    // Рисуем счёт в нижней части окна
    sf::Font font;
    if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        sf::Text scoreText(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(10, 510));
        window.draw(scoreText);
    }
}

// Обновление состояния игры
void Game::update() {
    // Проверяем, прошло ли достаточно времени для следующего движения
    if (moveClock.getElapsedTime().asSeconds() >= moveInterval) {
        moveSnake();
        moveClock.restart(); // Перезапускаем часы
    }
}

// Движение змейки
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

// Обработка ввода
void Game::handleInput(const sf::Event& event) {
    // Проверяем, была ли нажата клавиша
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

// Проверка окончания игры
bool Game::isGameOver() const {
    return gameOver;
}

// Получение счёта
int Game::getScore() const {
    return score;
}