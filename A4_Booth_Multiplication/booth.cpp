#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// Function to convert a decimal number to binary representation
// Returns a vector of integers (0s and 1s) representing the binary form
vector<int> dectobin(int n)
{
    vector<int> zero(1,0);
    vector<int> binary;
    if (n == 0) return zero; // Handle base case

    // Extract binary digits using modulo and division
    while (n > 0) {
        int remainder = n % 2;          // Get rightmost bit (0 or 1)
        binary.push_back(remainder);     // Add bit to vector
        n = n / 2;                      // Divide by 2 for next iteration
    }

    // Reverse to get most significant bit first
    reverse(binary.begin(),binary.end());
    return binary;
}

// Function to compute two's complement of a binary number
// Two's complement is used to represent negative numbers in binary
vector<int> twoscompliment(vector<int> v)
{
    // Step 1: Invert all bits (one's complement)
    for (int i = 0; i < v.size(); i++) {
        v[i] = (v[i] == 0) ? 1 : 0;
    }

    // Step 2: Add 1 to the result
    int carry = 1;
    int base = 2;

    // Add 1 with carry propagation from right to left
    for (int i = v.size() - 1; i >= 0; i--) {
        int total = v[i] + carry;      // Add bit and carry
        v[i] = total % base;            // Store result bit
        carry = total / base;           // Propagate carry
    }
    return v;
}

// Main function implementing Booth's Multiplication Algorithm
// This algorithm efficiently multiplies two binary numbers
int main()
{
    // Input: Multiplicand (M)
    int m;
    cout<<"Enter the multiplicand: ";
    cin>>m;
    vector<int> mul = dectobin(abs(m));

    // Input: Multiplier (Q)
    int q;
    cout<<"Enter the multiplier: ";
    cin>>q;
    // Convert multiplier to binary and store in vector
    vector<int> Q = dectobin(abs(q));

    // Pad both binary numbers to same length (plus 1 for sign bit)
    int bits = max(mul.size(), Q.size()) + 1;

    // Pad multiplicand with leading zeros
    int m_pad = bits - mul.size();
    mul.insert(mul.begin(), m_pad, 0);
    // If multiplicand is negative, convert to two's complement
    if (m < 0) mul = twoscompliment(mul);

    // Pad multiplier with leading zeros
    int q_pad = bits - Q.size();
    Q.insert(Q.begin(), q_pad, 0);
    // If multiplier is negative, convert to two's complement
    if (q < 0) Q = twoscompliment(Q);

    // Compute negative of multiplicand (two's complement of M) for subtraction
    vector<int> negm = twoscompliment(mul);

    // Initialize variables for Booth's algorithm
    int q0 = 0;                         // Previous bit of Q
    vector<int> a(Q.size(),0);          // Accumulator A (initially 0)
    int cnt = a.size();                 // Number of iterations = length of Q

    // Booth's Algorithm: Process each bit of the multiplier
    for (int i = 0;i<cnt;i++)
    {
        // Get current bit of Q
        int q1 = Q.back();

        // Case 1: Q_current = 1 and Q_previous = 0 => Subtract M from A
        if(q1==1 && q0==0)
        {
            // Perform A = A - M (by adding two's complement of M)
            int carry = 0;
            for (int i = a.size() - 1; i >= 0; i--) {
                int sum = a[i] + negm[i] + carry;  // Add with carry
                a[i] = sum % 2;                    // Store bit
                carry = sum / 2;                   // Propagate carry
            }
        }
        // Case 2: Q_current = 0 and Q_previous = 1 => Add M to A
        else if (q1==0 && q0==1)
        {
            // Perform A = A + M
            int carry = 0;
            for (int i = a.size() - 1; i >= 0; i--) {
                int sum = a[i] + mul[i] + carry;   // Add with carry
                a[i] = sum % 2;                    // Store bit
                carry = sum / 2;                   // Propagate carry
            }
        }

        // Update Q_previous with current Q bit
        q0 = Q.back();
        // Right arithmetic shift: shift A and Q right by 1 bit
        Q.pop_back();                   // Remove last bit of Q
        Q.insert(Q.begin(), a.back()); // Shift MSB of A into LSB of Q

        // Arithmetic right shift of A (copy MSB to maintain sign)
        int a0 = a.front();             // Save MSB of A (sign bit)
        a.pop_back();                   // Remove LSB of A
        a.insert(a.begin(), a0);        // Insert MSB back at front (sign extension)
    }

    // Combine A and Q to get final product (A || Q)
    vector<int> product = a;
    product.insert(product.end(), Q.begin(), Q.end());

    // Display the final product in binary format
    cout << "Product: ";
    for (int val : product) {
        cout << val;
    }
    cout << endl;

    return 0;
}
