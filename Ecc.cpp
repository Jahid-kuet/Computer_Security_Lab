#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
struct Point
{
    ll x;
    ll y;
    bool infinity;
};
ll mod(ll a, ll m)
{
    a %= m;
    if (a < 0)
        a += m;
    return a;
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
ll modinv(ll e, ll phi)
{
    ll x, y;
    ll g = egcd(e, phi, x, y);
    if (g != 1)
    {
        return -1;
    }
    return mod(x, phi);
}
bool isPointOnCurve(Point P, ll a, ll b, ll p)
{
    if (P.infinity)
        return true;
    ll left = mod(P.y * P.y, p);
    ll right = mod(P.x * P.x * P.x + a * P.x + b, p);
    return left == right;
}

Point add(Point P, Point Q, ll a, ll p)
{
    if (P.infinity)
        return Q;
    if (Q.infinity)
        return P;
    if (P.x == Q.x && mod(P.y + Q.y, p) == 0)
    {
        return {0, 0, true};
    }
    ll lambda;
    if (P.x == Q.x && P.y == Q.y)
    {
        if (P.y == 0)
        {
            return {0, 0, true};
        }
        ll numerator = mod(3 * P.x * P.x + a, p);
        ll denominator = mod(2 * P.y, p);
        lambda = mod(numerator * modinv(denominator, p), p);
    }
    else
    {
        ll numerator = mod(Q.y - P.y, p);
        ll denominator = mod(Q.x - P.x, p);
        lambda = mod(numerator * modinv(denominator, p), p);
    }
    ll x3 = mod(lambda * lambda - P.x - Q.x, p);
    ll y3 = mod(lambda * (P.x - x3) - P.y, p);
    return {x3, y3, false};
}
Point Negate(Point P, ll p)
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
        if (k % 2 == 1)
            result = add(result, P, a, p);
        P = add(P, P, a, p);
        k /= 2;
    }
    return result;
}
int main()
{
    ll a, b, p;
    cout << "Enter curve parameters a, b, and prime p: ";
    cin >> a >> b >> p;
    // ll gx, gy;
    // cin >> gx >> gy;
    // Point G = {gx, gy, false};
    // if (!isPointOnCurve(G, a, b, p)) {
    //     cout << "Generator point is not on the curve" << endl;
    //     return 0;
    // }
    Point G;
    bool found = false;
    for (ll x = 0; x < p; x++)
    {
        for (ll y = 0; y < p; y++)
        {
            Point temp = {x, y, false};
            if (isPointOnCurve(temp, a, b, p))
            {
                G = temp;
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    if (!found)
    {
        cout << "No valid generator point found on the curve" << endl;
        return 0;
    }
    cout << "Generator point G = (" << G.x << ", " << G.y << ")" << endl;

    ll d;
    cout << "Enter private key d: ";
    cin >> d;
    Point Y = multiply(d, G, a, p);

    ll mx, my;
    cout << "Enter message point coordinates mx and my: ";
    cin >> mx >> my;
    Point M = {mx, my, false};
    if (!isPointOnCurve(M, a, b, p))
    {
        cout << "Message point is not on the curve" << endl;
        return 0;
    }

    ll k;
    cout << "Enter random key k: ";
    cin >> k;
    Point C1 = multiply(k, G, a, p);
    Point kY = multiply(k, Y, a, p);
    Point C2 = add(M, kY, a, p);
    cout << C1.x << " " << C1.y << endl;
    cout << C2.x << " " << C2.y << endl;

    // Decryption
    Point dC1 = multiply(d, C1, a, p);
    Point negdC1 = Negate(dC1, p);
    Point decryptedM = add(C2, negdC1, a, p);
    cout << decryptedM.x << " " << decryptedM.y << endl;
}