#include <iostream>
#include <vector>
#include <string>

using namespace std;

//--------------------------------------------------
// Manual GCD
//--------------------------------------------------
long long gcd(long long a, long long b)
{
    while (b != 0)
    {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

//--------------------------------------------------
// Modular Multiplication
//--------------------------------------------------
long long modMultiply(long long a, long long b, long long mod)
{
    long long result = 0;

    while (b > 0)
    {
        if (b % 2 == 1)
            result = (result + a) % mod;

        a = (a + a) % mod;
        b = b / 2;
    }

    return result;
}

//--------------------------------------------------
// Modular Exponentiation
//--------------------------------------------------
long long modPower(long long base,
                   long long exponent,
                   long long mod)
{
    long long result = 1;

    base = base % mod;

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            result = modMultiply(result, base, mod);

        base = modMultiply(base, base, mod);

        exponent = exponent / 2;
    }

    return result;
}

//--------------------------------------------------
// Extended Euclidean Algorithm
//--------------------------------------------------
long long extendedGCD(long long a,
                      long long b,
                      long long &x,
                      long long &y)
{
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }

    long long x1, y1;

    long long g =
        extendedGCD(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return g;
}

//--------------------------------------------------
// Modular Inverse
//--------------------------------------------------
long long modularInverse(long long e,
                         long long phi)
{
    long long x, y;

    long long g =
        extendedGCD(e, phi, x, y);

    if (g != 1)
        return -1;

    while (x < 0)
        x += phi;

    return x % phi;
}

//--------------------------------------------------
// Prime Checking
//--------------------------------------------------
bool isPrime(long long n)
{
    if (n < 2)
        return false;

    for (long long i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

//--------------------------------------------------
// Main
//--------------------------------------------------
int main()
{
    long long p, q;

    cout << "Enter first prime : ";
    cin >> p;

    cout << "Enter second prime : ";
    cin >> q;

    if (!isPrime(p) || !isPrime(q))
    {
        cout << "Both numbers must be prime.\n";
        return 0;
    }

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    cout << "\nModulus n = " << n << endl;
    cout << "Euler Totient = " << phi << endl;

    long long e;

    cout << "\nEnter public exponent (0 = auto): ";
    cin >> e;

    if (e == 0)
    {
        e = 2;

        while (gcd(e, phi) != 1)
            e++;

        cout << "Selected e = " << e << endl;
    }
    else
    {
        if (gcd(e, phi) != 1)
        {
            cout << "Invalid public exponent.\n";
            return 0;
        }
    }

    long long d = modularInverse(e, phi);

    cout << "Private exponent d = " << d << endl;

    cin.ignore();

    string message;

    cout << "\nEnter plaintext: ";
    getline(cin, message);

    vector<long long> cipher;

    cout << "\nASCII Values:\n";

    for (int i = 0; i < message.length(); i++)
    {
        cout << (int)message[i] << " ";
    }

    cout << "\n\nEncrypted Values:\n";

    for (int i = 0; i < message.length(); i++)
    {
        long long value = (int)message[i];

        long long encrypted =
            modPower(value, e, n);

        cipher.push_back(encrypted);

        cout << encrypted << " ";
    }

    cout << "\n\nDecrypted Text:\n";

    string recovered = "";

    for (int i = 0; i < cipher.size(); i++)
    {
        long long decrypted =
            modPower(cipher[i], d, n);

        recovered += (char)decrypted;
    }

    cout << recovered << endl;

    return 0;
}