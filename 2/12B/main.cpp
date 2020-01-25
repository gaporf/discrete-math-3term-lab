#include <fstream>
#include <vector>
#include <algorithm>

struct edge {
    size_t u, v;
    int64_t s;
    size_t num;
    bool used;

    edge() = default;

    bool operator<(const edge &rhs) {
        return s < rhs.s;
    }
};

size_t get_next(size_t v, std::vector<size_t> &next) {
    if (next[v] == v) {
        return v;
    } else {
        return next[v] = get_next(next[v], next);
    }
}

void unite(size_t u, size_t v, std::vector<size_t> &next, std::vector<size_t> &rang) {
    u = get_next(u, next);
    v = get_next(v, next);
    if (rang[u] < rang[v]) {
        std::swap(u, v);
    } else if (rang[u] == rang[v]) {
        rang[u]++;
    }
    next[v] = u;
}

int main() {
    std::ifstream in("destroy.in");
    std::ofstream out("destroy.out");
    size_t n, m;
    int64_t s;
    in >> n >> m >> s;
    std::vector<edge> edges(m);
    for (size_t i = 0; i < m; i++) {
        edges[i].num = i;
        edges[i].used = false;
        in >> edges[i].u >> edges[i].v >> edges[i].s;
        edges[i].u--;
        edges[i].v--;
    }
    std::sort(edges.rbegin(), edges.rend());
    std::vector<size_t> next(n),
            rang(n, 0);
    for (size_t i = 0; i < n; i++) {
        next[i] = i;
    }
    for (auto &edge : edges) {
        size_t u = edge.u,
                v = edge.v;
        if (get_next(u, next) != get_next(v, next)) {
            unite(u, v, next, rang);
            edge.used = true;
        }
    }
    std::vector<size_t> ans;
    for (auto it = edges.rbegin(); it != edges.rend(); it++) {
        if (!it->used) {
            if (s >= it->s) {
                s -= it->s;
                ans.push_back(it->num + 1);
            } else {
                break;
            }
        }
    }
    out << ans.size() << std::endl;
    for (size_t v : ans) {
        out << v << " ";
    }
    out << std::endl;
    return 0;
}
