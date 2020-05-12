#include<iostream>
#include<stdlib.h> 
#include<time.h>
#include<bits/stdc++.h>
#include "Graph.h"
#include "GAGCP.h"
using namespace std;
int main()
{
    clock_t start, end;
    Graph g1("myciel5.col");
    //g1.printAdjMat();
    GAGCP ga(g1);
    start=clock();
    ga.run();
    end=clock(); 
    double time_taken = double(end - start)/double(CLOCKS_PER_SEC); 
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl; 
    cout<<endl;
    //srand(time(0)); 
}
