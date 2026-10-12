#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

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
         << p << ", " << alpha << ", " << beta << ")\n";

    cout << "Private Key a = " << privateKey << "\n";

    // Random value
    int64 r = 11;

    cout << "Random r = " << r << "\n";

    // C1 = alpha^r mod p
    int64 C1 = mod_pow(alpha, r, p);

    // K = beta^r mod p
    int64 K = mod_pow(beta, r, p);

    // Same K obtained during decryption
    // K = C1^a mod p
    int64 K2 = mod_pow(C1, privateKey, p);

    int64 Kinv = modinv(K2, p);

    cout << "\nC1 = " << C1 << "\n";
    cout << "Shared K = " << K2 << "\n";
    cout << "K inverse = " << Kinv << "\n";

    // =====================================================
    // CASE 1: C2 WITHOUT MESSAGE
    // =====================================================
    // Message
    int64 M;

    cout << "\nEnter message M: ";
    cin >> M;

    if (M <= 0)
    {
        cout << "Message must be positive.\n";
        return 0;
    }
    int64 C2 = K;

    cout << "\nCase 1: Without message in C2\n";

    cout << "Ciphertext = (" << C1 << ", " << C2 << ")\n";

    int64 dec1 = (__int128)C2 * Kinv % p;

    cout << "Decrypted output = " << dec1 << "\n";

    // =====================================================
    // CASE 2: NORMAL ELGAMAL
    // =====================================================

    C2 = (__int128)(M % p) * K % p;

    cout << "\nCase 2: Normal ElGamal\n";

    cout << "Input M = " << M << "\n";

    cout << "Ciphertext = (" << C1 << ", " << C2 << ")\n";

    int64 dec2 = (__int128)C2 * Kinv % p;

    cout << "Decrypted output = " << dec2 << "\n";

    cout << "Expected M mod p = " << M % p << "\n";

    // =====================================================
    // CASE 3: M > p
    // =====================================================

    if (M > p)
    {
        C2 = (__int128)(M % p) * K % p;

        cout << "\nCase 3: M > p\n";

        cout << "Input M = " << M << "\n";

        cout << "Ciphertext = (" << C1 << ", " << C2 << ")\n";

        int64 dec3 = (__int128)C2 * Kinv % p;

        cout << "Decrypted output = " << dec3 << "\n";

        cout << "Expected M mod p = " << M % p << "\n";
    }

    return 0;
}