#include "game.h"
#include <SFML/Graphics.hpp>

// Точка входа в программу
int main() {
    // Создание игрового окна 600x600 пикселей
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Snake Game");
    Game game;
    
    // Главный игровой цикл
    while (window.isOpen()) {
        // Обработка очереди событий
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            // Закрытие окна по крестику
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            // Обработка нажатий клавиш
            if (event->is<sf::Event::KeyPressed>()) {
                const auto keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent) {
                    // Выход из игры по клавише Escape
                    if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                    
                    // Перезапуск игры по клавише R после проигрыша
                    if (keyEvent->code == sf::Keyboard::Key::R && game.isGameOver()) {
                        game.reset();
                    }
                }
            }
            
            // Передача события в игру для обработки управления
            game.handleInput(*event);
        }
        
        // Обновление игрового состояния (движение змейки по таймеру)
        game.update();
        
        // Отрисовка текущего кадра
        game.render(window);
        
        // Наложение экрана Game Over поверх игры при проигрыше
        if (game.isGameOver()) {
            sf::Font font;
            if (font.openFromFile("/System/Library/Fonts/Helvetica.ttc")) {
                sf::Text gameOverText(font);
                gameOverText.setString("Game Over!\nScore: " + std::to_string(game.getScore()) + "\nPress R to restart\nPress ESC to exit");
                gameOverText.setCharacterSize(40);
                gameOverText.setFillColor(sf::Color::White);
                
                // Центрирование текста по середине окна
                sf::FloatRect textRect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(sf::Vector2f(
                    textRect.size.x / 2, 
                    textRect.size.y / 2
                ));
                gameOverText.setPosition(sf::Vector2f(300, 300));
                
                window.draw(gameOverText);
            }
        }
        
        // Вывод отрисованного кадра на экран
        window.display();
    }
    
    return 0;
}