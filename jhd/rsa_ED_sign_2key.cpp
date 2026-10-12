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
    // RSA KEY PAIR 1: Encryption / Decryption
    int64 p1 = 61, q1 = 53;
    int64 n1 = p1 * q1;
    int64 phi1 = (p1 - 1) * (q1 - 1);

    int64 e1 = 17;
    int64 d1 = modinv(e1, phi1);

    // RSA KEY PAIR 2: Digital Signature / Verification
    int64 p2 = 47, q2 = 59;
    int64 n2 = p2 * q2;
    int64 phi2 = (p2 - 1) * (q2 - 1);

    int64 e2 = 13;
    int64 d2 = modinv(e2, phi2);

    cout << "--- Encryption Keys ---\n";
    cout << "Public key (n1, e1): ("
         << n1 << ", " << e1 << ")\n";
    cout << "Private key d1: " << d1 << "\n";

    cout << "\n--- Signature Keys ---\n";
    cout << "Public key (n2, e2): ("
         << n2 << ", " << e2 << ")\n";
    cout << "Private key d2: " << d2 << "\n";

    int64 M;
    cout << "\nEnter message (integer): ";
    cin >> M;

    // The message must fit both RSA moduli
    if (M < 0 || M >= min(n1, n2))
    {
        cout << "Invalid message. M must be in [0, "
             << min(n1, n2) - 1 << "].\n";
        return 0;
    }

    // 1. Encryption using encryption public key
    int64 C = mod_pow(M, e1, n1);

    cout << "\n--- Encryption ---\n";
    cout << "Ciphertext C = " << C << "\n";

    // 2. Digital signature using signature private key
    int64 S = mod_pow(M, d2, n2);

    cout << "\n--- Digital Signature ---\n";
    cout << "Signature S = " << S << "\n";

    // 3. Decryption using encryption private key
    int64 decrypted = mod_pow(C, d1, n1);

    cout << "\n--- Decryption ---\n";
    cout << "Decrypted message = " << decrypted << "\n";

    // 4. Signature verification using signature public key
    int64 V = mod_pow(S, e2, n2);

    cout << "\n--- Signature Verification ---\n";
    cout << "Verification result = " << V << "\n";

    if (V == M)
        cout << "Signature is VALID\n";
    else
        cout << "Signature is INVALID\n";

    return 0;
}