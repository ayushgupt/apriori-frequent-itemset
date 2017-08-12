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

int main()
{
    supportPercentage=20;
    inputFile="retail.dat";
    readfile();
    double supportNumHelp=((supportPercentage*double(transactions.size()))/double(100.0));
    supportNum=int(ceil(supportNumHelp));





    DEBUG2(transactions.size());
    DEBUG2(transactions[5].size());
    return 0;
}