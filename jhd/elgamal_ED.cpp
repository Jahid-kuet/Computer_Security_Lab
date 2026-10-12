#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int64 gcd(int64 a, int64 b)
{
    while (b != 0)
    {
        int64 temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int64 mod_pow(int64 a, int64 e, int64 m)
{
    int64 r = 1;
    a %= m;

    while (e)
    {
        if (e & 1)
            r = (__int128)r * a % m;

        a = (__int128)a * a % m;
        e >>= 1;
    }

    return r;
}

int64 egcd(int64 a, int64 b, int64 &x, int64 &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    int64 x1, y1;
    int64 g = egcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
}

int64 modinv(int64 a, int64 m)
{
    int64 x, y;

    int64 g = egcd(a, m, x, y);

    if (g != 1)
        return -1;

    x %= m;

    if (x < 0)
        x += m;

    return x;
}

int main()
{

    // Key Generation
    int64 p = 467;
    int64 alpha = 2;
    int64 privateKey = 127;

    int64 beta = mod_pow(alpha, privateKey, p);

    cout << "Public Key (p, alpha, beta): ("
         << p << ", "
         << alpha << ", "
         << beta << ")\n";

    cout << "Private Key a = "
         << privateKey << "\n\n";

    // Message Input
    int64 M;

    cout << "Enter Message (0 < M < " << p << "): ";
    cin >> M;

    if (M <= 0 || M >= p)
    {
        cout << "Invalid Message\n";
        return 0;
    }

    // Encryption
    int64 r = 11;

    cout << "\nRandom r = "
         << r << "\n";

    int64 C1 = mod_pow(alpha, r, p);

    int64 C2 =
        ((__int128)M *
         mod_pow(beta, r, p)) %
        p;

    cout << "\nCiphertext (C1, C2) = ("
         << C1 << ", "
         << C2 << ")\n";

    // Decryption
    int64 K =
        mod_pow(C1, privateKey, p);

    int64 Kinv =
        modinv(K, p);

    int64 recoveredMessage =
        ((__int128)C2 * Kinv) % p;

    cout << "\nRecovered Message = "
         << recoveredMessage << "\n";

    return 0;
}