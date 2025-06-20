#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <unordered_set>
#include <utility>

using namespace std;

class Game {
private:
    int boardSize;
    int arraySize;
    vector<vector<int>> board;
    vector<vector<string>> fieldIndexes;
    vector<vector<pair<int, int>>> existingChains;
    vector<pair<string, string>> indexCombinations;
    vector<int> rowLengthsVect;
    vector<int> fieldsRowLengthsVect;
    int whitePawnsNumb;
    int blackPawnsNumb;
    int reserveWhitePawnsNumb;
    int reserveBlackPawnsNumb;
    int whoseTurn;
    int pawnsNumb;
    int pawnsCaptureNumb;
    string captureStart;
    string captrueEnd;
    string captureColor;
public:
    Game();
    void readBoard();
    void setBoard(const vector<vector<int>>& board);
    void checkIfPawnsNumberIsCorrect();
    void checkIfParametersAreCorrect(tuple<int, int, int, int> parameters);
    void detectCommand(string input);
    tuple<int, int, int, int> readInput(string input);
    tuple<int, int, int> readState(string input);
    static string formatInput(string input);
    void calculateRowLengths();
    void calculateFieldsRowLengths();
    void drawBoard();
    void emptyBoard();
    bool isEmptyBoard();
    void createFieldIndexes();
    bool checkIfIndexIsCorrect(const string& index);
    bool emptyNeighbor(int i, int j);
    bool checkIfStartIndexIsCorrect(const string& index);
    bool checkIfEndIndexIsCorrect(const string& index);
    static bool checkIfDirectionIsCorrect(string startIndex, string endIndex);
    string findMoveDirection(const string& startIndex, const string& endIndex);
    pair<int, int> findIndexOnBoard(const string& index);
    void moveLeft(const string& startIndex, bool trying);
    void moveRight(const string& startIndex, bool trying);
    void moveTopLeft(const string& startIndex, bool trying);
    void moveDownLeft(const string& startIndex, bool trying);
    void moveTopRight(const string& startIndex, bool trying);
    void moveDownRight(const string& startIndex, bool trying);
    void makeMove(const string& startIndex, const string& endIndex);
    void finishTurn();
    void drawEverything();
    void checkIfBoardIncludesRowOfLenghtK();
    void captureDownRight(int capturingColor, pair<int, int> index);
    void captureDownLeft(int capturingColor, pair<int, int> index);
    void captureRight(int capturingColor, pair<int, int> index);
    void captureLeft(int capturingColor, pair<int, int> index);
    void captureTopRight(int capturingColor, pair<int, int> index);
    void captureTopLeft(int capturingColor, pair<int, int> index);
    void capturePieces();
    vector<vector<pair<int, int>>> findChains(int chainLength);
    vector<vector<pair<int, int>>> removeContainedVectors();
    static bool isVectorContained(const vector<pair<int, int>>& vec, const vector<vector<pair<int, int>>>& vectors);
    vector<pair<string, string>> generateIndexCombinations();
    vector<pair<string, string>> generatePossibleMoves();
    void deleteRepeatedPairs();
    void tryMove(const string& startIndex, const string& endIndex);
    void checkIfGameIsOver();
    bool hasMultiNeighbours();
};
