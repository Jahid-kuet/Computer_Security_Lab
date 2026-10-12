#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

string int_to_bin(int n)
{
    string res = "";
    while (n)
    {
        res += char((n % 2) + '0');
        n /= 2;
    }

    for (int i = 0; i < res.length() / 2; i++)
    {
        char tmp = res[i];
        res[i] = res[res.length() - (i + 1)];
        res[res.length() - (i + 1)] = tmp;
    }

    return res;
}

int bin_to_int(string bin)
{
    int res = 0;

    for (int i = 0; i < bin.length(); i++)
    {
        res += int(pow(2, bin.length() - i - 1) * int(bin[i] - '0'));
    }

    return res;
}

list<string> str_to_bin(string s)
{
    list<string> res;
    for (int i = 0; i < s.length(); i++)
    {
        int ascii = int(s[i]);

        res.push_back(int_to_bin(ascii));
    }

    return res;
}

list<int> whole_bin_to_int(list<string> decrypt)
{
    list<int> res;
    for (auto it = decrypt.begin(); it != decrypt.end(); it++)
    {
        int val = bin_to_int(*it);
        res.push_back(val);
    }

    return res;
}

string ascii_to_str(list<int> ascii)
{
    string res = "";
    for (auto it = ascii.begin(); it != ascii.end(); it++)
    {
        res += char(*it);
    }

    return res;
}

string rand_key_gen(string s)
{
    string key = "";
    for (int i = 0; i < s.length(); i++)
    {
        int val = rand() % 2;
        key += char(val + '0');
    }

    return key;
}

list<string> xor_str(list<string> str, list<string> key)
{
    list<string> res;

    auto itk = key.begin();
    for (auto its = str.begin(); its != str.end(); its++)
    {
        string intermediate_res = "";
        for (int i = 0; i < (*its).length(); i++)
        {
            if ((*its)[i] == '0' && (*itk)[i] == '0')
                intermediate_res += char('0');
            else if ((*its)[i] == '1' && (*itk)[i] == '1')
                intermediate_res += char('0');
            else
                intermediate_res += char('1');
        }
        res.push_back(intermediate_res);
        itk++;
    }

    return res;
}

int main()
{
    srand(time(nullptr));
    string input = "hello!";
    cout << "Input string: " << input << endl;
    list<string> str = str_to_bin(input);

    // generate a random key for each character in the string
    list<string> key;
    cout << "Key: ";
    for (auto it = str.begin(); it != str.end(); it++)
    {
        string k = rand_key_gen(*it);
        key.push_back(k);
        cout << k << " ";
    }
    cout << endl;

    // encrypt the string using the key
    list<string> encrypted = xor_str(str, key);

    // print the encrypted string
    cout << "Encrypted string: ";
    for (auto it = encrypted.begin(); it != encrypted.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;

    // decrypt the string using the key
    list<string> decrypted = xor_str(encrypted, key);

    // print the decrypted string
    cout << "Decrypted string: ";
    for (auto it = decrypted.begin(); it != decrypted.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;

    // convert the decrypted binary string to ASCII and then to a string
    list<int> decrypted_ascii = whole_bin_to_int(decrypted);
    string decrypted_str = ascii_to_str(decrypted_ascii);

    // print the decrypted strings
    cout << "Decrypted: " << decrypted_str << endl;
}
/*
(A⊕B)⊕B=A
*/