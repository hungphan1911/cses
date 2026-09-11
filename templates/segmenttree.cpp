#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

// use case: dynamic range queries
// query: logn, update: logn
// example: https://cses.fi/problemset/task/1649
struct SegmentTree {
    int n;
    vector<int> tree;
    SegmentTree(int n) : n(n) { 
        tree.assign(2LL*n, 0);
    }

    void add(int k, int x) {
        k += n;
        tree[k] += x;
        for (k /= 2; k >= 1; k /= 2) {
            tree[k] = min(tree[2*k], tree[2*k+1]);
        }
    }

    int get_min(int a, int b) {
        a += n;
        b += n;
        int res = 1e10;
        while (a <= b) {
            if (a%2 == 1) res = min(tree[a++], res); 
            if (b%2 == 0) res = min(tree[b--], res); 
            a /= 2; b /= 2;
        }

        return res;
    }
};

// Actual solve method
void solve() {
    int n, q; cin >> n >> q;
    SegmentTree st(n);
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        int tmp; cin >> tmp;
        a[i] = tmp;
        st.add(i, tmp);
    }

    for (int i = 0; i < q; i++) {
        int t, x, y; cin >> t >> x >> y;
        if (t == 1) {
            x--;
            int delta = y - a[x];
            a[x] = y;
            st.add(x, delta);
        }
        else {
            x--; y--;
            cout << st.get_min(x, y) << endl;
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    const bool MANY_TESTS = false;
    int t = 1;
    if (MANY_TESTS) cin >> t;
    while (t--) solve();
}