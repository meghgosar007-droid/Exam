#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

void printFrame(vector<int> a, int p)
{
    for(auto i : a)
    {
        cout << i << " ";
    }
    cout << endl;
    cout << "Pointer = " << a[p] << endl;
}

void LRU(vector<int> arr)
{
    const int capacity = 3;              // number of frames
    vector<int> frame;                   // current pages in memory
    vector<int> lastUsed;                // last used time index for each frame
    int pageHit = 0;

    for (int t = 0; t < (int)arr.size(); t++)
    {
        int page = arr[t];
        auto it = find(frame.begin(), frame.end(), page);

        if ((int)frame.size() < capacity)
        {
            // Not full yet: just add if not present
            if (it == frame.end())
            {
                frame.push_back(page);
                lastUsed.push_back(t);
            }
            else
            {
                // Page hit while still filling
                int idx = distance(frame.begin(), it);
                lastUsed[idx] = t;
                pageHit++;
                cout << "Page Hit" << endl;
            }
            // Print current frame; choose "pointer" as most recently used index
            int p = distance(lastUsed.begin(), max_element(lastUsed.begin(), lastUsed.end()));
            printFrame(frame, p);
        }
        else
        {
            if (it != frame.end())
            {
                // Page hit
                int idx = distance(frame.begin(), it);
                lastUsed[idx] = t;
                pageHit++;
                cout << "Page Hit" << endl;

                int p = distance(lastUsed.begin(), max_element(lastUsed.begin(), lastUsed.end()));
                printFrame(frame, p);
            }
            else
            {
                // Page fault: replace least recently used
                int lruIdx = distance(lastUsed.begin(), min_element(lastUsed.begin(), lastUsed.end()));
                frame[lruIdx] = page;
                lastUsed[lruIdx] = t;

                int p = distance(lastUsed.begin(), max_element(lastUsed.begin(), lastUsed.end()));
                printFrame(frame, p);
            }
        }
    }

    cout << "Total Page Hits: " << pageHit << endl;
    cout << "Total Page Faults: " << ((int)arr.size() - pageHit) << endl;
    double t = static_cast<double>(pageHit) / arr.size();
    cout << fixed << setprecision(2);
    cout << "Page Hit Rate: " << t * 100.00 << endl;
}

void FIFO(vector<int> arr)
{
    vector<int> pageF;
    int p = 0;
    int c = 0;
    int pageHit = 0;
    for(int i = 0; i < (int)arr.size(); i++)
    {
        if(c < 3)
        {
            pageF.push_back(arr[i]);
            printFrame(pageF, p);
            c++;
        }
        else
        {
            if(find(pageF.begin(), pageF.end(), arr[i]) != pageF.end())
            {
                cout << "Page Hit" << endl;
                pageHit++;
                printFrame(pageF, p);
                continue;
            }
            else
            {
                pageF[p] = arr[i];
                printFrame(pageF, p);
                p = (p + 1) % 3;
            }
        }
    }
    cout << "Total Page Hits: " << pageHit << endl;
    cout << "Total Page Faults: " << ((int)arr.size() - pageHit) << endl;
    double t = static_cast<double>(pageHit) / arr.size();
    cout << fixed << setprecision(2);
    cout << "Page Hit Rate: " << t * 100.00 << endl;
}

int main()
{
    vector<int> frames(20);
    cout << "enter the frame order : ";
    for(int i = 0; i < 20; i++)
    {
        cin >> frames[i];
    }

    cout << "\n--- FIFO ---\n";
    FIFO(frames);

    cout << "\n--- LRU ---\n";
    LRU(frames);

    return 0;
}
