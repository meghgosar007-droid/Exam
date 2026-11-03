# Assignment 2 - COPY / PASTE (block transfer) in 8086 using DEBUG

Aim: simulate copy/paste of a memory block, handling all overlap cases
(source == destination, source > destination, source < destination -> copy backwards).

Algorithm (generalised):
1. Initialise counter, source pointer, destination pointer.
2. If SI == DI -> stop. If SI > DI -> copy forward. Else copy backward
   (point both pointers at the last element: ptr + count - 1, then decrement).
3. Move one element at a time until counter is 0.

The code was entered in DEBUG (`images/code_listing.png`) and the result checked
with `D` (`images/output.png`).
