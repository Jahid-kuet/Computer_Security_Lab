
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point
{
    ll x, y;
    bool infinity;
};

ll mod(ll a, ll p)
{
    a %= p;
    return a < 0 ? a + p : a;
}

ll egcd(ll a, ll b, ll &x, ll &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    ll x1, y1;
    ll g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll modinv(ll a, ll p)
{
    ll x, y;
    ll g = egcd(a, p, x, y);
    if (g != 1)
        return -1;
    return mod(x, p);
}

Point add(Point P, Point Q, ll a, ll p)
{
    if (P.infinity)
        return Q;
    if (Q.infinity)
        return P;

    if (P.x == Q.x && mod(P.y + Q.y, p) == 0)
        return {0, 0, true};

    ll lambda;

    if (P.x == Q.x && P.y == Q.y)
    {
        if (P.y == 0)
            return {0, 0, true};

        lambda = mod(
            mod(3 * P.x * P.x + a, p) *
                modinv(mod(2 * P.y, p), p),
            p);
    }
    else
    {
        lambda = mod(
            mod(Q.y - P.y, p) *
                modinv(mod(Q.x - P.x, p), p),
            p);
    }

    ll x3 = mod(lambda * lambda - P.x - Q.x, p);
    ll y3 = mod(lambda * (P.x - x3) - P.y, p);

    return {x3, y3, false};
}

Point negatePoint(Point P, ll p)
{
    if (P.infinity)
        return P;
    return {P.x, mod(-P.y, p), false};
}

Point multiply(ll k, Point P, ll a, ll p)
{
    Point result = {0, 0, true};

    while (k > 0)
    {
        if (k & 1)
            result = add(result, P, a, p);

        P = add(P, P, a, p);
        k >>= 1;
    }

    return result;
}

void printPoint(string label, Point P)
{
    cout << label << " = ";
    if (P.infinity)
        cout << "Infinity\n";
    else
        cout << "(" << P.x << ", " << P.y << ")\n";
}

struct Cipher
{
    Point C1, C2;
};

// ECC ElGamal encryption of message point M
Cipher encrypt(Point M, ll k, Point G, Point Y, ll a, ll p)
{
    Point C1 = multiply(k, G, a, p);
    Point kY = multiply(k, Y, a, p);
    Point C2 = add(M, kY, a, p);

    return {C1, C2};
}

// Add two ciphertexts component-wise
Cipher addCipher(Cipher E1, Cipher E2, ll a, ll p)
{
    Point C1 = add(E1.C1, E2.C1, a, p);
    Point C2 = add(E1.C2, E2.C2, a, p);

    return {C1, C2};
}

// Decrypt ciphertext
Point decrypt(Cipher E, ll d, ll a, ll p)
{
    Point shared = multiply(d, E.C1, a, p);
    return add(E.C2, negatePoint(shared, p), a, p);
}

int main()
{
    ll a = 2, b = 2, p = 17;

    // Generator used in the previous example
    Point G = {0, 6, false};

    // Verify generator is on the curve
    if (mod(G.y * G.y, p) !=
        mod(G.x * G.x * G.x + a * G.x + b, p))
    {
        cout << "Invalid generator point\n";
        return 0;
    }

    ll d = 5; // Private key
    Point Y = multiply(d, G, a, p);

    ll m1, m2, k1, k2;
    cout << "Enter messages m1 and m2: ";
    cin >> m1 >> m2;

    cout << "Enter random keys k1 and k2: ";
    cin >> k1 >> k2;

    // Encode integers as elliptic curve points
    Point M1 = multiply(m1, G, a, p);
    Point M2 = multiply(m2, G, a, p);

    // Encrypt both messages
    Cipher E1 = encrypt(M1, k1, G, Y, a, p);
    Cipher E2 = encrypt(M2, k2, G, Y, a, p);

    printPoint("M1", M1);
    printPoint("M2", M2);

    printPoint("E1.C1", E1.C1);
    printPoint("E1.C2", E1.C2);
    printPoint("E2.C1", E2.C1);
    printPoint("E2.C2", E2.C2);

    // Homomorphic addition
    Cipher E_sum = addCipher(E1, E2, a, p);

    printPoint("Sum ciphertext C1", E_sum.C1);
    printPoint("Sum ciphertext C2", E_sum.C2);

    // Decrypt the sum ciphertext
    Point decryptedSum = decrypt(E_sum, d, a, p);

    // Expected point: (m1 + m2)G
    Point expectedSum = multiply(m1 + m2, G, a, p);

    printPoint("Decrypted sum", decryptedSum);
    printPoint("Expected (m1+m2)G", expectedSum);

    if (decryptedSum.infinity == expectedSum.infinity && (decryptedSum.infinity || (decryptedSum.x == expectedSum.x && decryptedSum.y == expectedSum.y)))
    {
        cout << "Homomorphic addition verified!\n";
    }
    else
    {
        cout << "Verification failed.\n";
    }

    return 0;
}
/*
Enter messages m1 and m2: 3 4
Enter random keys k1 and k2: 2 3
*/