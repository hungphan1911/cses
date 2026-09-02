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

class SparseTable {
public:
    vector<vector<int>> spT;
    vector<int> log;

    SparseTable(vector<int>& a) {
        int n = a.size();

        log.resize(n + 1);
        log[1] = 0;
        for (int i = 2; i <= n; i++)
            log[i] = log[i / 2] + 1;

        int K = log[n];
        spT.assign(K + 1, vector<int>(n));
        spT[0] = a;

        for (int i = 1; i <= K; i++) {
            for (int j = 0; j + (1 << i) <= n; j++) {
                spT[i][j] = min(spT[i-1][j], spT[i-1][j + (1 << (i - 1))]);
            }
        }
    }

    int query(int L, int R) {
        int i = log[R - L + 1];
        return min(spT[i][L], spT[i][R - (1 << i) + 1]);
    }
};

void solve() {
    int n, q; cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    SparseTable st(a);
    for (int i = 0; i < q; i++) {
        int l, r; cin >> l >> r;
        l--; r--;
        cout << st.query(l, r) << endl;
    }
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    const bool MANY_TESTS = false;
    int t = 1; if (MANY_TESTS) cin >> t;
    while (t--) solve();
}
