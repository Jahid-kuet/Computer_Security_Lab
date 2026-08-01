#include <iostream>
using namespace std;

//--------------------------------------------------
// Find GCD manually
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
// Modular multiplication
// (avoids overflow using repeated addition)
//--------------------------------------------------
long long modMultiply(long long a, long long b, long long mod)
{
    long long result = 0;

    while (b > 0)
    {
        if (b % 2 == 1)
        {
            result = (result + a) % mod;
        }

        a = (a + a) % mod;
        b = b / 2;
    }

    return result;
}

//--------------------------------------------------
// Modular exponentiation
//--------------------------------------------------
long long modPower(long long base, long long exponent, long long mod)
{
    long long result = 1;

    base = base % mod;

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {
            result = modMultiply(result, base, mod);
        }

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

    long long gcdValue =
        extendedGCD(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcdValue;
}

//--------------------------------------------------
// Modular inverse
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

    x = x % phi;

    return x;
}

//--------------------------------------------------
// Prime checking
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

    //------------------------------------------------

    long long n = p * q;

    long long phi =
        (p - 1) * (q - 1);

    cout << "\nModulus n = " << n << endl;
    cout << "Euler Totient = " << phi << endl;

    //------------------------------------------------

    long long e;

    cout << "\nEnter public exponent (0 = auto): ";
    cin >> e;

    if (e == 0)
    {
        e = 2;

        while (gcd(e, phi) != 1)
        {
            e++;
        }

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

    //------------------------------------------------

    long long d =
        modularInverse(e, phi);

    if (d == -1)
    {
        cout << "Private key cannot be found.\n";
        return 0;
    }

    cout << "\nPrivate exponent d = "
         << d << endl;

    //------------------------------------------------

    cout << "\nSome possible d values:\n";

    int count = 0;

    for (long long k = 1;
         k < phi && count < 8;
         k++)
    {
        long long value = 1 + k * phi;

        if (value % e == 0)
        {
            cout << value / e << " ";
            count++;
        }
    }

    cout << endl;

    //------------------------------------------------

    long long message;

    cout << "\nEnter plaintext : ";
    cin >> message;

    long long cipher =
        modPower(message, e, n);

    cout << "Ciphertext = "
         << cipher << endl;

    //------------------------------------------------

    cout << "\nSearching private exponent...\n";

    for (long long i = 1;
         i <= phi;
         i++)
    {
        if (modPower(cipher, i, n) == message)
        {
            cout << "Possible d = "
                 << i << endl;

            break;
        }
    }

    //------------------------------------------------

    long long original =
        modPower(cipher, d, n);

    cout << "\nRecovered plaintext = "
         << original << endl;

    return 0;
}