#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

using ll = long long;
using ld = long double;
using pii = pair<int,int>;
using pll = pair<ll,ll>;
const ll MOD = 1e9+7;
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define sz(x) (int)(x).size()

template<typename T> using min_heap=priority_queue<T,vector<T>,greater<T>>;
template<typename T> using max_heap=priority_queue<T>;

// Debug helper
template<typename T> void print(vector<T> x){for(auto i: x) cout << i << ' ';cout << "\n";}
template<typename T> void print(set<T> x){for(auto i: x) cout << i << ' ';cout << "\n";}
template<typename T> void print(unordered_set<T> x){for(auto i: x) cout << i << ' ';cout << "\n";}
template<typename T> void print(T && x) {cout << x << "\n";}
template<typename... Args> void print(Args&&... args) {((cout << args << " "), ...);cout << "\n";}

// Some math
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll lcm(ll a, ll b) { return a / gcd(a,b) * b; }
ll binpow(ll a, ll b, ll mod = MOD) { ll res = 1; while (b) { if (b & 1) res = res * a % mod; a = a * a % mod; b >>= 1; } return res; }
ll modinv(ll a, ll mod = MOD) { return binpow(a, mod - 2, mod); }

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