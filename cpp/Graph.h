#pragma once
using namespace std;
class Graph //Defining a Graph class
{
    
    public:
        vector<vector <int>> edges;
        vector<vector <bool>> adjmat;
        int nvertex,nedges;
        Graph(string str);
        void printAdjMat();
        //bool isAdj(int,int);
};
Graph :: Graph(string str)
{
    cout<<"Edges:"<<endl;
    ifstream inData(str);

    int x,y;
    char e;
    string s;
    inData>>e>>s>>nvertex>>nedges;
    adjmat.resize(nvertex,vector<bool>(nvertex));
    edges.resize(nedges,vector<int>(2));
    for (int a = 0; a < nvertex; a++)
    {
        for (int b = 0; b < nvertex; b++)
        {
            adjmat[a][b]=0;
        }      
    }
    cout<<nvertex<<" "<<nedges<<endl;
    for (int i=0; i<nedges; i++)
    {
    	inData >> e >> x >> y;
        edges[i][0]=x;
        edges[i][1]=y;
        cout << edges[i][0] <<"\t"<< edges[i][1] << endl;
        adjmat[x-1][y-1]=1;
        adjmat[y-1][x-1]=1;
    }
    inData.close();
}