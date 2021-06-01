#include <iostream>
#include <SFML/Graphics.hpp>
#include "game.hpp"

int main(){
    std::cout<<"Pick player's color"<<std::endl;
    std::cout<<"1) black, 2) white"<<std::endl;
    int color;
    std::cin>>color;
    std::cout<<"Choose Ai's mode"<<std::endl;
    std::cout<<"1) best pick, 2) alpha-beta"<<std::endl;
    int mode;
    std::cin>>mode;

    Game core;
    core.enterPlayerColor(color);
    core.enterAiMode(mode);
    core.run();

    return 0;
}