#include "game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Snake Game");
    Game game;
    
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            
            // Передаём событие в игру
            game.handleInput(*event);
        }
        
        // Обновляем состояние игры
        game.update();
        
        // Рисуем игру
        game.render(window);
        
        // Отображаем на экран
        window.display();
    }
    
    return 0;
}