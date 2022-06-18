#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
#define INF 100000000

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

bool color = false;
int beeWin = INF / 100;
int player, avail = 225;
const int SIZE = 15, DEPTH = 3;
std::array<std::array<int, SIZE>, SIZE> board;
class node {
    public:
    double value;
    int x; 
    int y;
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

int getCsctScore(int cnt, int blocks, bool curTurn) {
    if (blocks == 2 && cnt < 5) return 0;
    if (cnt == 5) return INF;
    else if (cnt == 4) {
        return (curTurn) ? beeWin : (blocks == 0) ? beeWin >> 2 : 200;
    }
    else if (cnt == 3) {
        return (blocks == 0) ? ((curTurn) ? 50000 : 200) : ((curTurn) ? 10 : 5);
    }
    else if (cnt == 2) {
        return (blocks == 0) ? ((curTurn) ? 7 : 5) : 3;
    }
    else if (cnt == 1) return 1;
    return 2 * INF;
}

int eval_hrz(std::array<std::array<int, SIZE>, SIZE> bd, bool is_blk, bool myturn) {
    int csct = 0, blocks = 2, score = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (bd[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (bd[i][j] == EMPTY) {
                if (csct > 0) {
                    blocks--;
                    score += getCsctScore(csct, blocks, is_blk == myturn);
                    csct = 0; blocks = 1;
                }
                else blocks++;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_blk == myturn);
                csct = 0; blocks = 2;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_blk == myturn);
        csct = 0; blocks = 2;
    }
    return score;
}

int eval_vtc(std::array<std::array<int, SIZE>, SIZE> bd, bool is_blk, bool myturn) {
    int csct = 0, blocks = 2, score = 0;
    for (int j = 0; j < SIZE; j++) {
        for (int i = 0; i < SIZE; i++) {
            if (bd[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (bd[i][j] == EMPTY) {
                if (csct > 0) {
                    blocks--;
                    score += getCsctScore(csct, blocks, is_blk == myturn);
                    csct = 0; blocks = 1;
                }
                else blocks++;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_blk == myturn);
                csct = 0; blocks = 2;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_blk == myturn);
        csct = 0; blocks = 2;
    }
    return score;
}

int eval_dgn(std::array<std::array<int, SIZE>, SIZE> bd, bool is_blk, bool myturn) {
    int csct = 0, blocks = 2, score = 0;
    for (int k = 0; k <= 2 * (SIZE - 1); k++) {
        int st = std::max(0, k - SIZE + 1);
        int ed = std::min(SIZE - 1, k);
        for (int i = st; i <= ed; i++) {
            int j = k - i; //btLeft -> upRight
            if (bd[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (bd[i][j] == EMPTY) {
                if (csct > 0) {
                    blocks--;
                    score += getCsctScore(csct, blocks, is_blk == myturn);
                    csct = 0; blocks = 1;
                }
                else blocks++;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_blk == myturn);
                csct = 0; blocks = 2;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_blk == myturn);
        csct = 0; blocks = 2;
    }
    for (int k = 1 - SIZE; k < SIZE; k++) {
        int st = std::max(0, k);
        int ed = std::min(SIZE + k - 1, SIZE - 1);
        for (int i = st; i <= ed; i++) {
            int j = i - k;
            if (bd[i][j] == (is_blk ? BLACK : WHITE)) csct++;
            else if (bd[i][j] == EMPTY) {
                if (csct > 0) {
                    blocks--;
                    score += getCsctScore(csct, blocks, is_blk == myturn);
                    csct = 0; blocks = 1;
                }
                else blocks++;
            }
            else if (csct > 0) {
                score += getCsctScore(csct, blocks, is_blk == myturn);
                csct = 0; blocks = 2;
            }
            else blocks = 2;
        }
        if (csct > 0) score += getCsctScore(csct, blocks, is_blk == myturn);
        csct = 0; blocks = 2;
    }
    return score;
}

double calBoard(std::array<std::array<int, SIZE>, SIZE> bd, bool is_blk, bool myturn) {
    return eval_dgn(bd, is_blk, myturn) + eval_hrz(bd, is_blk, myturn) + eval_vtc(bd, is_blk, myturn);
}

double evalBoard(std::array<std::array<int, SIZE>, SIZE> bd, bool myturn) {
    double blkScore = calBoard(bd, true, myturn);
    double whtScore = calBoard(bd, false, myturn);
    if (color) {
        if (whtScore == 0) whtScore = 1.0;
        return blkScore / whtScore;
    } else {
        if (blkScore == 0) blkScore = 1.0;
        return whtScore / blkScore;
    }
}

/*std::vector<std::pair<int, int>> findMoves(std::array<std::array<int, SIZE>, SIZE> bd) {
    std::vector<std::vector<bool>> is_res(SIZE, std::vector<bool> (SIZE, false));
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (bd[i][j] != EMPTY || is_res[i][j]) continue;
            if ((i - 2 >= 0 && bd[i - 2][j]) || (i - 1 >= 0 && bd[i - 1][j]) ||
                (i + 2 < SIZE && bd[i + 2][j]) || (i + 1 < SIZE && bd[i + 1][j]) ||
                (j - 2 >= 0 && bd[i][j - 2]) || (j - 1 >= 0 && bd[i][j - 1]) ||
                (j + 2 < SIZE && bd[i][j + 2]) || (j + 1 < SIZE && bd[i][j + 1]) ||
                (i - 2 >= 0 && j - 2 >= 0 && bd[i - 2][j - 2]) || (i - 1 >= 0 && j - 1 >= 0 && bd[i - 1][j - 1]) ||
                (i + 2 < SIZE && j - 2 >= 0 && bd[i + 2][j - 2]) || (i + 1 < SIZE && j - 1 >= 0 && bd[i + 1][j - 1]) ||
                (i - 2 >= 0 && j + 2 < SIZE && bd[i - 2][j + 2]) || (i - 1 >= 0 && j + 1 < SIZE && bd[i - 1][j + 1]) ||
                (i + 2 < SIZE && j + 2 < SIZE && bd[i + 2][j + 2]) || (i + 1 < SIZE && j + 1 < SIZE && bd[i + 1][j + 1]) ||
                (i - 2 >= 0 && j - 1 >= 0 && bd[i - 2][j - 1]) || (i - 1 >= 0 && j - 2 >= 0 && bd[i - 1][j - 2]) ||
                (i - 2 >= 0 && j + 1 < SIZE && bd[i - 2][j + 1]) || (i - 1 >= 0 && j + 2 < SIZE && bd[i - 1][j + 2]) ||
                (i + 2 < SIZE && j - 1 >= 0 && bd[i + 2][j - 1]) || (i + 1 < SIZE && j - 2 >= 0 && bd[i + 1][j - 2]) ||
                (i + 2 < SIZE && j + 1 < SIZE && bd[i + 2][j + 1]) || (i + 1 < SIZE && j + 2 < SIZE && bd[i + 1][j + 2])) {
                    is_res[i][j] = true;
                    moves.emplace_back(std::pair<int, int> {i, j});
                }
        }
    }
    return moves;
}*/

std::vector<std::pair<int, int>> findMoves(std::array<std::array<int, SIZE>, SIZE> bd) {
    std::vector<std::vector<bool>> is_res(SIZE, std::vector<bool> (SIZE, false));
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (bd[i][j] != EMPTY || is_res[i][j]) continue;
            if ((i - 1 >= 0 && bd[i - 1][j]) || (i + 1 < SIZE && bd[i + 1][j]) ||
                (j - 1 >= 0 && bd[i][j - 1]) || (j + 1 < SIZE && bd[i][j + 1]) ||
                (i - 1 >= 0 && j - 1 >= 0 && bd[i - 1][j - 1]) || (i + 1 < SIZE && j - 1 >= 0 && bd[i + 1][j - 1]) ||
                (i - 1 >= 0 && j + 1 < SIZE && bd[i - 1][j + 1]) || (i + 1 < SIZE && j + 1 < SIZE && bd[i + 1][j + 1])) {
                    is_res[i][j] = true;
                    moves.emplace_back(std::pair<int, int> {i, j});
                }
        }
    }
    return moves;
}

node alphabeta(std::array<std::array<int, SIZE>, SIZE> bd, int depth, double a, double b, bool maximizingPlayer, std::ofstream& mov) {
    if (depth == 0) {
        double val = evalBoard(bd, !maximizingPlayer);
        //mov << "val: " << val << '\n';
        //mov.flush();
        return node(val, -1, -1);
    }
    std::vector<std::pair<int, int>> avail_move = findMoves(bd);
    if (avail_move.empty()) return node(evalBoard(bd, !maximizingPlayer), -1, -1);
    node bestMov(-1, -1);
    mov << avail_move.size() << '\n';
    if (maximizingPlayer) {
        bestMov.value = -1;
        for (auto m : avail_move) {
            std::array<std::array<int, SIZE>, SIZE> tmp = bd;
            tmp[m.first][m.second] = (color) ? BLACK : WHITE;
            /*for (auto it : tmp) {
                for (auto j : it) mov << j << ' ';
                mov << "\n";
            }
            mov << '\n';
            mov.flush();*/
            node tmpMov = alphabeta(tmp, depth - 1, a, b, !maximizingPlayer, mov);
            if (tmpMov.value > a) a = tmpMov.value;
            if (tmpMov.value >= b) return tmpMov;
            if (tmpMov.value > bestMov.value) {
                bestMov = tmpMov;
                bestMov.x = m.first; bestMov.y = m.second;
                //mov << tmpMov.x << " " << tmpMov.y << " " << tmpMov.value << '\n';
                //mov.flush();
            }
        }
    }
    else {
        bestMov.value = INF;
        bestMov.x = avail_move[0].first;
        bestMov.y = avail_move[1].second;
        for (auto m : avail_move) {
            std::array<std::array<int, SIZE>, SIZE> tmp = bd;
            tmp[m.first][m.second] = (!color) ? BLACK : WHITE;
            /*for (auto it : tmp) {
                for (auto j : it) mov << j << ' ';
                mov << "\n";
            }
            mov << '\n';
            mov.flush();*/
            node tmpMov = alphabeta(tmp, depth - 1, a, b, !maximizingPlayer, mov);
            if (tmpMov.value < b) b = tmpMov.value;
            if (tmpMov.value <= a) return tmpMov;
            if (tmpMov.value < bestMov.value) {
                bestMov = tmpMov;
                bestMov.x = m.first;
                bestMov.y = m.second;
                //mov << tmpMov.x << " " << tmpMov.y << " " << tmpMov.value << '\n';
                //mov.flush();
            }
        }
    }
    return bestMov;
}

node findWinMov(std::array<std::array<int, SIZE>, SIZE> bd) {
    std::vector<std::pair<int, int>> avail_mov = findMoves(bd);
    node winMov(-1, -1);
    for (auto &it : avail_mov) {
        std::array<std::array<int, SIZE>, SIZE> tmp = bd;
        tmp[it.first][it.second] = color;
        if (calBoard(tmp, color, color) >= INF) {
            winMov.x = it.first;
            winMov.y = it.second;
            break;
        }
    }
    return winMov;
}

void write_valid_spot(std::ofstream& fout, std::ofstream& mov) {  
    int x, y;
    if (avail == 225) {
        fout << 7 << " " << 7 << '\n';
        fout.flush();
        return;
    }
    color = (avail & 1) ? true : false;
    node bestMove = findWinMov(board);
    if (bestMove.x != -1 && bestMove.y != -1) {
        fout << bestMove.x << " " << bestMove.y << '\n';
        fout.flush();
        return;
    }
    bestMove = alphabeta(board, DEPTH, -1, INF, !color, mov);
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
    std::ofstream tryMov(argv[3]);
    std::ofstream pos_mov(argv[3]);
    read_board(fin);
    write_valid_spot(fout, pos_mov);
    fin.close();
    fout.close();
    tryMov.close();
    return 0;
}