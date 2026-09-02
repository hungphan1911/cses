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


// Actual solve method
void solve() {
    
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    const bool MANY_TESTS = false;
    int t = 1;
    if (MANY_TESTS) cin >> t;
    while (t--) solve();
}