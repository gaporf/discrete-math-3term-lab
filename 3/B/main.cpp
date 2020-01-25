#include <fstream>
#include <vector>
#include <queue>

struct edge {
    edge(size_t from, size_t to) : from(from), to(to) {}

    size_t from, to;
};

struct dsu {
    explicit dsu(size_t n) : next(n), rank(n, 0) {
        for (size_t i = 0; i < n; i++) {
            next[i] = i;
        }
    }

    bool is_equal(size_t u, size_t v) {
        return get_next(u) == get_next(v);
    }

    void unite(size_t u, size_t v) {
        u = get_next(u);
        v = get_next(v);
        if (rank[u] < rank[v]) {
            std::swap(u, v);
        } else if (rank[u] == rank[v]) {
            rank[u]++;
        }
        next[v] = u;
    }

private:
    size_t get_next(size_t u) {
        if (next[u] == u) {
            return u;
        } else {
            return next[u] = get_next(next[u]);
        }
    }

    std::vector<size_t> next, rank;
};

int main() {
    std::ifstream in("multispan.in");
    std::ofstream out("multispan.out");
    size_t n, m;
    in >> n >> m;
    std::vector<edge> edges;
    edges.reserve(m);
    for (size_t i = 0; i < m; i++) {
        size_t from, to;
        in >> from >> to;
        from--;
        to--;
        edges.emplace_back(from, to);
    }
    std::vector<size_t> get_num(m, m);
    std::vector<std::vector<size_t> > G(m);
    size_t k = 0;
    for (size_t i = 0; i < m / (n - 1); i++) {
        std::vector<size_t> num_copy = get_num;
        dsu cur_dsu(n);
        size_t add_elements = 0;
        for (size_t j = 0; j < m; j++) {
            if (get_num[j] == m && !cur_dsu.is_equal(edges[j].from, edges[j].to)) {
                cur_dsu.unite(edges[j].from, edges[j].to);
                get_num[j] = i;
                add_elements++;
            }
        }
        while (add_elements != n - 1) {
            std::queue<size_t> q;
            std::vector<size_t> p(m, m);
            for (size_t j = 0; j < m; j++) {
                if (!cur_dsu.is_equal(edges[j].from, edges[j].to)) {
                    q.push(j);
                    p[j] = j;
                }
            }
            bool is_add = false;
            while (!q.empty()) {
                size_t v = q.front();
                q.pop();
                if (get_num[v] == m) {
                    std::vector<size_t> change;
                    for (size_t u = v;;) {
                        get_num[u] = get_num[p[u]];
                        change.push_back(get_num[u]);
                        u = p[u];
                        if (u == p[u]) {
                            cur_dsu.unite(edges[u].from, edges[u].to);
                            get_num[u] = i;
                            G[u].clear();
                            break;
                        }
                    }
                    for (size_t comp : change) {
                        std::vector<std::vector<std::pair<size_t, size_t> > > g(n);
                        for (size_t j = 0; j < m; j++) {
                            if (get_num[j] == comp) {
                                g[edges[j].from].push_back({edges[j].to, j});
                                g[edges[j].to].push_back({edges[j].from, j});
                                G[j].clear();
                            }
                        }
                        for (size_t j = 0; j < m; j++) {
                            std::queue<size_t> Q;
                            Q.push(edges[j].from);
                            std::vector<std::pair<size_t, size_t> > P(n, {n, n});
                            P[edges[j].from] = {0, 0};
                            while (!Q.empty()) {
                                size_t V = Q.front();
                                Q.pop();
                                for (auto &TO : g[V]) {
                                    if (P[TO.first].first == n) {
                                        P[TO.first] = {V, TO.second};
                                        Q.push(TO.first);
                                    }
                                }
                            }
                            for (size_t U = edges[j].to; U != edges[j].from; U = P[U].first) {
                                G[P[U].second].push_back(j);
                            }
                        }
                    }
                    add_elements++;
                    is_add = true;
                    break;
                }
                for (size_t to : G[v]) {
                    if (p[to] == m) {
                        p[to] = v;
                        q.push(to);
                    }
                }
            }
            if (!is_add) {
                break;
            }
        }
        if (add_elements == n - 1) {
            k++;
            std::vector<std::vector<std::pair<size_t, size_t> > > g(n);
            for (size_t j = 0; j < m; j++) {
                if (get_num[j] == i) {
                    g[edges[j].from].push_back({edges[j].to, j});
                    g[edges[j].to].push_back({edges[j].from, j});
                }
            }
            for (size_t j = 0; j < m; j++) {
                std::queue<size_t> q;
                q.push(edges[j].from);
                std::vector<std::pair<size_t, size_t> > p(n, {n, n});
                p[edges[j].from] = {0, 0};
                while (!q.empty()) {
                    size_t v = q.front();
                    q.pop();
                    for (auto &to : g[v]) {
                        if (p[to.first].first == n) {
                            p[to.first] = {v, to.second};
                            q.push(to.first);
                        }
                    }
                }
                for (size_t u = edges[j].to; u != edges[j].from; u = p[u].first) {
                    G[p[u].second].push_back(j);
                }
            }
        } else {
            get_num = num_copy;
            break;
        }
    }
    out << k << std::endl;
    std::vector<std::vector<size_t> > ans(k);
    for (size_t i = 0; i < m; i++) {
        if (get_num[i] != m) {
            ans[get_num[i]].push_back(i + 1);
        }
    }
    for (auto &v : ans) {
        for (size_t w : v) {
            out << w << " ";
        }
        out << std::endl;
    }
    return 0;
}