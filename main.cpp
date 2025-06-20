#include <string>
#include <tuple>
#include <iostream>
#include "Game.hpp"

using namespace std;

int main() {
    string input = "";
    Game game;
    
    while (true) {
        getline(cin, input);
        if (!cin) {
            exit(0);
        } else {
            game.detectCommand(input);
        }
    }
    return 0;
}
