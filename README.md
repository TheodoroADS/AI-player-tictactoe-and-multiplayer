# Tic-Tac-Toe Minimax Player vs. AI and Player vs. Player console game

Ever connected to a remote terminal and felt like playing a little game? How about spending some time alone of with a friend playing a little game of tic-tac-toe?

This is a simple tic-tac-toe game with two game modes : 

- **Player vs Player** where each player plays against each other in turns
- **Player vs Machine** where a single player can play with against an AI running a weaker version of the MiniMax Algorithm (otherwise it would be impossible to win against it)


![screenshot of the game](/images/screenshot1.jpg)
  
![another screenshot of the game](/images/screenshot2.jpg)


## Quick start 

> If precompiled, run de executable.

> Else , in the project's directory, compile everything with a C++17 compatible compiler. Eg.

``` sh 
clang++ -std=c++17 -o tictactoe.exe main.cpp
# or
gcc++ -std=c++17 -o tictactoe.exe main.cpp
# or whichever compiler you prefer
```
Of course, if you are not in Windows you can omit the .exe extension from the executable name.


## About

This is a small project that I did to practice C++. I avoided separating the code in multiple .cpp files to make it easier to compile, since this is such a small project - So everything is .h header files.

