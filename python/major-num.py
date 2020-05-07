from random import random
from random import randint
from collections import Counter
import numpy as np
import time
class Graph:
    nedges,nvertex=0,0
    edges=np.empty([2, 2], dtype=int)
    adjmat=np.empty([2, 2], dtype=int)
    def __init__(self,filename):
        f=open(filename,"r")
        j=0
        adjmat=[]
        edges=[]
        for x in f:
            if(j==0):
                i=[int(y) for y in x.split() if y!="p" and y!="edge"]
                self.nedges=i[1]
                self.nvertex=i[0]
                adjmat=[[0 for col in range(self.nvertex)] for row in range(self.nvertex)]
            else:
                i=[int(y) for y in x.split() if y!="e"]
                edges.append(i)
                adjmat[i[0]-1][i[1]-1]=1
                adjmat[i[1]-1][i[0]-1]=1
            j+=1
        self.edges=np.array(edges)
        self.adjmat=np.array(adjmat)
    def printAdj(self):
        for i in self.adjmat:
            print(i)
class GA:
    chrom=[]
    fitScore=[]
    mutation_prob,crossover_prob=0.05,0.7
    pop,chrom_length=0,0
    adjmatrix=np.empty([2, 2], dtype=int)
    is_run=False
    solution=dict()
    def __init__(self,g1,cross_prob=0.7,mutate_prob=0.05):
        self.mutation_prob,self.crossover_prob=mutate_prob,cross_prob
        self.chrom_length=g1.nvertex
        self.adjmatrix=g1.adjmat
        self.pop=g1.nvertex
        self.pop=self.pop+(self.pop%2)
        self.chrom=[[randint(1, self.chrom_length) for col in range(self.chrom_length)] for row in range(self.pop)]
        self.fitScore=[[0 for col in range(self.pop)] for row in range(2)]

    def printChrom(self):
        print("\nChromosomes Gererated")
        for i in self.chrom:
            print(i)
    
    def fitness(self):
        for k in range(self.pop):
            fit=0
            for i in range(0,self.chrom_length-1):
                for j in range(i+1,self.chrom_length):
                    if (self.chrom[k][i]==self.chrom[k][j]) and self.adjmatrix[i][j]:
                        fit=fit+1
            self.fitScore[0][k]=fit
            self.fitScore[1][k]=len(Counter(self.chrom[k]).keys())

    def selection(self):
        select1,select2=0,0
        tempchrom=[]
        for i in range(0,self.pop,2):
            r1,r2=randint(0,self.pop-1),randint(0,self.pop-1)
            while r1==r2:
                r1=randint(0,self.pop-1)
            if self.fitScore[0][r1]<self.fitScore[0][r2]:
                select1=r1
            else:
                select1=r2
            r1,r2=randint(0,self.pop-1),randint(0,self.pop-1)
            while r1==r2:
                r1=randint(0,self.pop-1)
            if self.fitScore[1][r1]<self.fitScore[1][r2]:
                select2=r1
            else:
                select2=r2
            tempchrom.append(self.chrom[select1])
            tempchrom.append(self.chrom[select2])
        self.chrom=tempchrom

    def crossover(self):
        for i in range(0,self.pop,2):
            cp=random()
            chr1,chr2=0,0
            if cp<self.crossover_prob:
                chr1,chr2=randint(0,self.pop-1),randint(0,self.pop-1)
                while chr1==chr2:
                    chr1=randint(0,self.pop-1)
                pos1=randint(0,self.chrom_length-1)
                swap=self.chrom[chr1][pos1:self.chrom_length]
                self.chrom[chr1][pos1:self.chrom_length]=self.chrom[chr2][pos1:self.chrom_length]
                self.chrom[chr2][pos1:self.chrom_length]=swap
    
    def mutation1(self):
        for k in range(self.pop):
            mp=random()
            if mp<self.mutation_prob:
                allcolors=set([i for i in range(1,self.chrom_length+1)])
                adjcolors=set()
                validcolors=[]
                for i in range(self.chrom_length-1):
                    for j in range(i+1,self.chrom_length):
                        if self.chrom[k][i]==self.chrom[k][j] and self.adjmatrix[i][j]:
                            val=self.chrom[k][j]
                            adjcolors.add(val)
                validcolors=list(allcolors-adjcolors)
                for i in range(self.chrom_length-1):
                    for j in range(i,self.chrom_length):
                        if self.chrom[k][i]==self.chrom[k][j] and self.adjmatrix[i][j]:
                            s=randint(0,len(validcolors)-1)
                            self.chrom[k][j]=s
    
    def mutation2(self):
        for k in range(self.pop):
            mp=random()
            if mp<self.mutation_prob:
                for i in range(self.chrom_length-1):
                    for j in range(i+1,self.chrom_length):
                        if self.chrom[k][i]==self.chrom[k][j] and self.adjmatrix[i][j]:
                            self.chrom[k][j]=randint(1,self.chrom_length)

    def run(self):
        print("\nRunning Genetic Algorithm....") 
        a=time.time()
        self.fitness()
        i=0
        result0=False
        while result0!=True:
            i=i+1
            self.selection()
            self.crossover()
            self.mutation1()
            self.fitness()
            if self.fitScore[0][0]==0:
                result0 = all(elem == self.fitScore[0][0] for elem in self.fitScore[0])
        min_sol=self.fitScore[0].index(min(self.fitScore[0]))
        b=time.time()
        self.solution={'solution':self.chrom[0],'generations':i,'bad_edges':self.fitScore[0][0],'chromatic_number':self.fitScore[1][0],'exec_time':b-a, 'crossover_prob':self.crossover_prob,'mutation_prob':self.mutation_prob}
        print("\nSolution Generated Successfully in",b-a,"seconds\n")
        self.is_run=True

    def get_solution(self):
        if self.is_run==True:
                return self.solution
        else:
            print("Failed to generate solution without calling GA.run()")
            return -1


if __name__=="__main__": 
    g=Graph("myciel3.col")
    #print(g.edges)
    #g.printAdj()
    genalg=GA(g,cross_prob=0.7,mutate_prob=0.01)
    genalg.run()
    print(genalg.get_solution())