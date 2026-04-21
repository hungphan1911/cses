#include <bits/stdc++.h>
using namespace std;
#define int long long
#define endl '\n'
#define ll long long

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
    int n; cin >> n;
    if (n == 1) {
        cout << 1 << endl;
        return;
    }
    if (n == 4) {
        cout << "2 4 1 3" << endl;
        return;
    }
    if (n <= 4) {
        cout << "NO SOLUTION" << endl;
        return;
    }
    vector<int> a(n);
    int cur = 1;
    for (int i = 0; i < n; i+=2) {
        a[i] = cur;
        cur++;
    }
    for (int i = 1; i < n; i+=2) {
        a[i] = cur;
        cur++;
    }

    print(a);
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    const bool MANY_TESTS = false;
    int t = 1; if (MANY_TESTS) cin >> t;
    while (t--) solve();
}
