#include <iostream>
using namespace std;

typedef long long int64;

// Safe Modular Multiplication
int64 mul_mod(int64 a, int64 b, int64 mod)
{
    int64 result = 0;

    a = a % mod;

    while (b > 0)
    {
        if (b % 2 == 1)
        {
            result = result + a;

            if (result >= mod)
                result = result % mod;
        }

        a = a + a;

        if (a >= mod)
            a = a % mod;

        b = b / 2;
    }

    return result;
}

// Fast Modular Exponentiation
int64 mod_pow(int64 base, int64 exp, int64 mod)
{
    int64 result = 1;

    base = base % mod;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = mul_mod(result, base, mod);
        }

        base = mul_mod(base, base, mod);

        exp = exp / 2;
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

    int64 gcd = egcd(a, mod, x, y);

    if (gcd != 1)
        return -1;

    while (x < 0)
        x = x + mod;

    return x % mod;
}

int main()
{
    int64 p = 61;
    int64 q = 53;

    int64 n = p * q;
    int64 phi = (p - 1) * (q - 1);

    int64 e = 17;
    int64 d = modinv(e, phi);

    cout << "Public Key (n,e): (" << n << "," << e << ")" << endl;
    cout << "Private Key d: " << d << endl;

    int64 m1 = 65;
    int64 m2 = 77;

    int64 c1 = mod_pow(m1, e, n);
    int64 c2 = mod_pow(m2, e, n);

    cout << endl;
    cout << "Ciphertext 1 = " << c1 << endl;
    cout << "Ciphertext 2 = " << c2 << endl;

    int64 productCipher = mul_mod(c1, c2, n);

    cout << "Product Ciphertext = " << productCipher << endl;

    int64 decrypted = mod_pow(productCipher, d, n);

    cout << "Decrypted Product = " << decrypted << endl;

    cout << "Expected = " << mul_mod(m1, m2, n) << endl;

    return 0;
}