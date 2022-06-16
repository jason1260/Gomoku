//alphabeta(origin, depth, −∞, +∞, TRUE)
#define INF 2147483647
class node {
    public:
    int x, y, value;
    node(int x, int y) : x(x), y(y), value(0) {};
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
int eval() {
    
}
node alphabeta(node n, int depth, int α, int β, bool maximizingPlayer) {
    if (depth == 0) return n;
    if (maximizingPlayer) {
        n.value = -INF;
        for each child of node do
            value := max(value, alphabeta(child, depth − 1, α, β, FALSE));
            α := max(α, value)
            if value ≥ β then
                break;
        return value;
    }
    else {
        value = INF
        for each child of node do
            value := min(value, alphabeta(child, depth − 1, α, β, TRUE));
            β := min(β, value)
            if value ≤ α then
                break (* α cutoff *)
        return value;
    }
}