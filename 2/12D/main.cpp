#include <fstream>
#include <map>
#include <vector>

bool check_parent(uint64_t num, std::map<uint64_t, bool> &is_independent) {
    for (size_t i = 0, j = 1; i <= 10; i++, j *= 2) {
        if ((num & j) > 0) {
            if (!is_independent[num - j]) {
                return false;
            }
        }
    }
    return true;
}

bool check_third(uint64_t a, uint64_t b, std::map<uint64_t, bool> &is_independent) {
    bool used = false;
    for (size_t i = 0, j = 1; i <= 10; i++, j *= 2) {
        if ((a & j) > 0 && (b & j) == 0) {
            used = true;
            if (is_independent[b + j]) {
                return true;
            }
        }
    }
    return !used;
}

int main() {
    std::ifstream in("check.in");
    std::ofstream out("check.out");
    size_t n, m;
    in >> n >> m;
    if (m == 0) {
        out << "NO" << std::endl;
        return 0;
    }
    std::vector<uint64_t> pow_2(11);
    pow_2[0] = 1;
    for (size_t i = 1; i <= 10; i++) {
        pow_2[i] = pow_2[i - 1] * 2;
    }
    std::map<uint64_t, bool> is_independent;
    std::map<uint64_t, size_t> size;
    std::vector<uint64_t> numbers;
    while (m-- > 0) {
        size_t k;
        in >> k;
        uint64_t num = 0;
        for (size_t j = 0; j < k; j++) {
            size_t i;
            in >> i;
            i--;
            num += pow_2[i];
        }
        size[num] = k;
        is_independent[num] = true;
        numbers.push_back(num);
    }
    for (uint64_t num : numbers) {
        if (!check_parent(num, is_independent)) {
            out << "NO" << std::endl;
            return 0;
        }
        for (uint64_t another_num : numbers) {
            if (size[num] > size[another_num] && !check_third(num, another_num, is_independent)) {
                out << "NO" << std::endl;
                return 0;
            }
        }
    }
    out << "YES" << std::endl;
    return 0;
}
