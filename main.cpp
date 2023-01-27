#include <iostream>
#include <string>
#include <conio.h>
#include "tictac.h"
#include "TicTacAI.h"
#include <signal.h>

#define CLEAR_SCREEN "\x1B[2J\x1B[1;1H"



void segvHandler( int s ) // segmentation fault handlers
{
  printf( "Segmentation Fault! AHHHHHHHHHHHHHHHH \n" );
  exit( EXIT_FAILURE );
}


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
    TicTac::TicTacValue other_player = TicTac::oponent(player);

    u32 row_cursor = 0;
    u32 col_cursor = 0;

    TicTac::TicTacValue player_turn = player;

    std::cout << CLEAR_SCREEN;
    std::cout << "Press esc to quit" << std::endl;
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
                player_turn = TicTac::oponent(player_turn);
                should_refresh = true;
            }

        break;

        case 27 : // esc key

            std::cout << CLEAR_SCREEN << "Exited game" << std::endl;
            return;

        break;

        }


        if (should_refresh) {

            std::cout << CLEAR_SCREEN << std::endl;
            std::cout << "Press esc to quit" << std::endl;
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

    TicTac current_board;
    const TicTac::TicTacValue player = TicTac::X;
    const TicTac::TicTacValue other_player = TicTac::O;

    u32 row_cursor = 0;
    u32 col_cursor = 0;

    TicTac::TicTacValue player_turn = player;

    // agent is heap allocated on purpose, I want it to live for as long as the program runs and not to call its destructor
    // because cleaning up is such an expensive operation
    MinMaxAgent& agent = *(new MinMaxAgent(current_board, other_player));

    std::cout << CLEAR_SCREEN;
    std::cout << "Press esc to quit" << std::endl;
    current_board.display(row_cursor,col_cursor);

    while ((current_board.winner() == TicTac::EMPTY) && !current_board.isComplete() ) {

        if (player_turn == other_player) {

            u32 decision = agent.take_decision(current_board);

            // std::cout << "Decision " << decision << std::endl;

            // std::cout.flush();

            current_board.setTile(other_player, decision);

            player_turn = player;
            
            std::cout << CLEAR_SCREEN << "Press esc to quit" << std::endl;
            current_board.display(row_cursor, col_cursor);


        } else {


            char input = _getch();

            bool should_refresh = false;
            bool state_changed = false;

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

                        current_board.setTile( player , row_cursor, col_cursor);
                        should_refresh = true;
                        state_changed = true;
                    }

                break;

                case 27 : // esc key

                    std::cout << CLEAR_SCREEN << "Exited game" << std::endl;
                    return;

                break;

            }

            if (should_refresh) {

                std::cout << CLEAR_SCREEN << std::endl;
                std::cout << "Press esc to quit" << std::endl;
                current_board.display(row_cursor, col_cursor);
                
                if (state_changed) { 
                    agent.inform_decision(row_cursor, col_cursor);
                    player_turn = other_player;
                }

            }
            
        
        }


    }


    TicTac::TicTacValue winner = current_board.winner();
    std::cout << "End Game: " << std::endl;

    if (winner == TicTac::EMPTY) {
        std::cout << "DRAW!" << std::endl;
    }else {

        std::cout << TicTac::to_cstr(winner) << " won !" << std::endl; 
    }

    std::cout << "Cleaning up..." << std::endl; 

}


int main() {

    signal( SIGSEGV, segvHandler ); // helps to know if the error that occured is a segmentation fault (since powershell won't tell me) 

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