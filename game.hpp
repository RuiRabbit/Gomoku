#pragma once

#include <iostream>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>


class Game{
public:
    Game();
    ~Game() = default;

    void run();
    
    void processInput();
    
    void update();

    void render();

private:
    sf::RenderWindow window;
    std::map <std::string, sf::Texture> texture;
    sf::Sprite background;

    int line_amount;
    int grid_size;

    void loadAllTexture();
    void DrawBoard();
};

Game::Game(){
    window.create(sf::VideoMode(900, 900), "Gomoku", sf::Style::Titlebar | sf::Style::Close);

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    
    line_amount = 15;
    grid_size = window.getSize().x / line_amount;
    // std::cout<<"window size x = "<<window.getSize().x<<std::endl;
    // std::cout<<"Grid size = "<<grid_size<<std::endl;

    loadAllTexture();
}

void Game::run(){
    while(window.isOpen()){
        processInput();
        
        // update();

        render();
    }
}

void Game::processInput(){
    sf::Event event;
    while(window.pollEvent(event)){
        // Event input
        if(event.type == sf::Event::Closed)
            window.close();
    }
    // Real-time input
}

void Game::render(){
    window.clear();

    // window.draw();
    DrawBoard();

    window.display();
}

void Game::loadAllTexture(){
    if(!texture["wood"].loadFromFile("src/texture/wood.jpg")){
        std::cout<<"Error: Loading wood texture failed"<<std::endl;
    }
    background.setTexture(texture["wood"]);
    background.setScale(0.5f, 0.5f);
}

void Game::DrawBoard(){
    window.draw(background);
    // window.clear(sf::Color(255,207,97));
    float mid_point = 1.0 * grid_size / 2;

    // Horizontal lines
    for(int x = 0; x < line_amount; x++){
        sf::Vertex hline[] = {
            sf::Vertex(sf::Vector2f(mid_point, mid_point + x * grid_size), sf::Color::Black),
            sf::Vertex(sf::Vector2f(grid_size * line_amount - mid_point, mid_point + x * grid_size), sf::Color::Black)
        };
        window.draw(hline, 2, sf::Lines);
    };

    // Vertical lines
    for(int y = 0; y < line_amount; y++){
        sf::Vertex vline[] = {
            sf::Vertex(sf::Vector2f(mid_point + y * grid_size, mid_point), sf::Color::Black),
            sf::Vertex(sf::Vector2f(mid_point + y * grid_size, grid_size * line_amount - mid_point), sf::Color::Black)
        };
        window.draw(vline, 2, sf::Lines);
    };

    // Draw start points
    float radius = mid_point / 5;
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Black);

    std::pair<int, int> dot_pos[] = {
        {3, 3},
        {3, 11},
        {11, 3},
        {7, 7},
        {11, 11}
    };

    for(auto &[x, y] : dot_pos){
        circle.setPosition(mid_point + x * grid_size - radius,
                            mid_point + y * grid_size - radius);
        window.draw(circle);
    }

    // for (int x = 0; x < 3; x++) {
    //     for (int y = 0; y < 3; y++) {
    //         int cells_between = 6;
    //         int xDistance = (grid_size * cells_between) * x;
    //         int yDistance = (grid_size * cells_between) * y;
    //         circle.setPosition( ((mid_point + grid_size * 3.f) + xDistance ) - radius,
    //                             ((mid_point + grid_size * 3.f) + yDistance) - radius);
    //         window.draw(circle);
    //     };
    // };
}