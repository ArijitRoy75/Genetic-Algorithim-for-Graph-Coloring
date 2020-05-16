#include<iostream>
#include<vector>
#include<unordered_set>
#include<ctime>
#include<fstream>
#include<stdlib.h>
#include<iomanip>
using namespace std;
class Graph //Defining a Graph class
{
    public:
        vector<vector <int>> edges;
        vector<vector <bool>> adjmat;
        int nvertex,nedges;
        Graph(string str);
        void printAdjMat();
};
Graph :: Graph(string str)//Graph Constructor
{
    cout<<"Edges:"<<endl;
    ifstream inData(str);
    int x,y;
    char e;
    string s;
    inData>>e>>s>>nvertex>>nedges;
    adjmat.resize(nvertex,vector<bool>(nvertex,0));
    edges.resize(nedges,vector<int>(2));
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
void Graph :: printAdjMat()
{
    cout<<"Adjacent Matrix:"<<endl;
    int c=1;
    for (int i = 0; i < nvertex+1; i++)
    {
        for (int j = 0; j < nvertex+1; j++)
        {
            if(i==0)
			{
				if(i==0 && j==0)
					cout<<" \t";
				else
					cout<<j<<"\t";
			}
			else
			{
				if(j==0)
					cout<<i<<"\t";
				else
                    cout<<adjmat[i-1][j-1]<<"\t";
            }
        }
		cout<<endl;      
    }
}
class GAGCP
{
    public:
        int POP,VERTEX;
        vector<vector <int>> chrom;
        vector<vector <int>> parent1;
        vector<vector <int>> parent2;
        vector<vector <int>> fitScore;
        vector<vector <bool>> adjmat;
        vector<int> validColors;
        vector<int> adjColors;
        vector<int> allColors;
        GAGCP(Graph const &g1);
        void genChrom(); //generate chromosomes
        int countDistinct(vector<int>, int);//counts chromatic number
        void evalFitness();
        void crossover();
        void parentSelection1();
        void mutation1();
        void run();
        void printChrom();
        void printFitness();
        void printParent1();
        void printParent2();
        
};
GAGCP::GAGCP(Graph const &g1)
{
    POP=g1.nvertex;
    VERTEX=g1.nvertex;
    POP=POP+(POP%2);
    adjmat.resize(VERTEX,vector<bool>(VERTEX));
    fitScore.resize(POP,vector<int>(2));
    chrom.resize(POP,vector<int>(VERTEX));
    parent1.resize(POP,vector<int>(VERTEX));
    parent2.resize(POP,vector<int>(VERTEX));
    allColors.resize(VERTEX);
    adjColors.resize(VERTEX);
    validColors.resize(VERTEX);
    for(int i=0;i<VERTEX;i++)
    {
        for (int j = 0; j < VERTEX; j++)
        {
            adjmat[i][j]=g1.adjmat[i][j];
        }
    }
}
void GAGCP::genChrom()
{
    for (int i = 0; i < POP; i++)
    {
        for (int j = 0; j < VERTEX; j++)
        {
            chrom[i][j]=(rand()%VERTEX)+1;
        }
    }
}
void GAGCP::printChrom()
{
    cout<<"\nChromosomes Generated:"<<endl;
    int i,j;
    for (i = 0; i < POP; i++)
    {
        for (j = 0; j < VERTEX; j++)
        {
            cout<<chrom[i][j]<<"\t";
        }
        cout<<endl;
    }    
}
int GAGCP::countDistinct(vector<int> arr,int n) 
{ 
	unordered_set<int> s;  
	int res = 0; 
	for (int i = 0; i < n; i++)
    { 
		if (s.find(arr[i]) == s.end()) { 
			s.insert(arr[i]); 
			res++; 
		} 
	}
	return res; 
}
void GAGCP::evalFitness()
{
    int i,j,k,fit;
    for (k = 0; k < POP; k++)
    {
        fit=0;
        for(i=0;i<(VERTEX-1);i++)
        {
            for (j = i+1; j<VERTEX; j++)
            {
                if((chrom[k][i]==chrom[k][j]) && (adjmat[i][j]))
                {
                   ++fit;
                }
            }
        }
        fitScore[k][0]=fit;
        fitScore[k][1]=countDistinct(chrom[k], VERTEX);
    }
}
void GAGCP::printFitness()
{
    cout<<"\nBad Edges Count and Chromatic number:"<<endl;
    for (int i = 0; i < POP; i++)
    {
        cout<<i<<"\t"<<fitScore[i][0]<<"\t"<<fitScore[i][1]<<endl;
    }
}
void GAGCP::printParent1()
{
    cout<<"\nParent 1 Chromosomes Generated:"<<endl;
    int i,j;
    for (i = 0; i < POP; i++)
    {
        for (j = 0; j < VERTEX; j++)
        {
            cout<<parent1[i][j]<<"\t";
        }
        cout<<endl;
    }    
}
void GAGCP::printParent2()
{
    cout<<"\nParent2 Chromosomes Generated:"<<endl;
    int i,j;
    for (i = 0; i < POP; i++)
    {
        for (j = 0; j < VERTEX; j++)
        {
            cout<<parent2[i][j]<<"\t";
        }
        cout<<endl;
    }    
}
void GAGCP::parentSelection1()
{
	int i,j,r1,r2,select1,select2;
    for(i=0;i<POP;i++)
    {
        r1=rand()%POP;
        r2=rand()%POP;
        while(r1==r2)
            r1=rand()%POP;
        if(fitScore[r1][0]<fitScore[r2][0])
            select1=r1;
        else
            select1=r2;
        r1=rand()%POP;
        r2=rand()%POP;
        while(r1==r2)
            r1=rand()%POP;
        if(fitScore[r1][1]<fitScore[r2][1])
            select2=r1;
        else
            select2=r2;
        for(j=0;j<VERTEX;j++)
        {
            parent1[i][j]=chrom[select1][j];
            parent2[i][j]=chrom[select2][j];
        }
    }
}
void GAGCP::crossover()
{
	int i,k,pos;
	for(i=0;i<POP;i++)
	{
		pos=rand()%VERTEX;
        for (k = 0; k < pos; k++)
        {
            chrom[i][k]=parent1[i][k];
        }
		for(k=pos;k<VERTEX;k++)
		{
			chrom[i][k]=parent2[i][k];
		}
	}
}
void GAGCP::mutation1()
{
    int valid,adj,all=VERTEX;
    int s,i,j,k,flag,x;
    for (x = 0; x < VERTEX; x++)
    {
            allColors[x]=x+1;
    }
    for (x = 0; x < VERTEX; x++)
    {
        adjColors[x]=0;
        validColors[x]=0;
    }
    for (k = 0; k < POP; k++)
    {
        adj=0;
        for(i=0;i<VERTEX-1;i++)
        {
            for (j = i+1; j< VERTEX; j++)
            {
                if ((chrom[k][i]==chrom[k][j]) && (adjmat[i][j]))
                {
                    flag=1;
                    for (x = 0; x < adj; x++)
                    { 
                        if(adjColors[x]==chrom[k][j]){
                            flag=0;
                        }
                    }
                    if(flag==1){
                        adjColors[adj]=chrom[k][j];
                        adj++;
                    }        
                }        
            }
        }
        valid=0;
        for (i = 0; i < VERTEX; i++)
        {
                flag=1;
                for(j=0; j<adj; j++)
                {
                    if(allColors[i]==adjColors[j])
                    {
                       flag=0;
                    }
                }
                if(flag==1)
                {
                    validColors[valid]=allColors[i];
                    valid++;
                }
        }
        if(valid!=0)
        {
            for(i=0;i<VERTEX-1;i++)
            {
                for (j = i+1; j< VERTEX; j++)
                {
                    adj=0;
                    if ((chrom[k][i]==chrom[k][j]) && (adjmat[i][j]))
                    {
                    
                        s=(rand()%valid);
                        chrom[k][j]=validColors[s];
                    }
                }        
            }
        }
        for ( i = 0; i < VERTEX; i++)
        {
            validColors[i]=0;
        }
    }
}
void GAGCP::run()
{
    srand(time(0));
    genChrom();
    //printChrom();
    for(int i=0;i<600;i++)
    {
        evalFitness();
        //printFitness();
        parentSelection1();
        //printParent1();
        //printParent2();
        crossover();
        //printChrom();
        mutation1();
        //printChrom();
    }
    evalFitness();
    printFitness();
}
int main()
{
    clock_t start, end;
    Graph g("myciel4.col");
    g.printAdjMat();
    GAGCP ga(g);
    start=clock();
    ga.run();
    end=clock();
    double time_taken = double(end - start)/double(CLOCKS_PER_SEC);
    cout << "Execution Time : " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl; 
}
