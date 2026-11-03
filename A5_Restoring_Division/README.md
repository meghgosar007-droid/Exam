# Assignment 5 - Restoring division
`g++ restoring_division.cpp -o div && ./div` - unsigned dividend / divisor, prints quotient and remainder in binary.
Flow: shift [A,Q] left -> A = A - M -> if A<0 restore & Q0=0 else Q0=1 -> repeat N times.
