#include "game.h"
#include <SFML/Graphics.hpp>

// Точка входа в программу, содержащая главный игровой цикл
int main() {
    // Инициализация окна размером 600x600 пикселей и создание объекта игры
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Snake Game");
    Game game;
    
    // Главный цикл приложения: выполняется, пока окно не закрыто
    while (window.isOpen()) {
        // Обработка очереди событий (ввод пользователя, системные события)
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            // Закрытие окна при нажатии на системный крестик
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            // Обработка нажатий клавиш
            if (event->is<sf::Event::KeyPressed>()) {
                const auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    // Принудительное завершение работы приложения
                    if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                    
                    // Перезапуск игры клавишей 'R' (активно только при проигрыше)
                    if (keyEvent->code == sf::Keyboard::Key::R && game.isGameOver()) {
                        game.reset();
                    }
                }
            }
            
            // Передача события в игровой объект для обработки управления змейкой
            game.handleInput(*event);
        }
        
        // Обновление внутренней логики игры (движение змейки по таймеру)
        game.update();
        
        // Отрисовка текущего состояния игры (поле, объекты, интерфейс)
        game.render(window);
        
        // Наложение экрана проигрыша поверх основного игрового поля
        if (game.isGameOver()) {
            sf::Font font;
            if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
                sf::Text gameOverText(font);
                gameOverText.setString("Game Over!\nScore: " + std::to_string(game.getScore()) + "\nPress R to restart\nPress ESC to exit");
                gameOverText.setCharacterSize(40);
                gameOverText.setFillColor(sf::Color::White);
                
                // Вычисление границ текста для его точного центрирования на экране
                sf::FloatRect textRect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(sf::Vector2f(
                    textRect.size.x / 2, 
                    textRect.size.y / 2
                ));
                gameOverText.setPosition(sf::Vector2f(300, 300));
                
                window.draw(gameOverText);
            }
        }
        
        // Вывод отрисованного кадра из буфера на экран (double buffering)
        window.display();
    }
    
    return 0;
}