#include<iostream>
#include<vector>
#include<unordered_set>
#include<ctime>
#include<fstream>
#include<stdlib.h>
#include<iomanip>
#include<numeric>
using namespace std;
class Graph //Defining a Graph class
{
    public:
        string filename;
        vector<vector <int>> edges;
        vector<vector <bool>> adjmat;
        int nvertex,nedges,max_degree;
        Graph(string str);
        void printAdjMat();
};
Graph :: Graph(string str)//Graph Constructor
{
    filename=str;
    ifstream inData(str);
    if(inData.fail())
    {
        cout<<"\nFilename doesn't exist!! Make Sure You have enterd correct filename....\n";
        exit(1);
    }
    int x,y;
    char e;
    string s;
    cout<<"Edges:"<<endl;
    inData >> s;
    while(s != "edge")
            inData >> s;
    inData>>nvertex>>nedges;
    adjmat.resize(nvertex,vector<bool>(nvertex,0));
    edges.resize(nedges,vector<int>(2));
    cout<<nvertex<<" "<<nedges<<endl;
    for (int i=0; i<nedges; i++)
    {
    	inData >> e >> x >> y;
        edges[i][0]=x;
        edges[i][1]=y;
        //cout << edges[i][0] <<"\t"<< edges[i][1] << endl;
        adjmat[x-1][y-1]=1;
        adjmat[y-1][x-1]=1;
    }
    int sum,max=0;
    for (int i = 0; i < nvertex; i++)
    {
        sum=accumulate(adjmat[i].begin(),adjmat[i].end(),0);
        if(max<sum)
            max=sum;
    }
    max_degree=max;
    cout<<"\nMax Degree: "<<max_degree<<endl;
    inData.close();
    if(max_degree+1==nvertex)
    {
        cout<<"\nChromatic Number:"<<nvertex;
        exit(1);
    }
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
    //Based on Hindi, Musa & Yampolskiy, Roman. (2012). Genetic Algorithm Applied to the Graph Coloring Problem. Midwest Artificial Intelligence and Cognitive Science Conference. 60. 
    public:
        string filename;
        int POP,VERTEX,max_deg1;
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
        bool check();
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
    filename=g1.filename;
    POP=g1.nvertex;
    max_deg1=g1.max_degree+1;
    VERTEX=g1.nvertex;
    POP=POP+(POP%2);
    adjmat.resize(VERTEX,vector<bool>(VERTEX));
    fitScore.resize(POP,vector<int>(2));
    chrom.resize(POP,vector<int>(VERTEX));
    parent1.resize(POP,vector<int>(VERTEX));
    parent2.resize(POP,vector<int>(VERTEX));
    allColors.resize(max_deg1);
    adjColors.resize(max_deg1);
    validColors.resize(max_deg1);
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
            chrom[i][j]=(rand()%max_deg1)+1;
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
		if (s.find(arr[i]) == s.end())
        { 
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
        if(fitScore[r1][1]<fitScore[r2][1])
            select1=r1;
        else
            select1=r2;
        r1=rand()%POP;
        r2=rand()%POP;
        while(r1==r2)
            r1=rand()%POP;
        if(fitScore[r1][0]<fitScore[r2][0])
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
    int valid,adj;
    int s,i,j,k,flag,x;
    for (x = 0; x < max_deg1; x++)
    {
            allColors[x]=x+1;
    }
    for (x = 0; x < max_deg1; x++)
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
        for (i = 0; i < max_deg1; i++)
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
                    if ((chrom[k][i]==chrom[k][j]) && (adjmat[i][j]))
                    {
                        s=(rand()%valid);
                        chrom[k][j]=validColors[s];
                    }
                }        
            }
        }
        for (i = 0; i < max_deg1; i++)
        {
            validColors[i]=0;
        }
    }
}
bool GAGCP::check()
{
    int flag=0;
    for (int k = 0; k < POP; k++)
    {
        for(int i=0;i<(VERTEX-1);i++)
        {
            for (int j = i+1; j<VERTEX; j++)
            {
                if((chrom[k][i]==chrom[k][j]) && (adjmat[i][j]))
                {
                   return 1;
                }
            }
        }
    }
    int elem=fitScore[0][1];
    for (int i = 1; i < POP; i++)
    {
        if(elem!=fitScore[i][1])
            return 1;
    }
    return 0;
}
void GAGCP::run()
{
    clock_t start, end;
    cout<<"\n\nGAGCP:";
    cout<<"\nFile name: "<<filename<<endl;
    start=clock();
    srand(time(0));
    bool result;
    int i=0;
    do
    {
        ++i;
        if(i==1)
        {
            genChrom();
            evalFitness();
            result=check();
            continue;   
        }
        parentSelection1();
        crossover();
        mutation1();
        evalFitness();
        result=check();
    }while (result!=0);
    end=clock();
    cout<<"\nChromatic Number: "<<fitScore[0][1]<<endl;
    for (int j = 0; j < VERTEX; j++)
    {
        cout<<chrom[0][j]<<"\t";
    }
    double time_taken = double(end - start)/double(CLOCKS_PER_SEC);
    cout << "\n\nNo. Of Generations: " <<i<<endl;
    cout << "Execution Time : " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl;
}
class ACOGCP
{
    //Based on Pal, Anindya & Ray, Biman & Zakaria, Nordin & Naono, Ken & Sarma, Samar. (2012). Generating Chromatic Number of a Graph using Ant Colony Optimization and Comparing the Performance with Simulated Annealing. Procedia Engineering. 50. 629–639. 10.1016/j.proeng.2012.10.069. 
    public:
        string filename;
        vector<vector <bool>> adjmat;
        vector<int> color;
        int max_deg1,vertex,ncolor;
        bool result;
        ACOGCP(Graph const &g1);
        bool check();
        int countDistinct();
        void compute();
        void run();
        void printColor();
};
ACOGCP::ACOGCP(Graph const &g1)
{
    filename=g1.filename;
    max_deg1=g1.max_degree+1;
    vertex=g1.nvertex;
    adjmat.resize(vertex,vector<bool>(vertex));
    color.resize(vertex,1);
    for(int i=0; i<vertex; i++)
    {
        for (int j = 0; j < vertex; j++)
        {
            adjmat[i][j]=g1.adjmat[i][j];
        }
    }
}
void ACOGCP::compute()
{
    int value, j, k;
    bool result;
    for (j = 1; j <vertex; j++)
    {
        ncolor=countDistinct();
        result=check();
        if (result==1)
        {
            return;
        }
        for (k = 0; k < vertex; k++)
        {
            if (j!=k)
            {
                value=adjmat[j][k]*color[k];
                if (value==color[j])
                {
                    if (color[k]<=max_deg1)
                        color[k]=color[k]+1;
                    else
                        color[k]=color[k]%(+max_deg1)+1;
                    j=0;
                    k=0;
                }
            }
        }
    }
}
int ACOGCP::countDistinct()
{
    unordered_set<int> s;  
	int res = 0; 
	for (int i = 0; i < vertex; i++)
    { 
		if (s.find(color[i]) == s.end())
        { 
			s.insert(color[i]); 
			res++;
		} 
	}
	return res;
}
bool ACOGCP::check()
{
    for(int i=0;i<(vertex-1);i++)
    {
        for (int j = i+1; j<vertex; j++)
        {
            if((color[i]==color[j]) && (adjmat[i][j]))
            {
               return 0;
            }
        }
    }
    return 1;
}
void ACOGCP::printColor()
{
    for (int j = 0; j < vertex; j++)
    {
        cout<<color[j]<<"\t";
    }
}
void ACOGCP::run()
{
    clock_t start, end;
    cout<<"\n\nACOGCP:";
    cout<<"\nFile name:"<<filename<<endl;
    start=clock();
    compute();
    cout<<"\nChromatic Number: "<<countDistinct()<<endl;
    result=check();
    printColor();
    end=clock();
    double time_taken = double(end - start)/double(CLOCKS_PER_SEC);
    cout << "\nExecution Time : " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl;
}
int main(int argc,char* argv[])
{
    string file=argv[1];
    Graph g(file);
    //g.printAdjMat();
    ACOGCP aco(g);
    aco.run();
    GAGCP ga(g);
    ga.run();
}
