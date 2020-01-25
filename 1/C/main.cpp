#include <iostream>
#include <vector>
#include <algorithm>

size_t cnt = 0;

bool ask(size_t a, size_t b, std::vector<std::vector<char> > &g) {
    if (g[a][b] == '-') {
        cnt++;
        std::cout << "1 " << a + 1 << " " << b + 1 << std::endl;
        std::string ans;
        std::cin >> ans;
        if (ans == "YES") {
            g[a][b] = 'Y';
            g[b][a] = 'N';
        } else {
            g[a][b] = 'N';
            g[b][a] = 'Y';
        }
    }
    return g[a][b] == 'Y';
}

int main() {
    size_t n;
    std::cin >> n;
    if (n == 1) {
        std::cout << "0 1" << std::endl;
        return 0;
    }
    std::vector<std::vector<char> > g(n, std::vector<char>(n, '-'));
    std::vector<size_t> v(n);
    for (size_t i = 0; i < n; i++) {
        v[i] = i;
    }
    for (size_t i = 1; i < n; i++) {
        if (ask(v[i - 1], v[i], g)) {
            continue;
        }
        ptrdiff_t l = -1,
                r = i;
        while (l + 1 < r) {
            size_t mid = (l + r) / 2;
            if (ask(v[mid], v[i], g)) {
                l = mid;
            } else {
                r = mid;
            }
        }
        v.insert(v.begin() + l + 1, v[i]);
        v.erase(v.begin() + i + 1);
    }
    std::cout << 0 << " ";
    std::for_each(v.begin(), v.end(), [](size_t v) { std::cout << v + 1 << " "; });
    std::cout << std::endl;
    return 0;
}
