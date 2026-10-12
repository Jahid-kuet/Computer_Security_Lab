
#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

// Modular exponentiation
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

// Extended Euclidean Algorithm
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

// Modular inverse
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
    // Key generation
    int64 p = 467;
    int64 alpha = 2;
    int64 a = 127; // Private key

    int64 beta = mod_pow(alpha, a, p);

    cout << "Public Key (p, alpha, beta): ("
         << p << ", " << alpha << ", "
         << beta << ")\n";

    cout << "Private Key a = " << a << "\n";

    int64 M;
    cout << "\nEnter message (0 < M < " << p << "): ";
    cin >> M;

    if (M <= 0 || M >= p)
    {
        cout << "Invalid message\n";
        return 0;
    }

    // 1. ElGamal Encryption
    int64 r = 11;

    int64 C1 = mod_pow(alpha, r, p);
    int64 C2 = (__int128)M * mod_pow(beta, r, p) % p;

    cout << "\n--- Encryption ---\n";
    cout << "Random r = " << r << "\n";
    cout << "Ciphertext (C1, C2) = ("
         << C1 << ", " << C2 << ")\n";

    // 2. ElGamal Digital Signature
    int64 k = 5;

    if (__gcd(k, p - 1) != 1)
    {
        cout << "Signature key k is not coprime to p-1\n";
        return 0;
    }

    int64 y1 = mod_pow(alpha, k, p);
    int64 k_inv = modinv(k, p - 1);

    int64 y2 = (k_inv * (M - a * y1)) % (p - 1);
    if (y2 < 0)
        y2 += p - 1;

    cout << "\n--- Digital Signature ---\n";
    cout << "Signature (y1, y2) = ("
         << y1 << ", " << y2 << ")\n";

    // 3. Decryption
    int64 shared = mod_pow(C1, a, p);
    int64 shared_inv = modinv(shared, p);

    int64 recovered = (__int128)C2 * shared_inv % p;

    cout << "\n--- Decryption ---\n";
    cout << "Recovered message = " << recovered << "\n";

    // 4. Signature Verification
    int64 left = mod_pow(alpha, M, p);

    int64 right = (__int128)mod_pow(beta, y1, p) * mod_pow(y1, y2, p) % p;

    cout << "\n--- Signature Verification ---\n";

    if (left == right)
        cout << "Signature VALID\n";
    else
        cout << "Signature INVALID\n";

    return 0;
}