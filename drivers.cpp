//Created by Denys Zinoviev all rights reserved
#include <iostream>
#include <vector>

using namespace std;

struct point {
    int x;
    int y;
};

int main() {
    vector <point> drivers;
    vector <point> cafe;
    vector <point> orders;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        cafe.push_back({x, y}); 
    }
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        drivers.push_back({x, y}); 
    }
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        orders.push_back({x, y}); 
    }
}
