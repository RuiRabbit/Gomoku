#include <iostream>
#include <SFML/Graphics.hpp>

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 800), "Gomoku", sf::Style::Titlebar | sf::Style::Close);
    
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    sf::CircleShape shape(400.f);
    sf::Texture wood;
    if(!wood.loadFromFile("src/texture/wood.jpg")){
        std::cout<<"Error when load texture : wood"<<std::endl;
    }
    sf::Sprite background(wood);
    background.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(background.getTexture()->getSize())));
    shape.setFillColor(sf::Color::Blue);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
            // if(event.type == sf::Event::MouseMoved){
            //     std::cout<<event.mouseMove.x<<" "<<event.mouseMove.y<<std::endl;
            // }
        }

        window.clear();
        window.draw(shape);
        window.draw(background);
        window.display();
    }

    return 0;
}