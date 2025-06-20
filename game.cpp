#include "Game.hpp"

Game::Game() {
    whitePawnsNumb = 0;
    blackPawnsNumb = 0;
    reserveWhitePawnsNumb = 0;
    reserveBlackPawnsNumb = 0;
    whoseTurn = 0;
    pawnsNumb = 0;
    pawnsCaptureNumb = 0;
    boardSize = 0;
    arraySize = 0;
    captureStart = "";
    captrueEnd = "";
    captureColor = "";
}

// clear the board
void Game::emptyBoard() {
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            this->board[i][j] = -1;
        }
    }
}

// check if board is empty
bool Game::isEmptyBoard() {
    arraySize = boardSize * 2 - 1;
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            if (board[i][j] > -1) {
                return true;
            }
        }
    }
    return false;
}

// check if pawns number on board and in reserve add up to correct value
void Game::checkIfPawnsNumberIsCorrect() {
    int currentWhite = 0, currentBlack = 0;
    
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            if (board[i][j] == 1) {
                currentWhite++;
            } else if (board[i][j] == 2) {
                currentBlack++;
            }
        }
    }
    existingChains = findChains(pawnsCaptureNumb);
    existingChains = removeContainedVectors();
    if (currentBlack + reserveBlackPawnsNumb > blackPawnsNumb) {
        cout << "WRONG_BLACK_PAWNS_NUMBER" << endl;
        emptyBoard();
    } else if (currentWhite + reserveWhitePawnsNumb > whitePawnsNumb) {
        cout << "WRONG_WHITE_PAWNS_NUMBER" << endl;
        emptyBoard();
    } else if (existingChains.size() > 0) {
        if (existingChains.size() > 1) {
            cout << "ERROR_FOUND_" << existingChains.size() << "_ROWS_OF_LENGTH_K" << endl;
            emptyBoard();
        } else {
            cout << "ERROR_FOUND_" << existingChains.size() << "_ROW_OF_LENGTH_K" << endl;
            emptyBoard();
        }
    } else {
        cout << "BOARD_STATE_OK" << endl;
    }
}

// check if parameters provided during loading of board are correct
void Game::checkIfParametersAreCorrect(tuple<int, int, int, int> parameters) {
    boardSize = get<0>(parameters);
    int collectPawnNumber = get<1>(parameters);
    int whitePawns = get<2>(parameters);
    int blackPawns = get<3>(parameters);
    
    if (boardSize < 2) {
        throw "Board is too small";
    }
    
    if (collectPawnNumber < 2 || collectPawnNumber > 2 * boardSize - 1) {
        throw "Wrong number of collect pawns";
    }
    
    if (whitePawns < 3 || blackPawns < 3) {
        throw "Too low number of pawns in either side";
    }
}

// set the board
void Game::setBoard(const vector<vector<int>>& board) {
    // clear the existing board vector
    this->board.clear();
    
    // resize existing board vector to match the size of the one provided as argument
    this->board.resize(board.size(), vector<int>(board[0].size()));
    
    // copy elements from the input board to the new vector
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            this->board[i][j] = board[i][j];
        }
    }
}

// calculate length of each row of array
void Game::calculateRowLengths() {
    arraySize = boardSize * 2 - 1;
    vector<int> rowLengths(arraySize, 0);
    int median = arraySize / 2;
    rowLengths[median] = arraySize;
    
    // add values to the left and right until all values are not 0
    int value = arraySize - 1;
    for (int i = 1; i <= median; i++) {
        if (rowLengths[median - i] == 0) {
            rowLengths[median - i] = value;
        }
        if (rowLengths[median + i] == 0) {
            rowLengths[median + i] = value;
        }
        value--;
    }
    
    this->rowLengthsVect = rowLengths;
}

// function to read a board
void Game::readBoard() {
    string input;
    getline(cin, input);
    tuple<int, int, int, int> parameters = readInput(input);
    getline(cin, input);
    readState(input);
    this->boardSize = get<0>(parameters);
    checkIfParametersAreCorrect(parameters);
    this->arraySize = 2 * boardSize - 1;
    vector<vector<int>> board(arraySize, vector<int>(arraySize));
    calculateRowLengths();
    for (int i = 0; i < arraySize; i++) {
        int count = 0;
        string input;
        getline(cin, input);
        input = formatInput(input);
        if (input.length() != rowLengthsVect[i]) {
            cout << "WRONG_BOARD_ROW_LENGTH" << endl;
            emptyBoard();
            return;
        }
        for (int j = 0; j < arraySize; j++) {
            if (i + j < arraySize / 2 || i + j > arraySize * 2 - 2 - arraySize / 2) {
                board[i][j] = -1;
            } else {
                board[i][j] = input[count];
                if (input[count] == 95) {
                    board[i][j] = 0;
                } else if (input[count] == 87) {
                    board[i][j] = 1;
                } else if (input[count] == 66) {
                    board[i][j] = 2;
                }
                count++;
            }
        }
    }
    setBoard(board);
    checkIfPawnsNumberIsCorrect();
    createFieldIndexes();
}

// function to format the input so it does not contain any spaces
string Game::formatInput(string input) {
    string result;
    for (char c : input) {
        if (c != ' ') {
            result += c;
        }
    }
    return result;
}

// function to read the parameters of the board
tuple<int, int, int, int> Game::readInput(string input) {
    int whereFirstSpace = input.find(" ");
    
    string boardDimensionsString = input.substr(0, whereFirstSpace);
    int boardDim = stoi(boardDimensionsString);
    
    string secondString = input.substr(whereFirstSpace + 1, input.length());
    int whereSecondSpace = secondString.find(" ");
    
    string pawnNumberString = secondString.substr(0, whereSecondSpace);
    int pawnNumber = stoi(pawnNumberString);
    
    string thirdString = secondString.substr(whereSecondSpace + 1, secondString.length());
    int whereThirdSpace = thirdString.find(" ");
    
    string whitePawnNumberString = thirdString.substr(0, whereThirdSpace);
    int whitePawnNumb = stoi(whitePawnNumberString);
    
    string blackPawnNumberString = thirdString.substr(whereThirdSpace + 1, thirdString.length());
    int blackPawnNumb = stoi(blackPawnNumberString);
    
    whitePawnsNumb = whitePawnNumb;
    blackPawnsNumb = blackPawnNumb;
    pawnsCaptureNumb = pawnNumber;
    return make_tuple(boardDim, pawnNumber, whitePawnNumb, blackPawnNumb);
}

// function to read the number of pawns in reserve and whose turn is it
tuple<int, int, int> Game::readState(string input) {
    int whereFirstSpace = input.find(" ");
    
    string whiteString = input.substr(0, whereFirstSpace);
    int whiteInt = stoi(whiteString);
    
    string secondString = input.substr(whereFirstSpace + 1, input.length());
    int whereSecondSpace = secondString.find(" ");
    
    string blackString = secondString.substr(0, whereSecondSpace);
    int blackInt = stoi(blackString);
    
    string thirdString = secondString.substr(whereSecondSpace + 1, secondString.length());
    int turn = 0;
    if (thirdString == "W") {
        turn = 1;
    } else {
        turn = 2;
    }
    
    reserveBlackPawnsNumb = blackInt;
    reserveWhitePawnsNumb = whiteInt;
    whoseTurn = turn;
    return make_tuple(whiteInt, blackInt, turn);
}

// function to detect what command was inputed and run following command
void Game::detectCommand(string input) {
    if (input == "LOAD_GAME_BOARD") {
        readBoard();
    } else if (input == "PRINT_GAME_BOARD") {
        drawBoard();
    } else if (input.find("DO_MOVE") != string::npos) {
        string moveString = input.substr(input.find("DO_MOVE") + 8);
        
        // split the moveString using the dash as a delimiter
        stringstream ss(moveString);
        string val1, val2, val3;
        getline(ss, val1, '-');
        getline(ss, val2, ' ');
        getline(ss, val3);
        if (!val3.empty()) {
            // split the val2 string using the colon as a delimiter
            stringstream ss2(val3);
            getline(ss2, captureColor, ':');
            getline(ss2, captureStart);
            captureStart = formatInput(captureStart);
            captrueEnd = captureStart.substr(2, 4);
            captureStart = captureStart.substr(0, 2);
        }
        makeMove(val1, val2);
    } else if (input == "BOARD") {
        drawEverything();
    } else if (input == "GEN_ALL_POS_MOV_NUM") {
        vector<pair<string, string>> possibleMoves = generatePossibleMoves();
        cout << possibleMoves.size() << "_UNIQUE_MOVES" << endl;
    } else if (input == "IS_GAME_OVER") {
        checkIfGameIsOver();
    }
}

// function to draw the board
void Game::drawBoard() {
    if (!isEmptyBoard()) {
        cout << "EMPTY_BOARD" << endl;
        return;
    }
    char whoseTurnChar = '\0';
    if (whoseTurn == 1) {
        whoseTurnChar = 'W';
    } else if (whoseTurn == 2) {
        whoseTurnChar = 'B';
    }
    cout << boardSize << " " << pawnsCaptureNumb << " " << whitePawnsNumb << " " << blackPawnsNumb << endl;
    cout << reserveWhitePawnsNumb << " " << reserveBlackPawnsNumb << " " << whoseTurnChar << endl;
    for (int i = 0; i < arraySize; i++) {
        if (i > arraySize / 2) {
            for (int j = 0; j < i - boardSize + 1; j++) {
                cout << " ";
            }
        }
        for (int j = 0; j < arraySize; j++) {
            if (board[i][j] == -1) {
                cout << " ";
            } else if (board[i][j] == 0) {
                cout << "_ ";
            } else if (board[i][j] == 1) {
                cout << "W ";
            } else if (board[i][j] == 2) {
                cout << "B ";
            }
        }
        cout << endl;
    }
}

// function to calculate lengths of each row of field indexes
void Game::calculateFieldsRowLengths() {
    int fieldSize = boardSize * 2 + 1;
    vector<int> fieldLengths(fieldSize, 0);
    // find median value
    int median = fieldSize / 2;
    // set length of median row of fields to be equal to fieldSize
    fieldLengths[median] = fieldSize;
    
    int value = fieldSize - 1;
    // set neighbouring values to be smaller by 1 than the previous value until we reach 0
    for (int i = 1; i <= median; i++) {
        if (fieldLengths[median - i] == 0) {
            fieldLengths[median - i] = value;
        }
        if (fieldLengths[median + i] == 0) {
            fieldLengths[median + i] = value;
        }
        value--;
    }
    
    this->fieldsRowLengthsVect = fieldLengths;
}

// function to create array with indexes
void Game::createFieldIndexes() {
    calculateFieldsRowLengths();
    int fieldsSize = 2 * boardSize + 1;
    int letterCount = 0, count = 0;
    string letter = "";
    vector<vector<string>> fieldIndexes(fieldsSize, vector<string>(fieldsSize, ""));
    unordered_map<string, int> letterCounts;
    for (int i = 0; i < fieldsSize; i++) {
        count = fieldsRowLengthsVect[i];
        letterCount = 0;
        if (i > fieldsSize / 2) {
            letterCount = fieldsSize - fieldsRowLengthsVect[i];
        }
        for (int j = 0; j < fieldsSize; j++) {
            if (fieldsSize - count > 0) {
                fieldIndexes[i][j] = " ";
                count++;
            } else {
                letter = 97 + letterCount++;
                letterCounts[letter] += 1;
            }
        }
    }
    for (int i = 0; i < fieldsSize; i++) {
        count = fieldsRowLengthsVect[i];
        letterCount = 0;
        if (i > fieldsSize / 2) {
            letterCount = fieldsSize - fieldsRowLengthsVect[i];
        }
        for (int j = 0; j < fieldsSize; j++) {
            if (fieldsSize - count > 0) {
                fieldIndexes[i][j] = " ";
                count++;
            } else {
                letter = 97 + letterCount++;
                auto letterValue = letterCounts.find(letter);
                string index = to_string(letterValue->second);
                letterCounts[letter] -= 1;
                fieldIndexes[i][j] = letter + index;
            }
        }
    }
    this->fieldIndexes = fieldIndexes;
}

// function to check if there exist index provided as argument in the indexes array
bool Game::checkIfIndexIsCorrect(const string& index) {
    int fieldsSize = 2 * boardSize + 1;
    for (int i = 0; i < fieldsSize; i++) {
        for (int j = 0; j < fieldsSize; j++) {
            if (fieldIndexes[i][j] == index) {
                return true;
            }
        }
    }
    return false;
}

// function to check whether there is an empty spot in neighborhood of the index on position i,j
bool Game::emptyNeighbor(int i, int j) {
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    for (int k = 0; k < 4; k++) {
        if (fieldIndexes[i + dx[k]][j + dy[k]] == " ") {
            return true;
        }
    }
    return false;
}

// function to check if there exist index provided as argument in the indexes array and if it is valid starting point
bool Game::checkIfStartIndexIsCorrect(const string& index) {
    int fieldsSize = 2 * boardSize + 1;
    int foundI = 0;
    int foundJ = 0;
    for (int i = 0; i < fieldsSize; i++) {
        for (int j = 0; j < fieldsSize; j++) {
            if (fieldIndexes[i][j] == index) {
                foundI = i;
                foundJ = j;
                break;
            }
        }
    }
    
    if (foundI == 0 || foundI == fieldsSize - 1 || foundJ == 0 || foundJ == fieldsSize - 1 || emptyNeighbor(foundI, foundJ)) {
        return true;
    }
    return false;
}

// function to check if there exist index provided as argument in the indexes array and if it is valid ending point
bool Game::checkIfEndIndexIsCorrect(const string& index) {
    int fieldsSize = 2 * boardSize + 1;
    int foundI = 0;
    int foundJ = 0;
    for (int i = 0; i < fieldsSize; i++) {
        if (foundI != 0) break;
        for (int j = 0; j < fieldsSize; j++) {
            if (fieldIndexes[i][j] == index) {
                foundI = i;
                foundJ = j;
                break;
            }
        }
        
    }
    if (foundI != 0 && foundI != fieldsSize - 1 && foundJ != 0 && foundJ != fieldsSize - 1 && !emptyNeighbor(foundI, foundJ)) {
        return true;
    }
    return false;
}

// function to check if the starting index and ending index arent to far away from each other
bool Game::checkIfDirectionIsCorrect(string startIndex, string endIndex) {
    char startLetter = startIndex[0];
    char endLetter = endIndex[0];
    int startNumber = startIndex[1];
    int endNumber = endIndex[1];
    return abs(startLetter - endLetter) < 2 && abs(startNumber - endNumber) < 2;
}

// function to find direction in which the move will be executed
string Game::findMoveDirection(const string& startIndex, const string& endIndex) {
    string result = "";
    int foundI = -1, foundJ = -1, foundEndI = -1, foundEndJ = -1;
    
    for (int i = 0; i < arraySize + 2; i++) {
        if (foundI != -1 && foundEndI != -1) break;
        for (int j = 0; j < arraySize + 2; j++) {
            if (fieldIndexes[i][j] == startIndex) {
                foundI = i;
                foundJ = j;
            } else if (fieldIndexes[i][j] == endIndex) {
                foundEndI = i;
                foundEndJ = j;
            }
        }
    }
    // i - kolumna
    // j - rzad
    if (foundEndI == foundI && foundEndJ < foundJ) {
        result = "LEFT";
    } else if (foundEndI == foundI && foundEndJ > foundJ) {
        result = "RIGHT";
    } else if (foundEndJ == foundJ && foundEndI > foundI) {
        result = "DOWN_RIGHT";
    } else if (foundEndJ < foundJ && foundEndI > foundI) {
        result = "DOWN_LEFT";
    } else if (foundEndJ == foundJ && foundEndI < foundI) {
        result = "TOP_RIGHT";
    } else if (foundEndJ < foundJ && foundEndI < foundI) {
        result = "TOP_LEFT";
    }
    
    return result;
}

// function to make move based on the start and end indexes
void Game::makeMove(const string& startIndex, const string& endIndex) {
    string direction = "";
    // check if start and end indexes are both valid to use
    if (!checkIfIndexIsCorrect(startIndex)) {
        cout << "BAD_MOVE_" << startIndex << "_IS_WRONG_INDEX" << endl;
        return;
    } else if (!checkIfIndexIsCorrect(endIndex)) {
        cout << "BAD_MOVE_" << endIndex << "_IS_WRONG_INDEX" << endl;
        return;
    } else if (!checkIfStartIndexIsCorrect(startIndex)) {
        cout << "BAD_MOVE_" << startIndex << "_IS_WRONG_STARTING_FIELD" << endl;
        return;
    } else if (!checkIfEndIndexIsCorrect(endIndex)) {
        cout << "BAD_MOVE_" << endIndex << "_IS_WRONG_DESTINATION_FIELD" << endl;
        return;
    } else if (!checkIfDirectionIsCorrect(startIndex, endIndex)) {
        cout << "UNKNOWN_MOVE_DIRECTION" << endl;
        return;
    }
    
    direction = findMoveDirection(startIndex, endIndex);
    vector<vector<int>> tempBoard = board;
    
    if (direction == "LEFT") {
        moveLeft(endIndex, false);
    } else if (direction == "RIGHT") {
        moveRight(endIndex, false);
    } else if (direction == "TOP_LEFT") {
        moveTopLeft(endIndex, false);
    } else if (direction == "TOP_RIGHT") {
        moveTopRight(endIndex, false);
    } else if (direction == "DOWN_RIGHT") {
        moveDownRight(endIndex, false);
    } else if (direction == "DOWN_LEFT") {
        moveDownLeft(endIndex, false);
    }
    
    if (board != tempBoard) {
        finishTurn();
    }
}

// function that finishes turn
void Game::finishTurn() {
    // White - 1
    // Black - 2
    
    // swap turn and take one pawn from reserve
    if (whoseTurn == 1) {
        reserveWhitePawnsNumb--;
        whoseTurn = 2;
    } else if (whoseTurn == 2) {
        reserveBlackPawnsNumb--;
        whoseTurn = 1;
    }
    
    // find if there exist any chains of length required to capture
    existingChains = findChains(pawnsCaptureNumb);
    existingChains = removeContainedVectors();
    if (existingChains.size() > 0) {
        capturePieces();
    }
    
    cout << "MOVE_COMMITTED" << endl;
}

void Game::drawEverything() {
    for (int i = 0; i < arraySize; i++) {
        cout << rowLengthsVect[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < arraySize + 2; i++) {
        for (int j = 0; j < arraySize + 2; j++) {
            cout << fieldIndexes[i][j] << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// function to find the coordinates of the index on the board
pair<int, int> Game::findIndexOnBoard(const string& index) {
    int fieldsSize = 2 * boardSize + 1;
    for (int i = 1; i < fieldsSize - 1; i++) {
        for (int j = 1; j < fieldsSize - 1; j++) {
            if (fieldIndexes[i][j] == index) {
                int x = arraySize - rowLengthsVect[i - 1];
                int x2 = 1;
                if (i > fieldsSize / 2) {
                    x2 += x;
                }
                return make_pair(i - 1, j - x2);
            }
        }
    }
    return make_pair(-1, -1);
}

// function to move to the left
void Game::moveLeft(const string& startIndex, bool trying) {
    pair<int, int> coords = findIndexOnBoard(startIndex);
    int x = coords.first;
    int y = coords.second;
    int firstY = y;
    int currentColor = whoseTurn;
    int nextColor = board[x][y];
    
    // move to the left until we dont run into end of board or an empty spot on board
    while (firstY > 0 && board[x][firstY] > 0) {
        firstY--;
    }
    // check if left-most is different than 0 which indicates that the row is full
    if (board[x][firstY] != 0) {
        if (!trying) {
            cout << "BAD_MOVE_ROW_IS_FULL" << endl;
        }
        return;
    }
    board[x][y] = whoseTurn;
    // iterate until we didnt reach the end board
    while (y > 0 && board[x][y] != -1 && nextColor != 0) {
        currentColor = nextColor;
        nextColor = board[x][y - 1];
        y--;
        board[x][y] = currentColor;
    }
}

// function to move to the right
void Game::moveRight(const string& startIndex, bool trying) {
    pair<int, int> coords = findIndexOnBoard(startIndex);
    int currentColor = whoseTurn;
    int x = coords.first;
    int y = coords.second;
    int firstY = y;
    int nextColor = board[x][y];
    
    while (firstY < arraySize - 1 && board[x][firstY] > 0) {
        firstY++;
    }
    if (board[x][firstY] != 0) {
        if (!trying) {
            cout << "BAD_MOVE_ROW_IS_FULL" << endl;
        }
        return;
    }
    board[x][y] = whoseTurn;
    // iterate until we didnt reach the end board
    while (y < arraySize - 1 && board[x][y] != -1 && nextColor != 0) {
        currentColor = nextColor;
        nextColor = board[x][y + 1];
        y++;
        this->board[x][y] = currentColor;
    }
}

// function to move in the top-left direction
void Game::moveTopLeft(const string& startIndex, bool trying) {
    pair<int, int> coords = findIndexOnBoard(startIndex);
    int currentColor = whoseTurn;
    int x = coords.first;
    int y = coords.second;
    int firstY = y;
    int firstX = x;
    int nextColor = board[x][y];
    
    while (firstX > 0 && board[firstX][firstY] > 0) {
        firstX--;
    }
    if (board[firstX][y] != 0) {
        if (!trying) {
            cout << "BAD_MOVE_ROW_IS_FULL" << endl;
        }
        return;
    }
    board[x][y] = whoseTurn;
    // iterate until we didnt reach the end board
    while (x > 0 && board[x][y] != -1 && nextColor != 0) {
        currentColor = nextColor;
        nextColor = board[x - 1][y];
        x--;
        this->board[x][y] = currentColor;
    }
}

// function to move in top-right direction
void Game::moveTopRight(const string& startIndex, bool trying) {
    pair<int, int> coords = findIndexOnBoard(startIndex);
    int x = coords.first;
    int y = coords.second;
    int firstY = y;
    int firstX = x;
    int currentColor = whoseTurn;
    int nextColor = board[x][y];
    
    while (firstY < arraySize - 1 && firstX > 0 && board[firstX][firstY] > 0) {
        firstY++;
        firstX--;
    }
    if (board[firstX][firstY] != 0) {
        if (!trying) {
            cout << "BAD_MOVE_ROW_IS_FULL" << endl;
        }
        return;
    }
    board[x][y] = whoseTurn;
    // iterate until we didnt reach the end board
    while (y < arraySize - 1 && x > 0 && board[x][y] != -1 && nextColor != 0) {
        currentColor = nextColor;
        nextColor = board[x - 1][y + 1];
        y++;
        x--;
        this->board[x][y] = currentColor;
    }
}

// function to move in the down-left direction
void Game::moveDownLeft(const string& startIndex, bool trying) {
    pair<int, int> coords = findIndexOnBoard(startIndex);
    int x = coords.first;
    int y = coords.second;
    int firstY = y;
    int firstX = x;
    int currentColor = whoseTurn;
    int nextColor = board[x][y];
    
    while (firstY > 0 && firstX < arraySize - 1 && board[firstX][firstY] > 0) {
        firstY--;
        firstX++;
    }
    if (board[firstX][firstY] != 0) {
        if (!trying) {
            cout << "BAD_MOVE_ROW_IS_FULL" << endl;
        }
        return;
    }
    board[x][y] = whoseTurn;
    // iterate until we didnt reach the end board
    while (x < arraySize - 1 && y > 0 && board[x][y] != -1 && nextColor != 0) {
        currentColor = nextColor;
        nextColor = board[x + 1][y - 1];
        x++;
        y--;
        this->board[x][y] = currentColor;
    }
}

// function to move in the down-right direction
void Game::moveDownRight(const string& startIndex, bool trying) {
    pair<int, int> coords = findIndexOnBoard(startIndex);
    int x = coords.first;
    int y = coords.second;
    int firstY = y;
    int firstX = x;
    int currentColor = -1;
    int nextColor = board[x][y];
    
    while (firstX < arraySize - 1 && board[firstX][firstY] > 0) {
        firstX++;
    }
    if (board[firstX][y] != 0) {
        if (!trying) {
            cout << "BAD_MOVE_ROW_IS_FULL" << endl;
        }
        return;
    }
    board[x][y] = whoseTurn;
    // iterate until we didnt reach the end board
    while (x < arraySize - 1 && board[x][y] != -1 && nextColor != 0) {
        currentColor = nextColor;
        nextColor = board[x + 1][y];
        x++;
        this->board[x][y] = currentColor;
    }
}

// function to capture pieces based on existing chains on board
void Game::capturePieces() {
    if (captureStart != "") {
        pair<int, int> pos1 = findIndexOnBoard(captureStart);
        pair<int, int> pos2 = findIndexOnBoard(captrueEnd);
        int x1 = pos1.first;
        int x2 = pos2.first;
        int y1 = pos1.second;
        int y2 = pos2.second;
        int color = board[y1][x1];
        // find direction of capture
        if (x1 < x2 && y1 == y2) {
            captureRight(color, pos1);
        } else if (x1 > x2 && y1 == y2) {
            captureLeft(color, pos1);
        } else if (x1 > x2 && y1 < y2) {
            captureDownLeft(color, pos1);
        } else if (x1 == x2 && y2 > y1) {
            captureDownRight(color, pos1);
        } else if (x1 == x2 && y2 < y1) {
            captureTopRight(color, pos1);
        } else if (x1 < x2 && y1 < y2) {
            captureTopLeft(color, pos1);
        }
        return;
    }
    
    for (int i = 0; i < existingChains.size(); i++) {
        vector<pair<int, int>> chain = existingChains[i];
        pair<int, int> pos1 = chain[0];
        pair<int, int> pos2 = chain[1];
        int x1 = pos1.first;
        int x2 = pos2.first;
        int y1 = pos1.second;
        int y2 = pos2.second;
        int color = board[y1][x1];
        // find direction of capture
        if (x1 < x2 && y1 == y2) {
            captureRight(color, pos1);
        } else if (x1 > x2 && y1 == y2) {
            captureLeft(color, pos1);
        } else if (x1 > x2 && y1 < y2) {
            captureDownLeft(color, pos1);
        } else if (x1 == x2 && y2 > y1) {
            captureDownRight(color, pos1);
        } else if (x1 == x2 && y2 < y1) {
            captureTopRight(color, pos1);
        } else if (x1 < x2 && y1 < y2) {
            captureTopLeft(color, pos1);
        }
    }
}

// function to capture in the right direction
void Game::captureRight(int capturingColor, pair<int, int> index) {
    int x = index.second;
    int y = index.first;
    // move to the most left index on board
    while (y > 0 && board[x][y - 1] != -1) {
        y--;
    }
    // move to the right and capture any pawns
    while (y < arraySize - 1 && board[x][y] > 0) {
        int color = board[x][y];
        if (color == capturingColor) {
            if (capturingColor == 1) {
                reserveWhitePawnsNumb++;
            } else if (capturingColor == 2) {
                reserveBlackPawnsNumb++;
            }
        }
        this->board[x][y] = 0;
        y++;
    }
    if (board[x][y] == capturingColor) {
        if (capturingColor == 1) {
            reserveWhitePawnsNumb++;
        } else if (capturingColor == 2) {
            reserveBlackPawnsNumb++;
        }
    }
    this->board[x][y] = 0;
}

// function to capture in the left direction
void Game::captureLeft(int capturingColor, pair<int, int> index) {
    int x = index.first;
    int y = index.second;
    while (y < boardSize - 1 && board[x][y] != -1) {
        y++;
    }
    while (y > 0 && board[x][y] != -1) {
        int color = board[x][y];
        if (color == capturingColor) {
            if (capturingColor == 1) {
                reserveWhitePawnsNumb++;
            } else if (capturingColor == 2) {
                reserveBlackPawnsNumb++;
            }
        }
        this->board[x][y] = 0;
        y--;
    }
    if (board[x][y] == capturingColor) {
        if (capturingColor == 1) {
            reserveWhitePawnsNumb++;
        } else if (capturingColor == 2) {
            reserveBlackPawnsNumb++;
        }
    }
    this->board[x][y] = 0;
}

// function to capture in the top-right direction
void Game::captureTopRight(int capturingColor, pair<int, int> index) {
    int x = index.first;
    int y = index.second;
    while (y < arraySize - 1 && x > 0 && board[x][y] != -1) {
        y++;
        x--;
    }
    while (x < arraySize - 1 && y > 0 && board[x][y] != -1) {
        int color = board[x][y];
        if (color == capturingColor) {
            if (capturingColor == 1) {
                reserveWhitePawnsNumb++;
            } else if (capturingColor == 2) {
                reserveBlackPawnsNumb++;
            }
        }
        this->board[x][y] = 0;
        x++;
        y--;
    }
    if (board[x][y] == capturingColor) {
        if (capturingColor == 1) {
            reserveWhitePawnsNumb++;
        } else if (capturingColor == 2) {
            reserveBlackPawnsNumb++;
        }
    }
    this->board[x][y] = 0;
}

// function to capture in the top-left direction
void Game::captureTopLeft(int capturingColor, pair<int, int> index) {
    int x = index.first;
    int y = index.second;
    while (x > 0 && board[x][y] != -1) {
        x--;
    }
    while (x < arraySize - 1 && board[x][y] != -1) {
        int color = board[x][y];
        if (color == capturingColor) {
            if (capturingColor == 1) {
                reserveWhitePawnsNumb++;
            } else if (capturingColor == 2) {
                reserveBlackPawnsNumb++;
            }
        }
        this->board[x][y] = 0;
        x++;
    }
    if (board[x][y] == capturingColor) {
        if (capturingColor == 1) {
            reserveWhitePawnsNumb++;
        } else if (capturingColor == 2) {
            reserveBlackPawnsNumb++;
        }
    }
    this->board[x][y] = 0;
}

// function to capture in the down-right direction
void Game::captureDownRight(int capturingColor, pair<int, int> index) {
    int x = index.second;
    int y = index.first;
    while (x > 0 && board[x - 1][y] > 0) {
        x--;
    }
    while (x < arraySize - 1 && board[x][y] > 0) {
        int color = board[x][y];
        if (color == capturingColor) {
            if (capturingColor == 1) {
                reserveWhitePawnsNumb++;
            } else if (capturingColor == 2) {
                reserveBlackPawnsNumb++;
            }
        }
        this->board[x][y] = 0;
        x++;
    }
    if (board[x][y] == capturingColor) {
        if (capturingColor == 1) {
            reserveWhitePawnsNumb++;
        } else if (capturingColor == 2) {
            reserveBlackPawnsNumb++;
        }
    }
    this->board[x][y] = 0;
}

// function to capture in the down-left direction
void Game::captureDownLeft(int capturingColor, pair<int, int> index) {
    int color = 0;
    int x = index.second;
    int y = index.first;
    while (y < arraySize - 1 && x > 0 && board[x - 1][y + 1] > 0) {
        y++;
        x--;
    }
    while (x < arraySize - 1 && y > 0 && board[x][y] > 0) {
        color = board[x][y];
        if (color == capturingColor) {
            if (capturingColor == 1) {
                reserveWhitePawnsNumb++;
            } else if (capturingColor == 2) {
                reserveBlackPawnsNumb++;
            }
        }
        this->board[x][y] = 0;
        x++;
        y--;
    }
    if (board[x][y] == capturingColor) {
        if (capturingColor == 1) {
            reserveWhitePawnsNumb++;
        } else if (capturingColor == 2) {
            reserveBlackPawnsNumb++;
        }
    }
    this->board[x][y] = 0;
}

// function to find all chains of length of chainLength on board
vector<vector<pair<int, int>>> Game::findChains(int chainLength) {
    vector<vector<pair<int, int>>> chainsPos;
    
    // horizontal chains
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j <= arraySize - chainLength; j++) {
            bool isChain = true;
            for (int k = 1; k < chainLength; k++) {
                // check if consecutive elemnts are equal to each other
                if (board[i][j + k] != board[i][j] || board[i][j + k] == 0) {
                    isChain = false;
                    break;
                }
            }
            if (isChain) {
                vector<pair<int, int>> currentChain;
                int k = 0;
                // add positions of the chain to the vector
                while (j + k < arraySize && board[i][j + k] != 0) {
                    currentChain.push_back(make_pair(j + k, i));
                    k++;
                }
                chainsPos.push_back(currentChain);
            }
        }
    }
    
    // vertical chains
    for (int i = 0; i <= arraySize - chainLength; i++) {
        for (int j = 0; j < arraySize; j++) {
            bool isChain = true;
            for (int k = 1; k < chainLength; k++) {
                // check if consecutive elemnts are equal to each other
                if (board[i + k][j] != board[i][j] || board[i + k][j] == 0) {
                    isChain = false;
                    break;
                }
            }
            if (isChain) {
                vector<pair<int, int>> currentChain;
                int k = 0;
                // add positions of the chain to the vector
                while (i + k < arraySize && board[i + k][j] != 0) {
                    currentChain.push_back(make_pair(j, i + k));
                    k++;
                }
                chainsPos.push_back(currentChain);
            }
        }
    }
    
    // diagonal chains top-left to bottom-right
    for (int i = 0; i <= arraySize - chainLength; i++) {
        for (int j = 0; j <= arraySize - chainLength; j++) {
            bool isChain = true;
            for (int k = 1; k < chainLength; k++) {
                // check if consecutive elemnts are equal to each other
                if (board[i + k][j + k] != board[i][j] || board[i + k][j + k] == 0) {
                    isChain = false;
                    break;
                }
            }
            if (isChain) {
                vector<pair<int, int>> currentChain;
                int k = 0;
                // add positions of the chain to the vector
                while (j + k > 0 && i + k < arraySize && board[i + k][j + k] != 0) {
                    currentChain.push_back(make_pair(j + k, i + k));
                    k++;
                }
                chainsPos.push_back(currentChain);
            }
        }
    }
    
    // diagonal chains top-right to bottom-left
    for (int i = 0; i <= arraySize - chainLength; i++) {
        for (int j = chainLength - 1; j < arraySize; j++) {
            bool isChain = true;
            for (int k = 1; k < chainLength; k++) {
                // check if consecutive elemnts are equal to each other
                if (board[i + k][j - k] != board[i][j] || board[i + k][j - k] == 0) {
                    isChain = false;
                    break;
                }
            }
            if (isChain) {
                vector<pair<int, int>> currentChain;
                int k = 0;
                // add positions of the chain to the vector
                while (j - k > 0 && i + k < arraySize && board[i + k][j - k] != 0) {
                    currentChain.push_back(make_pair(j - k, i + k));
                    k++;
                }
                chainsPos.push_back(currentChain);
            }
        }
    }
    return chainsPos;
}

// function to check if vec is a subset of any vector in vectors
bool Game::isVectorContained(const vector<pair<int, int>>& vec, const vector<vector<pair<int, int>>>& vectors) {
    // iterate over each vector in the vector of vectors
    for (const auto& otherVector : vectors) {
        // skip if vector is larger than the given vector
        if (vec.size() >= otherVector.size())
            continue;
        bool isSubset = true;
        // iterate over each pair in the given vector
        for (const auto& pair : vec) {
            // check if pair is present in the current vector
            if (find(otherVector.begin(), otherVector.end(), pair) == otherVector.end()) {
                // if its not found then the vector is not a subset
                isSubset = false;
                break;
            }
        }
        if (isSubset) {
            return true;
        }
    }
    return false;
}

// function to remove all vectors that are included in other vectors
vector<vector<pair<int, int>>> Game::removeContainedVectors() {
    vector<vector<pair<int, int>>> result;
    for (const auto& vector : existingChains) {
        if (!isVectorContained(vector, existingChains)) {
            result.push_back(vector);
        }
    }
    return result;
}

// function to generate all possible index combinations possible for move
vector<pair<string, string>> Game::generateIndexCombinations() {
    vector<pair<string, string>> combinations;
    int fieldsSize = 2 * boardSize + 1;
    int k = 0, l = 0;
    for (int i = 1; i < fieldsSize - 1; i++) {
        k = 0;
        l = fieldsSize - 1;
        while (fieldIndexes[i][k] == " ") {
            k++;
        }
        while (fieldIndexes[i][l] == " ") {
            l--;
        }
        if (i == fieldsSize / 2) {
            combinations.push_back(make_pair(fieldIndexes[i][k], fieldIndexes[i][k + 1]));
            combinations.push_back(make_pair(fieldIndexes[i][l], fieldIndexes[i][l - 1]));
        }
        if (i > fieldsSize / 2) {
            combinations.push_back(make_pair(fieldIndexes[i][k], fieldIndexes[i][k + 1]));
            combinations.push_back(make_pair(fieldIndexes[i][k], fieldIndexes[i - 1][k]));
            combinations.push_back(make_pair(fieldIndexes[i][l], fieldIndexes[i][l - 1]));
            combinations.push_back(make_pair(fieldIndexes[i][l], fieldIndexes[i - 1][l - 1]));
        }
        if (i < fieldsSize / 2) {
            combinations.push_back(make_pair(fieldIndexes[i][k], fieldIndexes[i][k + 1]));
            combinations.push_back(make_pair(fieldIndexes[i][k], fieldIndexes[i + 1][k]));
            combinations.push_back(make_pair(fieldIndexes[i][l], fieldIndexes[i][l - 1]));
            combinations.push_back(make_pair(fieldIndexes[i][l], fieldIndexes[i + 1][l - 1]));
        }
    }
    
    k = 0;
    while (fieldIndexes[0][k] == " ") {
        k++;
    }
    for (int i = 0; i <= rowLengthsVect[0]; i++) {
        combinations.push_back(make_pair(fieldIndexes[0][k + i], fieldIndexes[1][k + i]));
        combinations.push_back(make_pair(fieldIndexes[0][k + i], fieldIndexes[1][k + i - 1]));
        combinations.push_back(make_pair(fieldIndexes[fieldsSize - 1][k + i], fieldIndexes[fieldsSize - 2][k + i]));
        combinations.push_back(make_pair(fieldIndexes[fieldsSize - 1][k + i], fieldIndexes[fieldsSize - 2][k + i - 1]));
    }
    this->indexCombinations = combinations;
    deleteRepeatedPairs();
    return combinations;
}

// function to generate all possible moves for current game states
vector<pair<string, string>> Game::generatePossibleMoves() {
    bool found = false;
    generateIndexCombinations();
    vector<pair<string, string>> possibleMoves;
    vector<vector<vector<int>>> generatedUniqueMoves;
    // iterate over all possible indexCombinations
    for (auto move : indexCombinations) {
        found = false;
        // create tempBoard that will store the board
        vector<vector<int>> tempBoard = board;
        // try to move
        tryMove(move.first, move.second);
        // check if the board changed and if yes then it indicates that move is possible
        if (tempBoard != board) {
            // check if the board state was already found
            for (auto genBoard : generatedUniqueMoves) {
                if (genBoard == board) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                existingChains = findChains(pawnsCaptureNumb);
                existingChains = removeContainedVectors();
                if (existingChains.size() > 0) {
                    if (hasMultiNeighbours()) {
                        for (int i = 0; i < existingChains.size(); i++) {
                            possibleMoves.push_back(move);
                        }
                    } else {
                        possibleMoves.push_back(move);
                    }
                } else {
                    possibleMoves.push_back(move);
                }
                // add the board to vector so it will be known that it was already checked
                generatedUniqueMoves.push_back(board);
            }
            board = tempBoard;
        }
    }
    return possibleMoves;
}

// function to delete repetaded pairs of index combinations
void Game::deleteRepeatedPairs() {
    unordered_set<string> uniquePairs;
    vector<pair<string, string>> tempCombinations;
    
    for (const auto& pair : indexCombinations) {
        string key = pair.first + "_" + pair.second;
        
        if (uniquePairs.find(key) == uniquePairs.end()) {
            uniquePairs.insert(key);
            tempCombinations.push_back(pair);
        }
    }
    
    indexCombinations = tempCombinations;
}

// function that tries a move
void Game::tryMove(const string& startIndex, const string& endIndex) {
    string direction = "";
    if (!checkIfIndexIsCorrect(startIndex)) {
        return;
    } else if (!checkIfIndexIsCorrect(endIndex)) {
        return;
    } else if (!checkIfStartIndexIsCorrect(startIndex)) {
        return;
    } else if (!checkIfEndIndexIsCorrect(endIndex)) {
        return;
    } else if (!checkIfDirectionIsCorrect(startIndex, endIndex)) {
        return;
    }
    
    direction = findMoveDirection(startIndex, endIndex);
    
    if (direction == "LEFT") {
        moveLeft(endIndex, true);
    } else if (direction == "RIGHT") {
        moveRight(endIndex, true);
    } else if (direction == "TOP_LEFT") {
        moveTopLeft(endIndex, true);
    } else if (direction == "TOP_RIGHT") {
        moveTopRight(endIndex, true);
    } else if (direction == "DOWN_RIGHT") {
        moveDownRight(endIndex, true);
    } else if (direction == "DOWN_LEFT") {
        moveDownLeft(endIndex, true);
    }
}

// function to check if any of the game ending requirements were met
void Game::checkIfGameIsOver() {
    if (reserveBlackPawnsNumb == 0) {
        cout << "THE_WINNER_IS_WHITE" << endl;
    } else if (reserveWhitePawnsNumb == 0) {
        cout << "THE_WINNER_IS_BLACK" << endl;
    } else {
        vector<pair<string, string>> possibleMoves = generatePossibleMoves();
        if (possibleMoves.size() == 0) {
            if (whoseTurn == 1) {
                cout << "THE_WINNER_IS_BLACK" << endl;
            } else if (whoseTurn == 2) {
                    cout << "THE_WINNER_IS_WHITE" << endl;
            }
        } else {
            cout << "GAME_IN_PROGRESS" << endl;
        }
    }
    
}

// function that checks if any spot has more than 2 same neighbors
bool Game::hasMultiNeighbours() {
    int count = 0, maxCount = 0, currentColor = -1;
    for (int i = 1; i < arraySize - 1; i++) {
        for (int j = 0; j < arraySize; j++) {
            count = 0;
            if (board[i][j] > 0) {
                currentColor = board[i][j];
                if (board[i - 1][j] == currentColor) count++;
                if (board[i + 1][j] == currentColor) count++;
                if (j > 0 && board[i][j - 1] == currentColor) count++;
                if (j < arraySize - 1 && board[i][j + 1] == currentColor) count++;
                if (j > 0 && board[i + 1][j - 1] == currentColor) count++;
                if (j < arraySize - 1 && board[i - 1][j + 1] == currentColor) count++;
            }
            if (count > maxCount) {
                maxCount = count;
            }
        }
    }
    return maxCount > 2;
}
