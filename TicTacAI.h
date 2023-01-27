
#pragma once

#include "tictac.h"
#include <assert.h>

using u32 = unsigned int;

class MinMaxAgent {


    struct TicTacNode {
 
        struct TicTacNode* parent;
        const TicTac* content;
        struct TicTacNode** children; // Node takes ownership of the children
        unsigned short childCount;
        short score;

        ~TicTacNode() { // Warning : recursive! 

            delete content;

            for (unsigned short child_idx = 0; child_idx < childCount; child_idx++) {
                delete children[child_idx];
            }

            delete[] children;

        }


    };


    TicTacNode* buildFrom(const TicTac* const current, TicTac::TicTacValue player) {

        TicTacNode* node = new TicTacNode;
        node->content = current;
        node->score = 0; // score for a draw
        TicTac::TicTacValue winner = current->winner();

        if (winner != TicTac::EMPTY) { // leaf where someone won
            
            node->score = (winner == current_player) ? 1 : -1;
            node->children = nullptr;
            node->childCount = 0;
            return node;
        }   

        TicTac::NextBoardsArray nextBoards = current->buildNextBoards(player);

        if (nextBoards.count == 0) { // leaf where there was a tie

            node->children = nullptr;
            node->childCount = 0;
            delete[] nextBoards.nextBoards;
            return node;
        }

        node->children = new TicTacNode*[nextBoards.count];
        node->childCount = nextBoards.count;    
        TicTac::TicTacValue nextPlayer = TicTac::oponent(player);

        for (u32 i = 0 ; i < nextBoards.count ; ++i) {

            TicTacNode* child = buildFrom(nextBoards.nextBoards[i], nextPlayer);
            child->parent = node;
            node->children[i] = child;
            
            if (player == current_player) { // Max player

                if (child->score > node->score) {
                    
                    node->score = child->score; 
                }

            } else { // Min player
             
                if (child->score < node->score) {
                    
                    node->score = child->score;
                
                }
            }
        }

        delete[] nextBoards.nextBoards;

        return node;

    }   


    TicTac::TicTacValue current_player;

    TicTacNode* solutionTree; // stores the root of the solution tree

    TicTacNode currentState; // store the state the game is in


public:

    MinMaxAgent() = delete;

    MinMaxAgent(const MinMaxAgent& agent) = delete;

    explicit MinMaxAgent(const TicTac& initial , TicTac::TicTacValue player) 
    : current_player(player)
    {
        assert(player != TicTac::EMPTY);

        solutionTree = buildFrom(new TicTac(initial), TicTac::oponent(player));
        currentState = *solutionTree;

    }   

    ~MinMaxAgent() {

        delete solutionTree; // deletes every node on the tree

        // avoid deleting data twice when the destructor for currentState is called
        currentState.content = nullptr;
        currentState.childCount = 0;
        currentState.children = nullptr;

    }

    void inform_decision(u32 row, u32 col) {

        const u32 index = row * N + col;

        assert(currentState.content->getTile(row,col) == TicTac::EMPTY);

        for (u32 i = 0; i < currentState.childCount; ++i) {

            if (currentState.children[i]->content->getTile(row,col) != TicTac::EMPTY) {

                currentState = *(currentState.children[i]);
                return;
            }

        }   

        assert("Oh not there was a synchronsation issue!" && false);

    }

    u32 take_decision(const TicTac& current_state) { // returns the index of the tile that must be set

        TicTacNode selected_state;


        if (*currentState.content != current_state) {
            
            std::cout << "Bosta" << std::endl;
            currentState.content->display();
            current_state.display();
            assert("Still not synchronised aaaah" && false);

        }

        assert(*currentState.content == current_state);

        TicTac::EmptyTilesArray nextEmptyIdxs =  current_state.findNextEmptyTiles();

        assert(nextEmptyIdxs.count > 0);

        u32 result = nextEmptyIdxs.indexes[0];
        selected_state = *currentState.children[0];

        for (unsigned short child_idx = 0; child_idx < currentState.childCount; ++child_idx) {

            // currentState.children[child_idx]->content->display(0,0);    
            // std::cout << "opa "<< currentState.children[child_idx]->score << std::endl;

            if (currentState.children[child_idx]->score > selected_state.score) {
                
                result = nextEmptyIdxs.indexes[child_idx];
                selected_state = *currentState.children[child_idx];

            }
        } 

        delete[] nextEmptyIdxs.indexes;
        currentState = selected_state;
        return result;
    } 


};