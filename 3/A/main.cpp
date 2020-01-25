#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

struct edge {
    edge(size_t from, size_t to, size_t color, size_t num) : from(from), to(to), color(color), num(num) {}

    edge(edge const &rhs) = default;

    bool operator==(edge const &rhs) {
        return num == rhs.num;
    }

    bool operator!=(edge const &rhs) {
        return num != rhs.num;
    }

    size_t from, to, color, num;
};

void dfs(size_t v, std::vector<std::vector<std::pair<size_t, size_t> > > &g, std::vector<size_t> &component, size_t cur_component) {
    component[v] = cur_component;
    for (std::pair<size_t, size_t> &to : g[v]) {
        if (component[to.first] == component.size()) {
            dfs(to.first, g, component, cur_component);
        }
    }
}

int main() {
    std::ifstream in("rainbow.in");
    std::ofstream out("rainbow.out");
    size_t n, m;
    in >> n >> m;
    if (m == 0) {
        out << 0 << std::endl;
        return 0;
    }
    std::vector<edge> edges;
    edges.reserve(m);
    for (size_t i = 0; i < m; i++) {
        size_t from, to, color;
        in >> from >> to >> color;
        from--;
        to--;
        color--;
        edges.emplace_back(from, to, color, i);
    }
    std::vector<edge> ans(1, edges[0]);
    std::vector<char> is_ans(m, false);
    is_ans[0] = true;
    while (true) {
        std::vector<std::vector<size_t> > G(m);
        size_t has_color[100];
        std::fill(has_color, has_color + 100, std::numeric_limits<size_t>::max());
        std::vector<std::vector<std::pair<size_t, size_t> > > g(n);
        std::vector<size_t> component(n, n);
        size_t cur_component = 0;
        for (edge &y : ans) {
            g[y.from].emplace_back(y.to, y.num);
            g[y.to].emplace_back(y.from, y.num);
            has_color[y.color] = y.num;
        }
        for (size_t i = 0; i < n; i++) {
            if (component[i] == n) {
                dfs(i, g, component, cur_component++);
            }
        }
        for (size_t z = 0; z < m; z++) {
            if (!is_ans[z]) {
                if (component[edges[z].from] != component[edges[z].to]) {
                    for (edge &y : ans) {
                        G[y.num].push_back(z);
                    }
                } else {
                    std::vector<std::pair<size_t, size_t> > p(n, {n, n});
                    std::queue<size_t> q;
                    q.push(edges[z].from);
                    while (!q.empty()) {
                        size_t v = q.front();
                        q.pop();
                        for (std::pair<size_t, size_t> &to : g[v]) {
                            if (p[to.first].first == n) {
                                p[to.first] = {v, to.second};
                                q.push(to.first);
                            }
                        }
                    }
                    for (size_t u = edges[z].to; u != edges[z].from; u = p[u].first) {
                        G[p[u].second].push_back(z);
                    }
                }
            }
        }
        for (size_t z = 0; z < m; z++) {
            if (!is_ans[z]) {
                if (has_color[edges[z].color] == std::numeric_limits<size_t>::max()) {
                    for (edge &y : ans) {
                        G[z].push_back(y.num);
                    }
                } else {
                    G[z].push_back(has_color[edges[z].color]);
                }
            }
        }
        std::vector<size_t> x1, x2;
        for (size_t z = 0; z < m; z++) {
            if (!is_ans[z]) {
                if (component[edges[z].from] != component[edges[z].to]) {
                    x1.push_back(z);
                }
                if (has_color[edges[z].color] == std::numeric_limits<size_t>::max()) {
                    x2.push_back(z);
                }
            }
        }
        if (x2.empty()) {
            break;
        }
        std::vector<size_t> dist(m, std::numeric_limits<size_t>::max()), p(m, m);
        std::queue<size_t> q;
        for (size_t x : x1) {
            dist[x] = 0;
            q.push(x);
        }
        while (!q.empty()) {
            size_t v = q.front();
            q.pop();
            for (size_t to : G[v]) {
                if (dist[to] == std::numeric_limits<size_t>::max()) {
                    dist[to] = dist[v] + 1;
                    p[to] = v;
                    q.push(to);
                }
            }
        }
        size_t best_ans = x2[0];
        for (size_t x : x2) {
            if (dist[x] < dist[best_ans]) {
                best_ans = x;
            }
        }
        if (dist[best_ans] == std::numeric_limits<size_t>::max()) {
            break;
        }
        std::vector<edge> new_ans;
        for (size_t u = best_ans;; u = p[u]) {
            if (is_ans[u]) {
                is_ans[u] = false;
            } else {
                is_ans[u] = true;
                new_ans.push_back(edges[u]);
            }
            if (dist[u] == 0) {
                break;
            }
        }
        for (edge &y : ans) {
            if (is_ans[y.num]) {
                new_ans.push_back(y);
            }
        }
        ans = new_ans;
    }
    out << ans.size() << std::endl;
    std::for_each(ans.begin(), ans.end(), [&out](edge &cur_edge) { out << cur_edge.num + 1 << " "; });
    out << std::endl;
    return 0;
}
