
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
    // RSA key generation
    int64 p = 61, q = 53;

    int64 n = p * q;
    int64 phi = (p - 1) * (q - 1);

    int64 e = 17;
    int64 d = modinv(e, phi);

    cout << "Public key (n, e): (" << n << ", "
         << e << ")\n";
    cout << "Private key d: " << d << "\n";

    int64 M;
    cout << "Enter message (integer, 0 <= M < n): ";
    cin >> M;

    if (M < 0 || M >= n)
    {
        cout << "Invalid message. Message must be in [0, n-1].\n";
        return 0;
    }

    // 1. Encryption using public key
    int64 C = mod_pow(M, e, n);
    cout << "\n--- Encryption ---\n";
    cout << "Ciphertext C = " << C << "\n";

    // 2. Digital signature using private key
    int64 S = mod_pow(M, d, n);
    cout << "\n--- Digital Signature ---\n";
    cout << "Signature S = " << S << "\n";

    // 3. Decryption using private key
    int64 decrypted = mod_pow(C, d, n);
    cout << "\n--- Decryption ---\n";
    cout << "Decrypted message = " << decrypted << "\n";

    // 4. Signature verification using public key
    int64 V = mod_pow(S, e, n);
    cout << "\n--- Signature Verification ---\n";
    cout << "Verification result = " << V << "\n";

    if (V == M)
        cout << "Signature is VALID\n";
    else
        cout << "Signature is INVALID\n";

    return 0;
}