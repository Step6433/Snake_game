#include "game.h"
#include <iostream>
#include <cstdlib> // для rand и srand
#include <ctime>   // для time

// Конструктор игры
Game::Game() : direction(RIGHT), score(0), gameOver(false) {
    // Инициализируем генератор случайных чисел (делаем это один раз при старте)
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
    } while (isSnakeAt(food)); // Повторяем, если сгенерировали координаты прямо на змейке
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

// --- Заглушки для будущих шагов (чтобы код собирался) ---

void Game::update() {
    // Пока пусто
}

void Game::render() const {
    // Очистка экрана терминала (работает на macOS/Linux)
    system("clear");
    
    // Проходим по всем строкам (Y) и столбцам (X)
    for (int y = 0; y < FIELD_SIZE; ++y) {
        for (int x = 0; x < FIELD_SIZE; ++x) {
            Point current = {x, y};
            
            // 1. Проверяем, является ли эта клетка головой змейки (первый элемент вектора)
            if (current.x == snake[0].x && current.y == snake[0].y) {
                std::cout << "O "; // Голова
            } 
            // 2. Проверяем, является ли клетка частью тела змейки
            else if (isSnakeAt(current)) {
                std::cout << "* "; // Тело
            } 
            // 3. Проверяем, находится ли здесь еда
            else if (current.x == food.x && current.y == food.y) {
                std::cout << "F "; // Еда
            } 
            // 4. Иначе это пустая клетка
            else {
                std::cout << ". "; // Пустота
            }
        }
        std::cout << "\n"; // Переход на новую строку после каждого ряда
    }
    
    // Выводим счёт под полем
    std::cout << "Счёт: " << score << "\n";
}

void Game::handleInput(char key) {
    // Пока пусто
}

bool Game::isGameOver() const {
    return gameOver;
}

int Game::getScore() const {
    return score;
}