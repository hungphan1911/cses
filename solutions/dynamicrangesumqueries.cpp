#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ll long long
#define endl '\n'
const int MOD = 1e9+7;
const int INF = 1e18;

template<class T> using min_heap = priority_queue<T, vector<T>, greater<T>>;
template<class T> using max_heap = priority_queue<T>;

// ===================== Debug =====================
template<typename T> void print(vector<T> x){for(auto i: x) cout << i << ' ';cout << "\n";}
template<typename T> void print(set<T> x){for(auto i: x) cout << i << ' ';cout << "\n";}
template<typename T> void print(unordered_set<T> x){for(auto i: x) cout << i << ' ';cout << "\n";}
template<typename T> void print(T && x) {cout << x << "\n";}
template<typename... Args> void print(Args&&... args) {((cout << args << " "), ...);cout << "\n";}

struct FenwickTree {
    vector<int> tree;
    int n;
    FenwickTree(int n) : n(n) {
        tree.resize(n+1, 0);
    }

    int sum(int k) {
        int s = 0;
        while (k >= 1) {
            s += tree[k];
            k -= k & -k;
        }
        return s;
    }

    void add(int k, int x) {
        while (k <= n) {
            tree[k] += x;
            k += k & -k;
        }
    }

    int query(int l, int r) {
        return sum(r) - sum(l-1);
    }
};

void solve() {
    int n, q; cin >> n >> q;
    FenwickTree tree(n);
    vector<int> cur_val(n);
    for (int i = 0; i < n; i++) {
        int tmp;
        cin >> tmp;
        cur_val[i] = tmp;
        tree.add(i+1, tmp);
    }
    for (int i = 0; i < q; i++) {
        int qtype; cin >> qtype;
        if (qtype == 1) {
            int k, u; cin >> k >> u;
            int incr_val = u - cur_val[k-1];
            cur_val[k-1] = u;
            tree.add(k, incr_val);
        }

        else {
            int l, r; cin >> l >> r;
            cout << tree.query(l, r) << endl;
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    const bool MANY_TESTS = false;
    int t = 1; if (MANY_TESTS) cin >> t;
    while (t--) solve();
}
