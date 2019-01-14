#include <iostream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>
#include <gzstream.h>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

class RandIndex
{
public:
    unordered_set<int> hsRandIndex;
    int iRand;
    RandIndex(int inum)
    {
        this->iRand = inum;
        this->Rewind();
    }
    void Rewind()
    {
        this->hsRandIndex.clear();
        srand(time(0));
        while(this->hsRandIndex.size() < this->iRand)
        {
            int iT = rand() % 100;
            if(this->hsRandIndex.find(iT) == this->hsRandIndex.end())
                hsRandIndex.insert(iT);
        }
    }
    bool IsSelect(int i)
    {
        return this->hsRandIndex.find(i) != this->hsRandIndex.end();
    }
};

int main(int argc, char** argv)
{
    string sFInput1, sFInput2, sOutput;
    int iFlop;
    for(int i = 1; i < argc; i++)
    {
        if((string)argv[i]=="-f1") sFInput1=(string)argv[++i];
        if((string)argv[i]=="-f2") sFInput2=(string)argv[++i];
        if((string)argv[i]=="-o") sOutput=(string)argv[++i];
        if((string)argv[i]=="-s") iFlop=atoi(((string)argv[++i]).c_str());
    }
    cout<<"Random select from "<<iFlop<<"/100 from "<<sFInput1<<","<<sFInput2<<" to "<<sOutput<<"\t"<<"\n";
    
    igzstream* igzFastq1 = new igzstream(sFInput1.c_str());
    igzstream* igzFastq2 = new igzstream(sFInput2.c_str());
    ogzstream* ogzFastq1 = new ogzstream((sOutput+"_1.fastq.gz").c_str());
    ogzstream* ogzFastq2 = new ogzstream((sOutput+"_2.fastq.gz").c_str());
    RandIndex* pSeed = new RandIndex(iFlop);
    
    string sL1, sL2;
    string sR1, sR2;
    int iRandomIndex=0;
    long long int iReadIndex =0;
    while(getline(*igzFastq1, sL1))
    {
        getline(*igzFastq2, sL2);
        sR1=sL1+"\n";
        sR2=sL2+"\n";
        if(iReadIndex % 1000000 == 0)
            cout<<"="<<flush;
        iReadIndex++;
        for(int i = 0;i < 3; i++)
        {
            getline(*igzFastq1, sL1);
            getline(*igzFastq2, sL2);
            sR1+=sL1+"\n";
            sR2+=sL2+"\n";
        }
        if(pSeed->IsSelect(iRandomIndex))
        {
            *ogzFastq1<<sR1;
            *ogzFastq2<<sR2;
        }
        iRandomIndex++;
        if(iRandomIndex == 100) iRandomIndex = 0;
    }
    cout<<"\ndone\n";
    igzFastq1->close();
    igzFastq2->close();
    ogzFastq1->close();
    ogzFastq2->close();
}