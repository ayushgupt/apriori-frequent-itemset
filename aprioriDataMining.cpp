//
// Created by ayush on 12/8/17.
//

#include <bits/stdc++.h>
using namespace std;
#define DEBUG2(x) do { std::cerr << #x << ": " << x << std::endl; } while (0)


double supportPercentage;
string inputFile;
int supportNum;
vector< vector<int> > transactions;

vector< vector< vector<int> > > candidateSet;
vector< vector< vector<int> > > frequentSet;

void readfile()
{
    ifstream infile;
    infile.open(inputFile.c_str());
    string transactionLine;
    while (std::getline(infile, transactionLine))
    {
        stringstream ss(transactionLine);
        vector<int> currTransactionInts;
        int dummyInt;
        while(ss>>dummyInt)
        {
            currTransactionInts.push_back(dummyInt);
        }
        transactions.push_back(currTransactionInts);
    }
    infile.close();
}

void makeInitialCandidateandFrequentSet()
{
    map<int,int> itemOccMap;
    for(int i=0;i<int(transactions.size());i++)
    {
        for(int j=0;j<int(transactions[i].size());j++)
        {
            map<int, int>::iterator it = itemOccMap.find(transactions[i][j]);
            if (it == itemOccMap.end())
            {
                itemOccMap[transactions[i][j]]=1;
            }
            else
            {
                it->second = ( (it->second) +1);
            }
        }
    }
    vector< vector<int> > candidateFirst;
    vector< vector<int> > frequentFirst;
    for (std::map<int,int>::iterator it=itemOccMap.begin(); it!=itemOccMap.end(); ++it)
    {
        vector<int> tempVector(1,it->first);
        candidateFirst.push_back(tempVector);
        if(it->second>=supportNum)
        {
            frequentFirst.push_back(tempVector);
        }
    }
    candidateSet.push_back(candidateFirst);
    frequentSet.push_back(frequentFirst);
}

void makeCandidateSet()
{
    int frequentSetIndex=int(frequentSet.size())-1;
    set< vector <int> > currCandidateSet;
    for(int i=0;i<frequentSet[frequentSetIndex].size();i++)
    {
        for(int j=(i+1);j<(frequentSet[frequentSetIndex].size());j++)
        {
            int numNotEq=0;
            int diffInSecond;
            for(int k=0;k<(frequentSetIndex+1);k++)
            {
                if(frequentSet[frequentSetIndex][i][k]!=frequentSet[frequentSetIndex][j][k])
                {
                    numNotEq+=1;
                    diffInSecond=frequentSet[frequentSetIndex][j][k];
                }
                if(numNotEq>1)
                {
                    break;
                }
            }
            if(numNotEq==1)
            {
                vector<int> newElementinCandidateSet=frequentSet[frequentSetIndex][i];
                newElementinCandidateSet.push_back(diffInSecond);
                sort(newElementinCandidateSet.begin(),newElementinCandidateSet.end());
                currCandidateSet.insert(newElementinCandidateSet);
            }
        }
    }
    //TODO: Do Pruning of currCandidateSet
    //Set to Vector Conversion
    vector< vector <int> > currCandidateSetVector(currCandidateSet.size());
    copy(currCandidateSet.begin(), currCandidateSet.end(), currCandidateSetVector.begin());
    //Pushing the Candidate Set that I just Found Out
    candidateSet.push_back(currCandidateSetVector);

}


void makeHigherLengthFrequentSets()
{
    for(int tempVar=1;frequentSet[tempVar-1].size()!=0;tempVar++)
    {
        //make candidateSet[tempVar] using frequentSet[tempVar-1]
        makeCandidateSet();
        //Find count of each element of candidateSet[tempVar] by iterating through the transactions
        vector<int> candidateSetCount(int(candidateSet[tempVar].size()),0);
        for(int i=0;i<int(transactions.size());i++)
        {
            set<int> transactionSet(transactions[i].begin(),transactions[i].end());
            for(int j=0;j<int(candidateSet[tempVar].size());j++)
            {
                //check if candidateSet[tempVar][j] is contained inside transactions[i]
                //if yes then candidateSetCount[j]+=1;
                bool elemContained=true;
                for(int k=0;k<candidateSet[tempVar][j].size();k++)
                {
                    if(transactionSet.find(candidateSet[tempVar][j][k])==transactionSet.end())
                    {
                        elemContained=false;
                        break;
                    }
                }
                if(elemContained)
                {
                    candidateSetCount[j]+=1;
                }
            }
        }
        //Make frequentSet[tempVar] by seeing if (count > supportNum)
        vector< vector<int> > currFrequentSet;
        for(int i=0;i<int(candidateSetCount.size());i++)
        {
            if(candidateSetCount[i]>=supportNum)
            {
                currFrequentSet.push_back(candidateSet[tempVar][i]);
            }
        }
        frequentSet.push_back(currFrequentSet);
    }
}

void printFrequentSets()
{
    for(int i=0;i<int(frequentSet.size());i++)
    {
        for(int j=0;j<int(frequentSet[i].size());j++)
        {
            sort(frequentSet[i][j].begin(),frequentSet[i][j].end());
        }
    }
    for(int i=0;i<int(frequentSet.size());i++)
    {
        sort(frequentSet[i].begin(),frequentSet[i].end());
    }
    ofstream outfile;
    outfile.open ("output.txt");
    for(int i=0;i<int(frequentSet.size());i++)
    {
        for(int j=0;j<int(frequentSet[i].size());j++)
        {
            string outLine="";
            for(int k=0;k<int(frequentSet[i][j].size());k++)
            {
                outLine+=to_string(frequentSet[i][j][k])+" ";
            }
            outfile<<outLine<<endl;
        }
    }
    outfile.close();
}

int main()
{
    supportPercentage=1;
    inputFile="retail.dat";
    readfile();
    double supportNumHelp=((supportPercentage*double(transactions.size()))/double(100.0));
    supportNum=int(ceil(supportNumHelp));
    makeInitialCandidateandFrequentSet();
    makeHigherLengthFrequentSets();
    printFrequentSets();
    DEBUG2(transactions.size());
    DEBUG2(transactions[5].size());
    return 0;
}