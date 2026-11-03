#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

vector<int> dectobin(int n)
{
    vector<int> zero(1, 0);
    vector<int> binary;
    if (n == 0) return zero;
    while (n > 0) {
        int remainder = n % 2;
        binary.push_back(remainder);
        n = n / 2;
    }
    reverse(binary.begin(), binary.end());
    return binary;
}

vector<int> twoscompliment(vector<int> v)
{
    for (size_t i = 0; i < v.size(); i++) {
        v[i] = (v[i] == 0) ? 1 : 0;
    }
    int carry = 1;
    int base = 2;

    for (int i = v.size() - 1; i >= 0; i--) {
        int total = v[i] + carry;
        v[i] = total % base;
        carry = total / base;
    }
    return v;
}

int main()
{
    int m, q;
    cout << "Enter the unsigned dividend (Q): ";
    cin >> q;
    cout << "Enter the unsigned divisor (M): ";
    cin >> m;

    vector<int> mul = dectobin(m);
    vector<int> Q = dectobin(q);

    // 1. Calculate standard bit width for Q
    int bits = max(mul.size(), Q.size());

    // 2. Pad Q to match the standard bit width
    int q_pad = bits - Q.size();
    Q.insert(Q.begin(), q_pad, 0);

    // 3. Pad M (mul) to be exactly 1 bit larger than Q (Guard Sign Bit)
    int am_bits = bits + 1;
    int m_pad = am_bits - mul.size();
    mul.insert(mul.begin(), m_pad, 0);

    // Create negative M matching the correct size
    vector<int> negm = twoscompliment(mul);

    // Initialize Accumulator A with the guard bit size
    vector<int> a(am_bits, 0);
    int cnt = bits; // Loop runs exactly equal to the number of Q bits

    for (int i = 0; i < cnt; i++)
    {
        // Combined Left Shift [A, Q]
        int q_msb = Q.front();
        Q.erase(Q.begin());
        Q.push_back(0);

        a.erase(a.begin());
        a.push_back(q_msb);

        // Subtraction: A = A - M (using dynamic a.size() to avoid out-of-bounds bugs)
        int carry = 0;
        for (int j = a.size() - 1; j >= 0; j--)
        {
            int sum = a[j] + negm[j] + carry;
            a[j] = sum % 2;
            carry = sum / 2;
        }

        // Check the true sign/guard bit at index 0
        if(a.front() == 1)
        {
            Q.back() = 0; // Subtraction failed

            // Restore: A = A + M
            int rc = 0;
            for (int j = a.size() - 1; j >= 0; j--)
            {
                int sum = a[j] + mul[j] + rc;
                a[j] = sum % 2;
                rc = sum / 2;
            }
        }
        else
        {
            Q.back() = 1; // Subtraction succeeded
        }
    }

    cout << "Quotient (Binary): ";
    for(int val : Q) cout << val;
    cout << endl;

    cout << "Remainder (Binary): ";
    for(int val : a) cout << val;
    cout << endl;
}
