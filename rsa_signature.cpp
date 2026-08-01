#include <iostream>
using namespace std;

typedef long long int64;

// Safe Modular Multiplication
int64 mul_mod(int64 a, int64 b, int64 mod)
{
    int64 result = 0;

    a %= mod;

    while (b > 0)
    {
        if (b % 2 == 1)
            result = (result + a) % mod;

        a = (a + a) % mod;
        b /= 2;
    }

    return result;
}

// Fast Modular Exponentiation
int64 mod_pow(int64 base, int64 exp, int64 mod)
{
    int64 result = 1;

    base %= mod;

    while (exp > 0)
    {
        if (exp % 2 == 1)
            result = mul_mod(result, base, mod);

        base = mul_mod(base, base, mod);
        exp /= 2;
    }

    return result;
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

    int64 gcd = egcd(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;
}

// Modular Inverse
int64 modinv(int64 a, int64 mod)
{
    int64 x, y;

    egcd(a, mod, x, y);

    while (x < 0)
        x += mod;

    return x % mod;
}

int main()
{
    // Key Generation
    int64 p = 61;
    int64 q = 53;

    int64 n = p * q;
    int64 phi = (p - 1) * (q - 1);

    int64 e = 17;
    int64 d = modinv(e, phi);

    cout << "Public Key (n,e): (" << n << "," << e << ")" << endl;
    cout << "Private Key d: " << d << endl;

    // Message
    int64 message;

    cout << "\nEnter Message: ";
    cin >> message;

    // Signing
    int64 signature = mod_pow(message, d, n);

    cout << "\nDigital Signature = " << signature << endl;

    // Verification
    int64 recovered = mod_pow(signature, e, n);

    cout << "Recovered Message = " << recovered << endl;

    if (recovered == message)
        cout << "Signature Verified (Valid)" << endl;
    else
        cout << "Signature Verification Failed (Invalid)" << endl;

    return 0;
}