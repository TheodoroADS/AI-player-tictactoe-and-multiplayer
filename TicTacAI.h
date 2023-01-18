
#pragma once

#include "tictac.h"
#include <assert.h>

using u32 = unsigned int;

class MinMaxAgent {


    struct TicTacNode {
 
        struct TicTacNode* parent;
        const TicTac* content;
        struct TicTacNode** children; // Node takes ownership of the children
        u32 childCount;
        unsigned int score;

        ~TicTacNode() { // Warning : recursive! 

            delete content;

            for (u32 child_idx = 0; child_idx < childCount; child_idx++) {
                delete children[child_idx];
            }

            delete[] children;

        }


    };


    TicTacNode* buildFrom(const TicTac* const current, TicTac::TicTacValue player) {

        TicTacNode* node = new TicTacNode;
        node->content = current;
        TicTac::TicTacValue winner = current->winner();

        if (winner != TicTac::EMPTY) { // End Game
            
            node->score = (winner == current_player) ? 1 : -1;
            node->children = nullptr;
            node->childCount = 0;
            return node;
        }   

        
        TicTac::NextBoardsArray nextBoards = current->buildNextBoards(player);

        if (nextBoards.count == 0) { // the board is full
            node->children = nullptr;
            node->childCount = 0;
            return node;
        }

        node->children = new TicTacNode*[nextBoards.count];
        node->childCount = nextBoards.count;    
        TicTac::TicTacValue nextPlayer = player == TicTac::X ? TicTac::O : TicTac::X;

        for (u32 i = 0 ; i < nextBoards.count ; ++i) {
            TicTacNode* child = buildFrom(nextBoards.nextBoards[i], nextPlayer);
            child->parent = node;
            node->children[i] = child;
            node->score += child->score;
        }

        delete nextBoards.nextBoards;

        return node;

    }   


    TicTac::TicTacValue current_player;

    TicTacNode* solutionTree; // stores the root of the solution tree

    TicTacNode currentState; // store the state the game is in


public:

    MinMaxAgent() = delete;

    explicit MinMaxAgent(const TicTac& initial , TicTac::TicTacValue player) 
    : current_player(player)
    {
        assert(player != TicTac::EMPTY);

        solutionTree = buildFrom(&initial, player);
        currentState = *solutionTree;

    }   

    ~MinMaxAgent() {

        delete solutionTree; // deletes every node on the tree

        // avoid deleting data twice when the destructor for currentState is called
        currentState.content = nullptr;
        currentState.childCount = 0;
        currentState.children = nullptr;

    }

    void inform_decision(const TicTac& updated_state) {

        for (u32 i = 0; i < currentState.childCount; ++i) {

            if (*(currentState.children[i]->content) == updated_state) {

                currentState = *(currentState.children[i]);
                return;

            }

        }   

        assert("Oh not there was a synchronsation issue!" && false);

    }

    u32 take_decision(const TicTac& current_state) { // returns the index of the tile that must be set

        return 0;
    } 


};