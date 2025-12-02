#include <bits/stdc++.h>
#define int long long
using namespace std;
const int N = 3e5 + 10, INF = 0x3f3f3f3f3f3f3f;

signed main() {
    ios::sync_with_stdio(false), cin.tie(0);
    int n = 32, m = 32;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int idx = (i * n + j) >> 3;
            cout << idx % 32 << ' ';
            if (idx % 32 < 10)
                cout << ' ';
        }
        cout << '\n';
    }
}