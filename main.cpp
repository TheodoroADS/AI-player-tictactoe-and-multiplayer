#include <iostream>
#include <string>
#include <conio.h>
#include "tictac.h"
#include "TicTacAI.h"


#define CLEAR_SCREEN "\x1B[2J\x1B[1;1H"


enum GameMode {

    PVP,
    AI

};



GameMode menu() {


    GameMode option = PVP;

    char input;
    bool selection_changed = true;

    do {


        if (selection_changed) {

            std::cout << CLEAR_SCREEN;
            std::cout << "Select your game mode :\n"
                << ((option == PVP) ? "> " : "")
                << "Player vs Player \n"
                << ((option == AI) ? "> " : "") 
                << "Player vs Machine"
                << std::endl;
        }

        input = _getch();

        if (input == 'w'){
          
            selection_changed = option != PVP;
            option = PVP;
        
        } else if (input == 's'){

            selection_changed = option != AI;     
            option = AI;  
        } 

    } while(input != ' ');



    return option;

}


void playerVsPlayer() {


    TicTac current_board;
    TicTac::TicTacValue player = TicTac::X;
    TicTac::TicTacValue other_player = player == TicTac::X ? TicTac::O : TicTac::X;

    u32 row_cursor = 0;
    u32 col_cursor = 0;

    TicTac::TicTacValue player_turn = TicTac::X;

    std::cout << CLEAR_SCREEN;
    current_board.display(row_cursor,col_cursor);

    while ((current_board.winner() == TicTac::EMPTY) && !current_board.isComplete() ) {

        
        char input = _getch();

        bool should_refresh = false;

        switch (input)
        {
        case 'd':

            if (col_cursor < N-1) {
                 col_cursor++;
                 should_refresh = true;
            }
            break;

        case 'a':

            if (col_cursor > 0) {
                col_cursor--;
                should_refresh = true;
            }
            break;
        
        case 's':

            if (row_cursor < N - 1) {
                row_cursor++;
                should_refresh = true;
            }
            break;  

        case 'w':

            if (row_cursor > 0) {
                row_cursor--;
                should_refresh = true;            
            }
            break;

        case ' ':

            if (current_board.getTile(row_cursor, col_cursor) == TicTac::EMPTY) {

                current_board.setTile( player_turn , row_cursor, col_cursor);
                player_turn = player_turn == TicTac::O ? TicTac::X : TicTac::O;
                should_refresh = true;
            }

        break;
        }


        if (should_refresh) {

            std::cout << CLEAR_SCREEN << std::endl;
            current_board.display(row_cursor, col_cursor);

        }


    }


    TicTac::TicTacValue winner = current_board.winner();
    std::cout << "End Game: " << std::endl;

    if (winner == TicTac::EMPTY) {
        std::cout << "DRAW!" << std::endl;
    }else {

        std::cout << TicTac::to_cstr(winner) << " won !" << std::endl; 
    }

}

void PlayerVsAI() {

    std::cout << "Not implemented yet uWu" << std::endl;
    return;

}


int main() {


    GameMode gameMode = menu();


    switch (gameMode)
    {
    case PVP:
        playerVsPlayer();
        break;
    
    case AI:
        PlayerVsAI();
        break;
    }

    return 0;

}