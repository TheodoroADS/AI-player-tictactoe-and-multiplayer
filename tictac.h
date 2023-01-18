#pragma once 

#include <string>
#include <array>
#include <iostream>
#include <optional>
#include <utility>

#define N 3

using u32 = unsigned int;
using cu32 = const unsigned int;

class TicTac {

public:
    enum TicTacValue {
        EMPTY = 0,
        X = 'X',
        O = 'O'
    };

    static const char* to_cstr(TicTacValue value) {
        switch (value)
        {
        case EMPTY:
            return " ";
        
        case X:
            return "X";
        case O: 
            return "O";
        default:
            return "WTF";
        }
    }


    static inline std::string to_string(TicTacValue value) {
        return std::string(to_cstr(value));
    }

private:

    std::array<TicTacValue , N*N> tiles;



public:


    TicTac() {
        tiles.fill(EMPTY);
    }

    TicTac(TicTac&& other) = default;
    
    TicTac& operator=(TicTac&& other) = default;
    
    TicTac(const std::array<TicTacValue, N*N>& other) {
        
        for ( u32 i = 0 ; i < N*N ; ++i) {
            tiles[i] = other[i];
        }   

    }

    TicTac(const TicTac& other) : TicTac(other.tiles) {}

    TicTac(const TicTac&other , const u32 index , const TicTacValue value) 
    : tiles(other.tiles)
    {
        tiles[index] = value;
    }
    
    inline void setTile(TicTacValue value , u32 row, u32 col) {
        tiles[row * N + col] = value;
    }

    inline TicTacValue getTile(u32 row, u32 col) const { // readonly function
        return tiles[row * N + col];
    }

    inline TicTacValue& operator()(u32 row, u32 col) {
        return tiles[row * N + col];
    }


    void display(u32 row_cursor , u32 col_cursor) const {

            for(u32 i = 0 ; i < N ; i++){

                for(u32 j = 0; j< N ; j++){
                    
                    if (i == row_cursor && j == col_cursor) {
                    
                        std::cout <<  "| "  << "[" << to_cstr(getTile(i,j))<< "]" <<  " " ;
                    
                    } else {

                        std::cout <<  "|  "  <<  to_cstr(getTile(i,j)) <<  "  " ;
                    }
                } 
            
            std::cout <<  "|\n";

        }

        std::cout << std::endl;
    
    }


    // std::string toString() const{


    //     std::string res = "\n ___ ___ ___\n";

    //     for(int i = 0 ; i < 3 ; i++){
            

    //         for(int j = 0; j< 3 ; j++){
                
    //             res += "| " + to_string(getTile(i,j)) + std::string(" ");
    //         }
    //         res += "|\n";
    //     } 

    // return res;
    
    // }


    inline bool operator==(const TicTac& other) const {
        return tiles == other.tiles;
    } 

    bool isComplete() const {
        
        for (const TicTacValue tile : tiles) {
            if (tile == EMPTY) return false;
        }

        return true;
    }   


    TicTacValue winner() const {

        for (u32 u = 0 ; u < N ; ++u) {

            TicTacValue firstInRow = getTile(u, 0);
            TicTacValue firstInCol = getTile(0,u);
            bool won = true;

            if (firstInRow != EMPTY) {


                for (u32 v = 1 ; v < N ; ++v) {

                    if (firstInRow != getTile(u, v)) {
                        won = false;
                        break;
                    }

                }

                if (won) return firstInRow;
    
            } 


            if (firstInCol != EMPTY) {
                
                won = true;

                for (u32 v = 1; v < N ; ++v) {

                    if (firstInCol != getTile(v,u)) {
                        won = false;
                        break;
                    }

                }

                if (won) return firstInCol;

            }
        }

        TicTacValue firstInFirstDiag = getTile(0,0);
        bool won = true;

        if (firstInFirstDiag != EMPTY) {

            for (u32 i = 1 ; i < N ; ++i) {
                
                if (getTile(i,i) != firstInFirstDiag) {
                    won = false;
                    break;
                }

            }
            if (won) return firstInFirstDiag;
        }



        u32 row = N - 1;
        u32 col = 0;
        TicTacValue firstInSecondDiag = getTile(row,col);
        
        won = true;

        if (firstInSecondDiag != EMPTY) {

            for (u32 i = 1 ; i < N ; ++i) {
                
                if (getTile(row - i, col + i) != firstInSecondDiag) {
                    won = false;
                    break;
                }

            }

            if (won) return firstInSecondDiag;
            
        }

        return EMPTY;
   
    }

    struct EmptyTilesArray {
        u32* indexes;
        u32 count;
    };

    EmptyTilesArray findNextEmptyTiles() const {
        
        u32 count = 0;
        u32 buffer[N];
        u32* result;

        for (u32 i = 0; i < N*N ; ++i) {

            if (tiles[i] == EMPTY) 
                buffer[count++] = i;
        }

        if (count > 0) {

            result = new u32[count];
            memcpy(result, buffer, count * sizeof(u32));

        } else {

            result = nullptr;

        }

        return {result, count};
        
    }

    struct NextBoardsArray {

        TicTac** nextBoards;
        u32 count;
    };

    NextBoardsArray buildNextBoards(TicTacValue player) const {

        EmptyTilesArray emptyTiles = findNextEmptyTiles();

        if (emptyTiles.count == 0) return {nullptr , 0};

        TicTac** nextBoards = new TicTac*[emptyTiles.count];

        for (u32 i = 0; i < emptyTiles.count; i++) {

            nextBoards[i] = new TicTac(*this, emptyTiles.indexes[i], player);
        }

        return {nextBoards, emptyTiles.count};

    }




};