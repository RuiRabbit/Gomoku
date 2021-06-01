#pragma once

#include <iostream>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cmath>
#include <queue>
#include <time.h>
#include <random>
#include <chrono>

#define WAIT_SEC 20
#define ACCEPT_RATE 0.5
// #include "ai.hpp"


class Game{
public:
    Game();
    ~Game() = default;

    void run();
    
    void processInput();
    
    void update();
    void updateScore(int x, int y);

    void render();
    void enterPlayerColor(int color);
    void enterAiMode(int mode);

    bool checkWin(int x, int y);
    int evaluate(int x, int y, int color);


    // AI
    void ai_best_pick();
    void ai_alpha_beta();

private:
    sf::RenderWindow window;
    std::map <std::string, sf::Texture> texture;
    std::map <std::string, int> value;
    sf::Sprite background;
    
    // std::mt19937 rng;

    int line_amount;
    int grid_size;
    std::vector <std::vector<int>> board;
    std::vector <std::vector<std::vector<int>>> board_score;
    int total_score[2] = {0};
    bool first_move = true;
    bool mouse_in_window;
    sf::Vector2i mouse_pos;
    bool black_turn;
    int player_color;
    int ai_mode;

    int alpha_beta(std::vector <std::vector<int>> &board, int color, int depth, int cutlimit);

    void loadAllTexture();
    void loadValue();
    void DrawBoard();
    void DrawStone();
};

Game::Game(){
    window.create(sf::VideoMode(900, 900), "Gomoku", sf::Style::Titlebar | sf::Style::Close);

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    
    // rng = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());

    line_amount = 15;
    grid_size = window.getSize().x / line_amount;
    board = std::vector<std::vector<int>> (line_amount, std::vector<int>(line_amount, 0));
    board_score = std::vector<std::vector<std::vector<int>>> (line_amount, std::vector<std::vector<int>>(line_amount, std::vector<int>(2, 0)));
    
    loadValue();
    
    for(int i = 0; i < line_amount; i++){
        for(int j = 0; j < line_amount; j++){
            board_score[i][j][0] = evaluate(i, j, 1);
            total_score[0] += board_score[i][j][0];
            board_score[i][j][1] = evaluate(i, j, 2);
            total_score[1] += board_score[i][j][1];
        }
    }
    for(int i = 0; i < line_amount; i++){
        for(int j = 0; j < line_amount; j++){
            printf("%4d ", board_score[i][j][0]);
            // std::cout<<board_score[i][j][color - 1]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    for(int i = 0; i < line_amount; i++){
        for(int j = 0; j < line_amount; j++){
            printf("%4d ", board_score[i][j][1]);
            // std::cout<<board_score[i][j][color - 1]<<" ";
        }
        std::cout<<std::endl;
    }
    
    for(int i = 2; i < line_amount - 2; i++){
        for(int j = 2; j < line_amount - 2; j++){
            board[i][j] = -1;
        }
    }
    // std::cout<<"window size x = "<<window.getSize().x<<std::endl;
    // std::cout<<"Grid size = "<<grid_size<<std::endl;
    black_turn = true;

    loadAllTexture();
}

bool Game::checkWin(int x, int y){
    int color = board[x][y];
    if(!color)
        return false;
    int total = 0;
    int i = x, j = y;
    while(i >= 0 && board[i][j] == color){
        i--;
        total++;
    }
    i = x;
    while(i < line_amount && board[i][j] == color){
        i++;
        total++;
    }
    i = x;
    if(total == 6)
        return true;
    total = 0;

    while(j >= 0 && board[i][j] == color){
        j--;
        total++;
    }
    j = y;
    while(j < line_amount && board[i][j] == color){
        j++;
        total++;
    }
    j = y;
    if(total == 6)
        return true;
    total = 0;
    
    while(i >= 0 && j >= 0 && board[i][j] == color){
        i--, j--;
        total++;
    }
    i = x, j = y;
    while(i < line_amount && j < line_amount && board[i][j] == color){
        i++, j++;
        total++;
    }
    i = x, j = y;
    if(total == 6)
        return true;
    total = 0;

    while(i >= 0 && j < line_amount && board[i][j] == color){
        i--, j++;
        total++;
    }
    i = x, j = y;
    while(i < line_amount && j >= 0 && board[i][j] == color){
        i++, j--;
        total++;
    }
    if(total == 6)
        return true;
    return false;
}

int Game::evaluate(int x, int y, int color){
    int score = 0;
    int nx = x, ny = y;
    int origin_color = board[x][y];
    if(origin_color == (color ^ 3))
        return 0;
    board[x][y] = color;
    if(checkWin(x, y)){
        board[x][y] = origin_color;
        return 100000;
    }
    board[x][y] = origin_color;
    int space = 2;
    nx--;
    std::string key = "1";
    // std::cout<<"my color: "<<color<<std::endl;
    // std::cout<<"enemy color: "<<(color ^ 3)<<std::endl;
    while(nx >= 0 && board[nx][ny] != (color ^ 3) && space > 0 && abs(nx - x) < 5){
        if(board[nx][ny] != color)
            space--;
        key = (char)('0' + (board[nx][ny] == color)) + key;
        nx--;
    }
    nx = x + 1;
    space = 2;
    while(nx < line_amount && board[nx][ny] != (color ^ 3) && space > 0 && abs(nx - x) < 5){
        if(board[nx][ny] != color)
            space--;
        key = key + (char)('0' + (board[nx][ny] == color));
        nx++;
    }
    // std::cout<<"Evaluate string: "<<key<<std::endl;
    score += value[key];

    key = "1";
    nx = x;
    ny = y - 1;
    space = 2;
    while(ny >= 0 && board[nx][ny] != (color ^ 3) && space > 0 && abs(ny - y) < 5){
        if(board[nx][ny] != color)
            space--;
        key = (char)('0' + (board[nx][ny] == color)) + key;
        ny--;
    }
    ny = y + 1;
    space = 2;
    while(ny < line_amount && board[nx][ny] != (color ^ 3) && space > 0 && abs(ny - y) < 5){
        if(board[nx][ny] != color)
            space--;
        key = key + (char)('0' + (board[nx][ny] == color));
        ny++;
    }
    // std::cout<<"Evaluate string: "<<key<<std::endl;
    score += value[key];

    key = "1";
    nx = x - 1;
    ny = y - 1;
    space = 2;
    while(nx >= 0 && ny >= 0 && board[nx][ny] != (color ^ 3) && space > 0 && abs(nx - x) < 5){
        if(board[nx][ny] != color)
            space--;
        key = (char)('0' + (board[nx][ny] == color)) + key;
        nx--;
        ny--;
    }
    nx = x + 1;
    ny = y + 1;
    space = 2;
    while(nx < line_amount && ny < line_amount && board[nx][ny] != (color ^ 3) && space > 0 && abs(nx - x) < 5){
        if(board[nx][ny] != color)
            space--;
        key = key + (char)('0' + (board[nx][ny] == color));
        nx++;
        ny++;
    }
    // std::cout<<"Evaluate string: "<<key<<std::endl;
    score += value[key];

    key = "1";
    nx = x - 1;
    ny = y + 1;
    space = 2;
    while(nx >= 0 && ny < line_amount && board[nx][ny] != (color ^ 3) && space > 0 && abs(nx - x) < 5){
        if(board[nx][ny] != color)
            space--;
        key = (char)('0' + (board[nx][ny] == color)) + key;
        nx--;
        ny++;
    }
    nx = x + 1;
    ny = y - 1;
    space = 2;
    while(nx < line_amount && ny >= 0 && board[nx][ny] != (color ^ 3) && space > 0 && abs(nx - x) < 5){
        if(board[nx][ny] != color)
            space--;
        key = key + (char)('0' + (board[nx][ny] == color));
        nx++;
        ny--;
    }
    // std::cout<<"Evaluate string: "<<key<<std::endl;
    score += value[key];

    // std::cout<<x<<" "<<y<<" "<<score<<std::endl;
    return score;
}

void Game::run(){
    while(window.isOpen()){
        processInput();
        
        update();

        render();
    }
}

void Game::processInput(){
    sf::Event event;
    while(window.pollEvent(event)){
        // Event input
        if(event.type == sf::Event::Closed)
            window.close();        
        if(event.type == sf::Event::MouseMoved){
            mouse_pos = {event.mouseMove.x, event.mouseMove.y};
        }
        if(event.type == sf::Event::MouseEntered){
            mouse_in_window = true;
        }
        if(event.type == sf::Event::MouseLeft){
            mouse_in_window = false;
        }
        if(event.type == sf::Event::MouseButtonReleased
            && !board[mouse_pos.x / grid_size][mouse_pos.y / grid_size]){
            int x = mouse_pos.x / grid_size, y = mouse_pos.y / grid_size;
            board[x][y] = 2 - black_turn;
            updateScore(x, y);
            std::cout<<evaluate(x, y, 2 - black_turn)<<std::endl;
            if(checkWin(mouse_pos.x / grid_size, mouse_pos.y / grid_size)){
                std::cout<<"Winner: ";
                if(black_turn)
                    std::cout<<"Black";
                else
                    std::cout<<"White";
                std::cout<<std::endl;
                window.close();
            }
            black_turn = !black_turn;
            if(first_move){
                for(int i = 2; i < line_amount - 2; i++){
                    for(int j = 2; j < line_amount - 2; j++){
                        board[i][j] = 0;
                    }
                }
                first_move = false;
            }
        }
    }
    // Real-time input
    // if(sf::Mouse::getPosition(window).x > 0
    //     && sf::Mouse::getPosition(window).y > 0
    //     && sf::Mouse::getPosition)
}

void Game::enterPlayerColor(int color){
    player_color = color;
    return;
}

void Game::enterAiMode(int mode){
    ai_mode = mode;
    return;
}

void Game::update(){
    // std::cout<<"ai! "<<2 - black_turn<<" "<<player_color<<std::endl;
    if(2 - black_turn != player_color){
        if(ai_mode == 1)
            ai_best_pick();
        else if(ai_mode == 2)
            ai_alpha_beta();
        // std::cout<<rng()<<std::endl;
        // std::cout<<(rng() / RAND_MAX)<<std::endl;
        // if(((double)rng() / RAND_MAX) < 0.5){
        //     std::cout<<"first strategy"<<std::endl;
        //     ai_best_pick();
        // }
        // else{
        //     std::cout<<"Second strategy"<<std::endl;
        //     ai_alpha_beta();
        // }
    }
    return;
}

void Game::updateScore(int x, int y){
    int nx = x - 1;
    int ny = y;
    std::pair <int, int> move[] = {
        {-1, 0},
        {-1, 1},
        {0, 1},
        {1, 1},
        {1, 0},
        {1, -1},
        {0, -1},
        {-1, -1}
    };
    // int score = evaluate(x, y, 1);
    // total_score[0] = total_score[0] - board_score[x][y][0] + score;
    // board_score[x][y][0] = score;
    // score = evaluate(x, y, 2);
    // total_score[1] = total_score[1] - board_score[x][y][1] + score;
    // board_score[x][y][1] = score;
    // for(int i = 1; i < 5; i++){
    //     for(int j = 0; j < 8; j++){
    //         int nx = x + i * move[j].first;
    //         int ny = y + i * move[j].second;
    //         if(nx >= 0 && nx < line_amount && ny >= 0 && ny < line_amount){
    //             if(board[i][j]){
    //                 score = 0;
    //                 total_score[0] = total_score[0] - board_score[i][j][0] + score;
    //                 board_score[i][j][0] = score;
    //                 total_score[1] = total_score[1] - board_score[i][j][1] + score;
    //                 board_score[i][j][1] = score;
    //             }
    //             if(board[i][j] != 2){
    //                 score = evaluate(i, j, 1);
    //                 total_score[0] = total_score[0] - board_score[i][j][0] + score;
    //                 board_score[i][j][0] = score;
    //             }
    //             if(board[i][j] != 1){
    //                 score = evaluate(i, j, 2);
    //                 total_score[1] = total_score[1] - board_score[i][j][1] + score;
    //                 board_score[i][j][1] = score;
    //             }
    //         }
    //     }
    // }
    for(int i = std::max(0, x - 4); i <= std::min(line_amount - 1, x + 4); i++){
        for(int j = std::max(0, y - 4); j <= std::min(line_amount - 1, y + 4); j++){
            int score;
            if(board[i][j]){
                score = 0;
                total_score[0] = total_score[0] - board_score[i][j][0] + score;
                board_score[i][j][0] = score;
                total_score[1] = total_score[1] - board_score[i][j][1] + score;
                board_score[i][j][1] = score;
            }
            if(board[i][j] != 2){
                score = evaluate(i, j, 1);
                total_score[0] = total_score[0] - board_score[i][j][0] + score;
                board_score[i][j][0] = score;
            }
            if(board[i][j] != 1){
                score = evaluate(i, j, 2);
                total_score[1] = total_score[1] - board_score[i][j][1] + score;
                board_score[i][j][1] = score;
            }
        }
    }
}

void Game::render(){
    window.clear();

    // window.draw();
    DrawBoard();
    DrawStone();

    window.display();
}

void Game::loadAllTexture(){
    if(!texture["wood"].loadFromFile("src/texture/wood.jpg")){
        std::cout<<"Error: Loading wood texture failed"<<std::endl;
    }
    background.setTexture(texture["wood"]);
    background.setScale(0.5f, 0.5f);
}

void Game::loadValue(){
    std::fstream file;
    file.open("value.txt", std::ios::in);
    while(!file.eof()){
        
        std::string str;
        int val;
        file>>str;
        if(str == "")
            continue;
        file>>val;
        value[str] = val;
        // std::cout<<"modify: "<<str<<" "<<val<<std::endl;
    }
    file.close();
}

void Game::DrawBoard(){
    window.draw(background);
    window.clear(sf::Color(255,207,97));
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

void Game::DrawStone(){
    // sf::Vector2 mouse_pos = sf::Mouse::getPosition(window);
    float radius = 1.0 * grid_size / 2;
    sf::CircleShape select_stone(radius);
    if(black_turn)
        select_stone.setFillColor(sf::Color(0, 0, 0, 128));
    else
        select_stone.setFillColor(sf::Color(255, 255, 255, 128));
    if(mouse_in_window
        && mouse_pos.x > 0
        && mouse_pos.x < window.getSize().x
        && mouse_pos.y > 0
        && mouse_pos.y < window.getSize().y
        && !board[mouse_pos.x / grid_size][mouse_pos.y / grid_size]){
        select_stone.setPosition(mouse_pos.x / grid_size * grid_size,
                                    mouse_pos.y / grid_size * grid_size);
        window.draw(select_stone);
    }
    sf::CircleShape black_stone(radius);
    black_stone.setFillColor(sf::Color::Black);
    sf::CircleShape white_stone(radius);
    white_stone.setFillColor(sf::Color::White);

    for(int i = 0; i < line_amount; i++){
        for(int j = 0; j < line_amount; j++){
            if(board[i][j] == 1){
                black_stone.setPosition(i * grid_size, j * grid_size);
                window.draw(black_stone);
            }
            else if(board[i][j] == 2){
                white_stone.setPosition(i * grid_size, j * grid_size);
                window.draw(white_stone);
            }
        }
    }
    return;
} 

// AI
void Game::ai_best_pick(){
    std::priority_queue <std::pair<int, std::pair<int, int>>> pq;
    int color = 2 - black_turn;
    int ai_score = 0;
    int player_score = 0;
    for(int i = 0; i < line_amount; i++){
        for(int j = 0; j < line_amount; j++){
            if(board[i][j])
                continue;
            board[i][j] = color;
            int score = 0;
            score += evaluate(i, j, color);
            ai_score += evaluate(i, j, color);
            // if(checkWin(i, j)){
            //     score += 100000;
            //     ai_score + 100000;
            // }
            // else{
            //     score += evaluate(i, j, color);
            //     ai_score += evaluate(i, j, color);
            // }
            board[i][j] = (color ^ 3);
            score += evaluate(i, j, (color ^ 3));
            player_score += evaluate(i, j, (color ^ 3));
            // if(checkWin(i, j)){
            //     score += 100000;
            //     player_score += 100000;
            // }
            // else{
            //     score += evaluate(i, j, (color ^ 3));
            //     player_score += evaluate(i, j, (color ^ 3));
            // }
            board[i][j] = 0;
            pq.push({score, {i, j}});
        }
    }
    int val = pq.top().first;
    int x = pq.top().second.first;
    int y = pq.top().second.second;
    std::cout<<"best move: "<<x<<" "<<y<<" = "<<val<<std::endl;
    std::cout<<"value: "<<ai_score - player_score<<std::endl;

    board[x][y] = color;
    if(checkWin(x, y)){
        std::cout<<"Winner: ";
        if(black_turn)
            std::cout<<"Black";
        else
            std::cout<<"White";
        std::cout<<std::endl;
        window.close();
    }
    black_turn = !black_turn;
    if(first_move){
        for(int i = 2; i < line_amount - 2; i++){
            for(int j = 2; j < line_amount - 2; j++){
                board[i][j] = 0;
            }
        }
        first_move = false;
    }
    return;
}

void Game::ai_alpha_beta(){
    // std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    int color = 2 - black_turn;

    // for(int i = 0; i < line_amount; i++){
    //     for(int j = 0; j < line_amount; j++){
    //         printf("%4d ", board_score[i][j][color - 1]);
    //         // std::cout<<board_score[i][j][color - 1]<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
    // std::cout<<std::endl;
    // for(int i = 0; i < line_amount; i++){
    //     for(int j = 0; j < line_amount; j++){
    //         printf("%4d ", board_score[i][j][(color ^ 3) - 1]);
    //         // std::cout<<board_score[i][j][color - 1]<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
    
    std::priority_queue <std::pair<int, std::pair<int, int>>> pq;
    for(int i = 0; i < line_amount; i++){
        for(int j = 0; j < line_amount; j++){
            if(board[i][j])
                continue;
            pq.push({board_score[i][j][color - 1] + board_score[i][j][(color ^ 3) - 1], {i, j}});
            // board[i][j] = color;
            // int score = 0;
            // score += evaluate(i, j, color);
            // // if(checkWin(i, j)){
            // //     score += 100000;
            // // }
            // // else{
            // //     score += evaluate(i, j, color);
            // // }
            // board[i][j] = (color ^ 3);
            // score += evaluate(i, j, (color ^ 3));
            // // if(checkWin(i, j)){
            // //     score += 100000;
            // // }
            // // else{
            // //     score += evaluate(i, j, (color ^ 3));
            // // }
            // board[i][j] = 0;
            // pq.push({score, {i, j}});
        }
    }
    int maximum = -2000000000;
    int mx = 0;
    int my = 0;
    clock_t start = clock();
    while(!pq.empty()){
        clock_t now = clock();
        if((double)(now - start) / CLOCKS_PER_SEC > WAIT_SEC)
            break;
        int x = pq.top().second.first;
        int y = pq.top().second.second;
        pq.pop();
        board[x][y] = color;
        updateScore(x, y);
        int score = alpha_beta(board, (color ^ 3), 1, maximum);
        board[x][y] = 0;
        updateScore(x, y);
        if(score > maximum){
            maximum = score;
            mx = x;
            my = y;
            std::cout<<"new best move: ("<<x<<", "<<y<<") = "<<maximum<<std::endl;
        }
        // if(score == maximum && rng() / RAND_MAX < ACCEPT_RATE){
        //     mx = x;
        //     my = y;
        //     std::cout<<"new best move: ("<<x<<", "<<y<<") = "<<maximum<<std::endl;
        // }
    }
    std::cout<<"best move: "<<mx<<" "<<my<<std::endl;
    std::cout<<"value: "<<maximum<<std::endl;

    board[mx][my] = color;
    updateScore(mx, my);
    if(checkWin(mx, my)){
        std::cout<<"Winner: ";
        if(black_turn)
            std::cout<<"Black";
        else
            std::cout<<"White";
        std::cout<<std::endl;
        window.close();
    }
    black_turn = !black_turn;
    if(first_move){
        for(int i = 2; i < line_amount - 2; i++){
            for(int j = 2; j < line_amount - 2; j++){
                board[i][j] = 0;
            }
        }
        first_move = false;
    }
    return;
}

int Game::alpha_beta(std::vector <std::vector<int>> &board, int color, int depth, int cutlimit){
    // if(depth < 2)
        // std::cout<<"now depth = "<<depth<<std::endl;
    if(depth >= 2){
        // int my_score = 0;
        // int enemy_score = 0;
        // for(int i = 0; i < line_amount; i++){
        //     for(int j = 0; j < line_amount; j++){
        //         if(board[i][j])
        //             continue;
        //         // board[i][j] = color;
        //         // if(checkWin(i, j)){
        //         //     my_score += 100000;
        //         // }
        //         // else{
        //         //     my_score += evaluate(i, j, color);
        //         // }
        //         // board[i][j] = (color ^ 3);
        //         // if(checkWin(i, j)){
        //         //     enemy_score += 100000;
        //         // }
        //         // else{
        //         //     enemy_score += evaluate(i, j, (color ^ 3));
        //         // }
        //         // board[i][j] = 0;
        //     }
        // }
        // std::cout<<"return score = "<<-1 * (my_score - enemy_score)<<std::endl;
        // std::cout<<"return score = "<<-1 * (total_score[color - 1] - total_score[(color ^ 3) - 1])<<std::endl;
        return -1 * (total_score[color - 1] - total_score[(color ^ 3) - 1]);
    }
    std::priority_queue <std::pair<int, std::pair<int, int>>> pq;
    for(int i = 0; i < line_amount; i++){
        for(int j = 0; j < line_amount; j++){
            if(board[i][j])
                continue;
            pq.push({board_score[i][j][color - 1] + board_score[i][j][(color ^ 3) - 1] * 1.1, {i, j}});
            // board[i][j] = color;
            // int score = 0;
            // if(checkWin(i, j)){
            //     score += 100000;
            // }
            // else{
            //     score += evaluate(i, j, color);
            // }
            // board[i][j] = (color ^ 3);
            // if(checkWin(i, j)){
            //     score += 100000;
            // }
            // else{
            //     score += evaluate(i, j, (color ^ 3));
            // }
            // board[i][j] = 0;
            // pq.push({score, {i, j}});
        }
    }
    int maximum = -2000000000;
    while(!pq.empty()){
        int x = pq.top().second.first;
        int y = pq.top().second.second;
        pq.pop();
        board[x][y] = color;
        updateScore(x, y);
        int score = alpha_beta(board, (color ^ 3), depth + 1, maximum);
        board[x][y] = 0;
        updateScore(x, y);
        if(score > maximum)
            maximum = score;
        if(-1 * maximum <= cutlimit){
            return -1 * maximum;
        }
    }
    return -1 * maximum;
}