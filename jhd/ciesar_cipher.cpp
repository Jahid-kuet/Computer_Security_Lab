#include <iostream>
#include <string>
using namespace std;

string encrypt(string text, int s)
{
    string result = "";
    s = s % 26;

    for (char ch : text)
    {
        if (ch >= 'A' && ch <= 'Z')
            result += char((ch - 'A' + s) % 26 + 'a');

        else if (ch >= 'a' && ch <= 'z')
            result += char((ch - 'a' + s) % 26 + 'A');

        else if (ch >= '0' && ch <= '9')
            result += char((ch - '0' + s) % 10 + '0');

        else
            result += ch;
    }

    return result;
}

string decrypt(string text, int s)
{
    string result = "";
    s = s % 26;

    for (char ch : text)
    {
        if (ch >= 'a' && ch <= 'z')
            result += char((ch - 'a' - s + 26) % 26 + 'A');

        else if (ch >= 'A' && ch <= 'Z')
            result += char((ch - 'A' - s + 26) % 26 + 'a');

        else if (ch >= '0' && ch <= '9')
            result += char((ch - '0' - (s % 10) + 10) % 10 + '0');

        else
            result += ch;
    }

    return result;
}

int main()
{
    string text;
    int s;

    cout << "Enter text: ";
    getline(cin, text);

    cout << "Enter shift: ";
    cin >> s;

    string enc = encrypt(text, s);

    cout << "Encrypted message: " << enc << "\n";
    cout << "Decrypted message: " << decrypt(enc, s) << "\n";

    return 0;
}