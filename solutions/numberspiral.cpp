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


void solve() {
    int x, y; cin >> y >> x;
    int min_num = (max(x, y)-1)*(max(x, y)-1) + 1;
    int max_num = max(x, y) * max(x, y);
    if (x < y) {
        if (y%2) cout << min_num+x-1 << endl;
        else cout << max_num-x+1 << endl;
    }
    else {
        if (x%2) cout << max_num-y+1 << endl;
        else cout << min_num+y-1 << endl;
    }
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    const bool MANY_TESTS = true;
    int t = 1; if (MANY_TESTS) cin >> t;
    while (t--) solve();
}
