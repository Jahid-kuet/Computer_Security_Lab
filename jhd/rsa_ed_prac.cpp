#include <bits/stdc++.h>
using namespace std;

// a^e mod m
int mod_pow(int a, int e, int m)
{
    int r = 1;
    a %= m;
    while (e)
    {
        if (e & 1)
            r = r * a % m;
        a = a * a % m;
        e >>= 1;
    }
    return r;
}
// gcd(a,b)=ax+by
int egcd(int a, int b, int &x, int &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
// a^-1 mod m
int modinv(int a, int m)
{
    int x, y;
    int g = egcd(a, m, x, y);
    if (g != 1)
        return -1;
    x %= m;
    if (x < 0)
        x += m;
    return x;
}
int main()
{
    int p = 2, q = 2;
    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int e = 17;
    int d = modinv(e, phi);
    int m;
    cin >> m;
    int c = mod_pow(m, e, n);
    int dec = mod_pow(c, d, n);
    cout << dec << endl;
}