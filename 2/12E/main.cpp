#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

int main() {
    std::ifstream in("cycles.in");
    std::ofstream out("cycles.out");
    int32_t ans = 0;
    size_t n, m;
    in >> n >> m;
    std::vector<std::pair<int32_t, size_t> > w(n);
    for (size_t i = 0; i < n; i++) {
        in >> w[i].first;
        w[i].second = i;
    }
    std::sort(w.rbegin(), w.rend());
    std::vector<int32_t> pow_2(n + 1);
    pow_2[0] = 1;
    for (size_t i = 1; i <= n; i++) {
        pow_2[i] = pow_2[i - 1] * 2;
    }
    std::vector<int32_t> dependant;
    while (m-- > 0) {
        int32_t num = 0;
        size_t k;
        in >> k;
        while (k-- > 0) {
            size_t i;
            in >> i;
            i--;
            num += pow_2[i];
        }
        dependant.push_back(num);
    }
    size_t cur_num = 0;
    for (size_t i = 0; i < n; i++) {
        bool yes = true;
        cur_num += pow_2[w[i].second];
        for (int32_t j : dependant) {
            if ((cur_num & j) == j) {
                yes = false;
                break;
            }
        }
        if (yes) {
            ans += w[i].first;
        } else {
            cur_num -= pow_2[w[i].second];
        }
    }
    out << ans << std::endl;
    return 0;
}