
#include <bits/stdc++.h>
using namespace std;

int modpow(int a, int e, int m)
{
    int r = 1;
    a %= m;

    while (e)
    {
        if (e & 1)
            r = (long long)r * a % m;
        a = (long long)a * a % m;
        e >>= 1;
    }
    return r;
}

int modinv(int a, int m)
{
    return modpow(a, m - 2, m);
}

int choose_alpha(int p)
{
    for (int alpha = 2; alpha < p; alpha++)
    {
        set<int> res;

        for (int i = 1; i < p; i++)
            res.insert(modpow(alpha, i, p));

        if (res.size() == p - 1)
            return alpha;
    }
    return -1;
}

int main()
{
    int p;
    cout << "p: ";
    cin >> p;

    int alpha = choose_alpha(p);
    cout << "alpha: " << alpha << endl;

    int a;
    cout << "a: ";
    cin >> a;

    int beta = modpow(alpha, a, p);

    cout << "beta: " << beta << endl;
    cout << "Public key: " << p << " " << alpha << " " << beta << endl;
    cout << "Private key: " << p << " " << a << endl;

    int m;
    cout << "m: ";
    cin >> m;

    int r;
    cout << "r: ";
    cin >> r;

    // Encryption
    int c1 = modpow(alpha, r, p);
    int c2 = (long long)m * modpow(beta, r, p) % p;

    cout << "c1: " << c1 << endl;
    cout << "c2: " << c2 << endl;

    // Decryption
    int s = modpow(c1, a, p);
    int M = (long long)c2 * modinv(s, p) % p;

    cout << "dec msg: " << M << endl;
}
/*
p: 23
a: 6
m: 15
r: 7
*/