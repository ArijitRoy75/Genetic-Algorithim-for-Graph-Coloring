#pragma once
using namespace std;
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
        void genChrom(); //generate 
        int countDistinct(vector<int>, int);
        void printChrom();
        void printFitness();
        void printparent1();
        void printparent2();
        bool elitism();
        bool fitness();
        void crossover();
        void selection();
        void mutation1();
        void run();
        GAGCP(Graph const &g1)
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
                    //cout<<adjmat[i][j]<<" ";
                }
            }
        }
};
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
    for (int i = 0; i < POP; i++)
    {
        for (int j = 0; j < VERTEX; j++)
        {
            cout<<chrom[i][j]<<"\t";
        }
        cout<<endl;
    }    
}
bool GAGCP::elitism()
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
    if (max>4)
        return 0;
    else
        return 1;
    
}
int GAGCP::countDistinct(vector<int> arr,int n) 
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
bool GAGCP::fitness()
{
    //cout<<"fitness()"<<endl;
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
                if((chrom[k][i]==chrom[k][j]) && (adjmat[i][j]))
                {
                   ++fit;
                }
            }
        }
        fitScore[k][0]=fit;
        fitScore[k][1]=countDistinct(chrom[k], VERTEX);
    }
    bool z = elitism();
}
void GAGCP::printFitness()
{
    cout<<"\nBad Edges Count and Chromatic number:"<<endl;
    for (int i = 0; i < POP; i++)
    {
        cout<<i<<"\t"<<fitScore[i][0]<<"\t"<<fitScore[i][1]<<endl;
    }
}
void GAGCP::printparent1()
{
    cout<<"\nParent 1 Chromosomes Generated:"<<endl;
    int i,j;
    for (int i = 0; i < POP; i++)
    {
        for (int j = 0; j < VERTEX; j++)
        {
            cout<<parent1[i][j]<<"\t";
        }
        cout<<endl;
    }    
}
void GAGCP::printparent2()
{
    cout<<"\nParent2 Chromosomes Generated:"<<endl;
    int i,j;
    for (int i = 0; i < POP; i++)
    {
        for (int j = 0; j < VERTEX; j++)
        {
            cout<<parent2[i][j]<<"\t";
        }
        cout<<endl;
    }    
}
void GAGCP::selection()
{
	srand(time(NULL));
	int i,j,r1,r2,select1,select2;
    //int count=0;
	//cout<<"parentSelection1()"<<endl<<endl;
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
        //cout<<"Select1: "<<select1<<endl;
        //cout<<"Select2: "<<select2<<endl;
        //cout<<endl;
        for(j=0;j<VERTEX;j++)
        {
            parent1[i][j]=chrom[select1][j];
            parent2[i][j]=chrom[select2][j];
        }
        //count++;
    }
}
void GAGCP::crossover()
{
	int i,j,k,swap,pos,chr1,chr2;
	double cp;
    //cout<<endl<<"crossover()"<<endl;
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
    //cout<<endl<<"mutation1"<<endl;
    int valid,adj,all=VERTEX;
    int s,c,d,i,j,k,flag;
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
                if ((chrom[k][i]==chrom[k][j]) && (adjmat[i][j]))
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
    for(int i=0;i<1000;i++)
    {
        fitness();
        //printFitness();
        selection();
        //printparent1();
        //printparent2();
        crossover();
        //printChrom();
        mutation1();
        //printChrom();
    }
    fitness();
    printFitness();
}

