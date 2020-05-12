#include<iostream>//including required header files
#include<conio.h>
#include<bits/stdc++.h> 
#include<cstdlib>
#include<ctime>
#include<math.h>
#include<fstream>
#define EDGES 20
#define VERTEX 11
#define POP 12
#define DATASET "myciel3.txt"
int chrom[POP][VERTEX];
int parent1[POP][VERTEX];
int parent2[POP][VERTEX];
//int tempChrom[POP][VERTEX];
int fitScore[POP][2];
int elcount=0;
using namespace std;
class Graph //Defining a Graph class
{
    public:
        int edges[EDGES][2];
        bool adjmat[VERTEX][VERTEX];
        int a,b;
        Graph(string str);
        void printAdjMat();
        bool isAdj(int,int);
        //void printEdge();
};
Graph :: Graph(string str)
{
    //int *adjmat=new int[][];
    for (int a = 0; a < VERTEX; a++)
    {
        for (int b = 0; b < VERTEX; b++)
        {
            adjmat[a][b]=0;
        }      
    }
    cout<<"Edges:"<<endl;
    ifstream inData(str);
    int x,y;
    char e;
    for (int i=0; i<EDGES; i++) {
    	inData >> e >> edges[i][0] >> edges[i][1]; //Read x and y coordinates at position i
        cout << edges[i][0] <<"\t"<< edges[i][1] << endl;
        x=edges[i][0];
        y=edges[i][1]; //Output the x and y coordinates read above
        adjmat[x-1][y-1]=1;
        adjmat[y-1][x-1]=1;
    }
    cout<<endl;
    inData.close();
}
void Graph :: printAdjMat(){
    cout<<"Adjacent Matrix:"<<endl;
    int c=1;
    for (int i = 0; i < VERTEX+1; i++)
    {
        for (int j = 0; j < VERTEX+1; j++)
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
bool Graph::isAdj(int a, int b)
{
    if(adjmat[a][b])
        return 1;
    else
        return 0;
    
}
void genChrom() //generate chromosomes
{
    for (int i = 0; i < POP; i++)
    {
        for (int j = 0; j < VERTEX; j++)
        {
            chrom[i][j]=(rand()%VERTEX)+1;
        }
    }    
}
int countDistinct(int arr[], int n) 
{ 
	// Creates an empty hashset 
	unordered_set<int> s; 
	// Traverse the input array 
	int res = 0; 
	for (int i = 0; i < n; i++) { 

		// If not present, then put it in 
		// hashtable and increment result 
		if (s.find(arr[i]) == s.end()) { 
			s.insert(arr[i]); 
			res++; 
		} 
	} 

	return res; 
}
void printChrom()
{
    cout<<"Chromosomes Generated:"<<endl;
    int i,j;
    for (int i = 0; i < POP; i++)
    {
        for (int j = 0; j < VERTEX; j++)
        {
            cout<<chrom[i][j]<<"  ";
        }
        cout<<endl;
    }    
}
bool elitism()
{
	int i,ind;
	int max=0;
    //fflush(stdin);
	for(i=0;i<POP;i++)
	{
		if(max<fitScore[i][0])
		{
			max=fitScore[i][0];
			ind=i;
		}
	}
	//for(i=0;i<VERTEX;i++)
	//	elite[elcount][i]=chrom[ind][i];
	//elite[elcount][]=max;
	//elcount++;
    if (max>4)
        return 0;
    else
        return 1;
    
}
bool fitness(Graph g)
{
    cout<<"fitness()"<<endl;
    int i,j,k,fit;
    //printChrom();
    for (k = 0; k < POP; k++)
    {
        fit=0;
        for(i=0;i<(VERTEX-1);i++)
        {
            for (j = i+1; j<VERTEX; j++)
            {
               // cout<<(chrom[k][i]==chrom[k][j]);
                if((chrom[k][i]==chrom[k][j]) && (g.isAdj(i,j)))
                {
                   ++fit;
                }
            }
        }
        fitScore[k][0]=fit;
        int n = sizeof(chrom[k]) / sizeof(chrom[0][0]);
        fitScore[k][1]=countDistinct(chrom[k], n);
    }
    bool z = elitism();
}
void printFitness(){
    cout<<"Bad Edges Count and Chromatic number:"<<endl;
    for (int i = 0; i < POP; i++)
    {
        cout<<i<<"\t"<<fitScore[i][0]<<"\t"<<fitScore[i][1]<<endl;
    }
}
void crossover()
{
	int i,j,k,swap,pos,chr1,chr2;
	double cp;
    cout<<endl<<"crossover()"<<endl;
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
void selection()
{
	srand(time(NULL));
	int i,j,r1,r2,select1,select2;
    int count=0;
	cout<<"parentSelection1()"<<endl<<endl;
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
            parent1[count][j]=chrom[select1][j];
            parent2[count][j]=chrom[select2][j];
        }
        count++;
    }
}
void mutation(Graph g, bool m)
{
    int i,j,k,flag;
    if(m==1)
    {
        cout<<endl<<"mutation1"<<endl;
        int valid,adj,all=VERTEX;
        int s,c,d;
        int validColors[VERTEX],adjColors[VERTEX],allColors[VERTEX];
        for (int x = 0; x < VERTEX; x++)
        {
            allColors[x]=x+1;
        }
        for (int x = 0; x < VERTEX; x++)
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
                    if ((chrom[k][i]==chrom[k][j]) && (g.isAdj(i,j)))
                    {
                        //cout<<chrom[k][j]<<endl;
                        flag=1;
                        for (int x = 0; x < adj; x++)
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
            for(i=0;i<VERTEX;i++)
            {
                for (j = i+1; j< VERTEX; j++)
                {
                    adj=0;
                    if ((chrom[k][i]==chrom[k][j]) && (g.isAdj(i,j)))
                    {
                        s=(rand()%valid);
                        chrom[k][j]=validColors[s];
                    }        
                }
            }
            for ( i = 0; i < VERTEX; i++)
            {
                validColors[i]=0;
            }
        }
    }
    else
    {
        cout<<endl<<"mutation2"<<endl;
        for (k = 0; k < POP; k++)
        {
            for(i=0;i<VERTEX-1;i++)
            {
                for (j = i+1; j< VERTEX; j++)
                {
                    if ((chrom[k][i]==chrom[k][j]) && (g.isAdj(i,j)))
                    {
                        chrom[k][j]=(rand()%VERTEX)+1;
                    }
                }
            }
        }
    }
}
int main()
{
    bool z;
    Graph g1(DATASET);
    g1.printAdjMat();
    cout<<endl<<endl;
    srand(time(0));  //123
    genChrom();
    printChrom();
    z=fitness(g1);
    //printChromatic();
    for (int j = 0; j < 1000; j++)
    {
        cout<<"Generation: "<<j<<endl<<endl<<endl;
        selection();
        crossover();
        mutation(g1,1);
        z=fitness(g1);
        //printChrom();
        //printFitness();
    }
    //printChrom();
    //printChromatic();
    printFitness();
    //getch(); 
}
