#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#include <climits>
#include <cfloat>
#define INF 100000000

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

bool color = false;
int beeWin = INF / 100;
int player, avail = 225, cnt = 0;
const int SIZE = 15, DEPTH = 3;
int history_table[SIZE][SIZE];
std::array<std::array<int, SIZE>, SIZE> board;
class node {
    public:
    int x, y;
    double value;
    node(int x, int y) : x(x), y(y), value(0) {};
    node(double val, int x, int y) : x(x), y(y), value(val) {};
    bool operator==(const node& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const node& rhs) const {
		return !operator==(rhs);
	}
    bool operator>(const node& rhs) const {
        return (value != rhs.value) ? value > rhs.value : (x != rhs.x) ? x > rhs.x : y > rhs.y;
    }
    bool operator<(const node& rhs) const {
        return (value != rhs.value) ? value < rhs.value : (x != rhs.x) ? x < rhs.x : y < rhs.y;
    }
};


void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            if (board[i][j] != EMPTY) avail--;
        }
    }
}

int getCsctScore(int cnt, int blocks, bool is_special, bool turn) {
    if (blocks == 2 && cnt < 5) return 0;
    if (cnt >= 5) {
        return (is_special) ? ((turn) ? 35000 : (blocks == 0) ? 30000 : 25000) : INF;
    }
    else if (cnt == 4) {
        return (is_special) ? ((turn) ? 30000 : (blocks == 0) ? 25000 : 20000) : 
                ((turn) ? 1000000 : ((blocks == 0) ? 50000 : 200));
    }
    else if (cnt == 3) {
        return (is_special) ? ((blocks == 0) ? ((turn) ? 10000 : 100) : 20) : 
                ((blocks == 0) ? ((turn) ? 50000 : 400) : ((turn) ? 10 : 5));
    }
    else if (cnt == 2) {
        return (is_special) ? ((blocks == 0) ? ((turn) ? 5 : 3) : 1) : ((blocks == 0) ? ((turn) ? 7 : 5) : 2);
    }
    else if (cnt == 1) return 1;
    return 0;
}

int eval_dgn(bool is_blk, bool turn) {
    bool is_special = false;
    int csct = 0, blocks = 2, score = 0;
    for (int k = 0; k <= 2 * (SIZE - 1); k++) {
        int st = std::max(0, k - SIZE + 1);
        int ed = std::min(SIZE - 1, k);
        for (int i = st; i <= ed; i++) {
            int j = k - i;
            if (board[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (board[i][j] == EMPTY) {
                if (csct > 0) {
                    if (i + 1 <= ed && board[i + 1][j - 1] == (is_blk ? BLACK : WHITE)) is_special = true;
                    else {
                        blocks--;
                        score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                        csct = 0; blocks = 1; is_special = false;
                    }
                }
                else blocks = 1;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                csct = 0; blocks = 2; is_special = false;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_special, is_blk == turn);
        csct = 0; blocks = 2; is_special = false;
    }
    for (int k = 1 - SIZE; k < SIZE; k++) {
        int st = std::max(0, k);
        int ed = std::min(SIZE + k - 1, SIZE - 1);
        for (int i = st; i <= ed; i++) {
            int j = i - k;
            if (board[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (board[i][j] == EMPTY) {
                if (csct > 0) {
                    if (i + 1 <= ed && board[i + 1][j + 1] == (is_blk ? BLACK : WHITE)) is_special = true;
                    else {
                        blocks--;
                        score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                        csct = 0; blocks = 1; is_special = false;
                    }
                }
                else blocks = 1;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                csct = 0; blocks = 2; is_special = false;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_special, is_blk == turn);
        csct = 0; blocks = 2; is_special = false;
    }
    return score;
}
int eval_hrz(bool is_blk, bool turn) {
    int csct = 0, blocks = 2, score = 0;
    bool is_special = false;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (board[i][j] == EMPTY) {
                if (csct > 0) {
                    if (j + 1 < SIZE && board[i][j + 1] == (is_blk ? BLACK : WHITE)) is_special = true;
                    else {
                        blocks--;
                        score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                        csct = 0; blocks = 1; is_special = false;
                    }
                }
                else blocks = 1;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                csct = 0; blocks = 2; is_special = false;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_special, is_blk == turn);
        csct = 0; blocks = 2; is_special = false;
    }
    return score;
}
int eval_vtc(bool is_blk, bool turn) {
    bool is_special = false;
    int csct = 0, blocks = 2, score = 0;
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
            if (board[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (board[i][j] == EMPTY) {
                if (csct > 0) {
                    if (i + 1 < SIZE && board[i + 1][j] == (is_blk ? BLACK : WHITE)) is_special = true;
                    else {
                        blocks--;
                        score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                        csct = 0; blocks = 1; is_special = false;
                    }
                }
                else blocks = 1;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_special, is_blk == turn);
                csct = 0; blocks = 2; is_special = false;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_special, is_blk == turn);
        csct = 0; blocks = 2; is_special = false;
    }
    return score;
}

int calBoard(bool is_blk, bool turn) {
    return eval_dgn(is_blk, turn) + eval_hrz(is_blk, turn) + eval_vtc(is_blk, turn);
}

double evalBoard(bool turn) {
    double blkScore, whtScore;
    if (color) {
        blkScore = calBoard(true, !turn);
        whtScore = calBoard(false, !turn);
        if (whtScore == 0) whtScore = 1;
        return blkScore / whtScore;
    } else {
        blkScore = calBoard(true, turn);
        whtScore = calBoard(false, turn);
        if (blkScore == 0) blkScore = 1;
        return whtScore / blkScore;
    }
}

std::vector<std::pair<int, int>> findMoves() {
    std::vector<std::vector<bool>> is_res(SIZE, std::vector<bool> (SIZE, false));
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != EMPTY || is_res[i][j]) continue;
            if ((i - 1 >= 0 && board[i - 1][j]) || (i + 1 < SIZE && board[i + 1][j]) ||
                (j - 1 >= 0 && board[i][j - 1]) || (j + 1 < SIZE && board[i][j + 1]) ||
                (i - 1 >= 0 && j - 1 >= 0 && board[i - 1][j - 1]) || (i + 1 < SIZE && j - 1 >= 0 && board[i + 1][j - 1]) ||
                (i - 1 >= 0 && j + 1 < SIZE && board[i - 1][j + 1]) || (i + 1 < SIZE && j + 1 < SIZE && board[i + 1][j + 1])) {
                    moves.emplace_back(std::pair<int, int> {i, j});
                    is_res[i][j] = true;
                }
        }
    }
    return moves;
}

node alphabeta(int depth, double a, double b, bool maximizingPlayer) {
    if (depth == 0) {
        double val = evalBoard(!maximizingPlayer);
        return node(val, -1, -1);
    }
    std::vector<std::pair<int, int>> avail_move = findMoves();
    if (avail_move.empty()) return node(evalBoard(!maximizingPlayer), -1, -1);
    node bestMov(-1, -1);
    if (maximizingPlayer) {
        bestMov.value = -DBL_MAX;
        bestMov.x = avail_move[0].first;
        bestMov.y = avail_move[0].second;
        for (auto m : avail_move) {
            board[m.first][m.second] = (color) ? BLACK : WHITE;
            node tmpMov = alphabeta(depth - 1, a, b, !maximizingPlayer);
            board[m.first][m.second] = EMPTY;
            a = std::max(tmpMov.value, a);
            if (tmpMov.value >= b) return tmpMov;
            if (tmpMov.value > bestMov.value) {
                bestMov = tmpMov;
                bestMov.x = m.first; bestMov.y = m.second;
            }
        }
    }
    else {
        bestMov.value = DBL_MAX;
        bestMov.x = avail_move[0].first;
        bestMov.y = avail_move[0].second;
        for (auto m : avail_move) {
            board[m.first][m.second] = (!color) ? BLACK : WHITE;
            node tmpMov = alphabeta(depth - 1, a, b, !maximizingPlayer);
            board[m.first][m.second] = EMPTY;
            b = std::min(tmpMov.value, b);
            if (tmpMov.value <= a) return tmpMov;
            if (tmpMov.value < bestMov.value) {
                bestMov = tmpMov;
                bestMov.x = m.first; bestMov.y = m.second;
            }
        }
    }
    return bestMov;
}

node findWinMov() {
    std::vector<std::pair<int, int>> avail_mov = findMoves();
    node winMov(-1, -1);
    for (auto &it : avail_mov) {
        board[it.first][it.second] = (color) ? BLACK : WHITE;
        if (calBoard(color, color) >= INF) {
            winMov.x = it.first;
            winMov.y = it.second;
            board[it.first][it.second] = EMPTY;
            break;
        }
        board[it.first][it.second] = EMPTY;
    }
    return winMov;
}

node findLoseMov() {
    std::vector<std::pair<int, int>> avail_mov = findMoves();
    node loseMov(-1, -1);
    for (auto &it : avail_mov) {
        board[it.first][it.second] = (!color) ? BLACK : WHITE;
        int total = calBoard(!color, !color);  
        if (total >= INF) {
            loseMov.x = it.first;
            loseMov.y = it.second;
            board[it.first][it.second] = EMPTY;
            break;
        }
        board[it.first][it.second] = EMPTY;
    }
    return loseMov;
}

void write_valid_spot(std::ofstream& fout) {  
    int x, y;
    if (avail == 225) {
        fout << 7 << " " << 7 << '\n';
        fout.flush();
        return;
    }
    color = (avail & 1) ? true : false;
    node bestMove = findWinMov();
    if (bestMove.x != -1 && bestMove.y != -1) {
        fout << bestMove.x << " " << bestMove.y << '\n';
        fout.flush();
        return;
    }
    bestMove = findLoseMov();
    if (bestMove.x != -1 && bestMove.y != -1) {
        fout << bestMove.x << " " << bestMove.y << '\n';
        fout.flush();
        return;
    }
    bestMove = alphabeta(DEPTH, -DBL_MAX, DBL_MAX, true);
    x = bestMove.x;
    y = bestMove.y;
    if (board[x][y] == EMPTY) {
        fout << x << " " << y << '\n';
        fout.flush();
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    //std::cout << cnt << " times of compute.\n";
    return 0;
}