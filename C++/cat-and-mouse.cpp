// Time:  O(n^3)
// Space: O(n^2)

class Solution {
private:
    template <typename A, typename B, typename C>
    struct TupleHash {
        size_t operator()(const tuple<A, B, C>& p) const {
            size_t seed = 0;
            A a; B b; C c;
            tie(a, b, c) = p;
            seed ^= std::hash<A>{}(a) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            seed ^= std::hash<B>{}(b) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            seed ^= std::hash<C>{}(c) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            return seed;
        }
    };
    using Lookup = unordered_map<tuple<int, int, int>, int, TupleHash<int, int, int>>;

    enum Location {HOLE, MOUSE_START, CAT_START};
    enum Result {DRAW, MOUSE, CAT};

public:
    int catMouseGame(vector<vector<int>>& graph) {
        Lookup degree;
        unordered_set<int> ignore(cbegin(graph[HOLE]), cend(graph[HOLE]));
        for (int m = 0; m < size(graph); ++m) {
            for (int c = 0; c < size(graph); ++c) {
                degree[make_tuple(m, c, MOUSE)] = size(graph[m]);
                degree[make_tuple(m, c, CAT)] = size(graph[c]) - ignore.count(c);
            }
        }
        Lookup color;
        queue<tuple<int, int, int, int>> q;
        for (int i = 0; i < size(graph); ++i) {
            if (i == HOLE) {
                continue;
            }
            color[make_tuple(HOLE, i, CAT)] = MOUSE;
            q.emplace(HOLE, i, CAT, MOUSE);
            for (const auto& t : {MOUSE, CAT}) {
                color[make_tuple(i, i, t)] = CAT;
                q.emplace(i, i, t, CAT);
            }
        }
        while (!empty(q)) {
            const auto [i, j, t, c] = q.front(); q.pop();
            for (const auto& [ni, nj, nt] : parents(graph, i, j, t)) {
                if (color[make_tuple(ni, nj, nt)] != DRAW) {
                    continue;
                }
                if (nt == c) {
                    color[make_tuple(ni, nj, nt)] = c;
                    q.emplace(ni, nj, nt, c);
                    continue;
                }
                --degree[make_tuple(ni, nj, nt)];
                if (!degree[make_tuple(ni, nj, nt)]) {
                    color[make_tuple(ni, nj, nt)] = c;
                    q.emplace(ni, nj, nt, c);
                }
            }
        }
        return color[make_tuple(MOUSE_START, CAT_START, MOUSE)];
    }

private:
    vector<tuple<int, int, int>> parents(const vector<vector<int>>& graph,
                                         int m, int c, int t) {
        vector<tuple<int, int, int>> result;
        if (t == CAT) {
            for (const auto& nm : graph[m]) {
                result.emplace_back(nm, c, MOUSE ^ CAT ^ t);
            }
        } else {
            for (const auto& nc : graph[c]) {
                if (nc != HOLE) {
                    result.emplace_back(m, nc, MOUSE ^ CAT ^ t);
                }
            }
        }
        return result;
    }
};

// Time:  O(n^3)
// Space: O(n^2)
class Solution2 {
private:
    template <typename A, typename B, typename C>
    struct TupleHash {
        size_t operator()(const tuple<A, B, C>& p) const {
            size_t seed = 0;
            A a; B b; C c;
            tie(a, b, c) = p;
            seed ^= std::hash<A>{}(a) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            seed ^= std::hash<B>{}(b) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            seed ^= std::hash<C>{}(c) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            return seed;
        }
    };
    using Lookup = unordered_map<tuple<int, int, int>, int, TupleHash<int, int, int>>;

    enum Location {HOLE, MOUSE_START, CAT_START};
    enum Result {DRAW, MOUSE, CAT};

public:
    int catMouseGame(vector<vector<int>>& graph) {
        Lookup degree;
        unordered_set<int> ignore(cbegin(graph[HOLE]), cend(graph[HOLE]));
        for (int m = 0; m < size(graph); ++m) {
            for (int c = 0; c < size(graph); ++c) {
                degree[make_tuple(m, c, MOUSE)] = size(graph[m]);
                degree[make_tuple(m, c, CAT)] = size(graph[c]) - ignore.count(c);
            }
        }
        Lookup color;
        queue<tuple<int, int, int>> q1;
        queue<tuple<int, int, int>> q2;
        for(int i = 0; i < size(graph); ++i) {
            if (i == HOLE) {
                continue;
            }
            color[make_tuple(HOLE, i, CAT)] = MOUSE;
            q1.emplace(HOLE, i, CAT);
            for (const auto& t : {MOUSE, CAT}) {
                color[make_tuple(i, i, t)] = CAT;
                q2.emplace(i, i, t);
            }
        }
        while (!empty(q1)) {
            const auto [i, j, t] = q1.front(); q1.pop();
            for (const auto& [ni, nj, nt] : parents(graph, i, j, t)) {
                if (color[make_tuple(ni, nj, nt)] != DRAW) {
                    continue;
                }
                if (t == CAT) {
                    color[make_tuple(ni, nj, nt)] = MOUSE;
                    q1.emplace(ni, nj, nt);
                    continue;
                }
                --degree[make_tuple(ni, nj, nt)];
                if (!degree[make_tuple(ni, nj, nt)]) {
                    color[make_tuple(ni, nj, nt)] = MOUSE;
                    q1.emplace(ni, nj, nt);
                }
            }
        }
        while (!empty(q2)) {
            const auto [i, j, t] = q2.front(); q2.pop();
            for (const auto& [ni, nj, nt] : parents(graph, i, j, t)) {
                if (color[make_tuple(ni, nj, nt)] != DRAW) {
                    continue;
                }
                if (t == MOUSE) {
                    color[make_tuple(ni, nj, nt)] = CAT;
                    q2.emplace(ni, nj, nt);
                    continue;
                }
                --degree[make_tuple(ni, nj, nt)];
                if (!degree[make_tuple(ni, nj, nt)]) {
                    color[make_tuple(ni, nj, nt)] = CAT;
                    q2.emplace(ni, nj, nt);
                }
            }
        }
        return color[make_tuple(MOUSE_START, CAT_START, MOUSE)];
    }

private:
    vector<tuple<int, int, int>> parents(const vector<vector<int>>& graph,
                                         int m, int c, int t) {
        vector<tuple<int, int, int>> result;
        if (t == CAT) {
            for (const auto& nm : graph[m]) {
                result.emplace_back(nm, c, MOUSE ^ CAT ^ t);
            }
        } else {
            for (const auto& nc : graph[c]) {
                if (nc != HOLE) {
                    result.emplace_back(m, nc, MOUSE ^ CAT ^ t);
                }
            }
        }
        return result;
    }
};
