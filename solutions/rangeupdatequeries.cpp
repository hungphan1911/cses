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

// Actual solve method
void solve() {
    int n, q; cin >> n >> q;
    FenwickTree ft(n+1);
    vector<int> a(n);
    vector<int> diff(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        if (i == 0) diff[i] = a[i];
        else diff[i] = a[i] - a[i-1];
        ft.add(i+1, diff[i]);
    }

    for (int i = 0; i < q; i++) {
        int ty; cin >> ty;
        if (ty == 1) {
            int a, b, u; cin >> a >> b >> u;
            ft.add(a, u);
            ft.add(b+1, -u);
        } else {
            int k; cin >> k;
            cout << ft.sum(k) << endl;
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