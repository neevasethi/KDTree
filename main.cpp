#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>
#include "KDtree.h"
using namespace std::chrono;
using namespace std;


void GetDataFromFile(string filePath, vector<vector<int>>& dataSet, int& totalDim)
{
    string lineFromFile;
    ifstream myFile(filePath);
    int counter = 0;
    if (myFile.is_open())
    {
        //go to each tuple
        while (getline(myFile, lineFromFile))
        {
            //counter++;
            //if (counter >= 10000)
            //{
            //    break; 
            //}
            vector<int> tuple;
            istringstream stream(lineFromFile);
            string tmp;
            totalDim = 0;
            //go to each number in tuple
            while (getline(stream, tmp, ','))
            {
                totalDim++;
                tuple.push_back(stoi(tmp));
            }

            dataSet.push_back(tuple);
        }
        myFile.close();
        //Note: totalDim is recalculated for every tuple, but it is remembered after the last tuple
    }
}

void GetRangeQueryFromFile(string filePath, vector<vector<pair<int, int>>>& query)
{
    ifstream myFile(filePath);
    if (myFile.is_open())
    {
        string lineFromFile;
        vector<int> tmp;
        while (getline(myFile, lineFromFile))
        {
            istringstream stream(lineFromFile);
            string inputNumber;
            while (getline(stream, inputNumber, ' '))
            {
                tmp.push_back(stoi(inputNumber));
            }
            //check if even number of values in tmp
            if (tmp.size() % 2 != 0)
            {
                cout << "error with reading in query";
                return;
            }

            vector<pair<int, int>>toPush;
            for (int i = 0; i < tmp.size(); i += 2)
            {
                int min = tmp[i];
                int max = tmp[i + 1];
                pair<int, int> constraint(min, max);
                toPush.push_back(constraint);
            }

            query.push_back(toPush);
            tmp.clear();
        }
    }

}

void MakeKeyPointerPairs(vector<vector<int>>& dataSet, vector<pair<vector<int>, vector<int>*>>& keyPointerPairs)
{
    for (int i = 0; i < dataSet.size(); i++)
    {
        vector<int> tmp = dataSet[i];
        vector<int>* tmpPtr = &dataSet[i];
        pair<vector<int>, vector<int>*> myPair(tmp, tmpPtr);
        keyPointerPairs.push_back(myPair);
    }
}
void SequentialSearch(vector<vector<int>>& dataSet, vector<vector<pair<int, int>>>& queries)
{
    for (int i = 0; i < queries.size(); i++)
    {
        vector<pair<int, int>>query = queries.at(i);
        //print out the query range here
        cout << "Query: ";
        for (auto x : query)
        {
            cout << x.first << " ";
            cout << x.second << " ";
        }
        cout << endl;
        vector<vector<int>> returnData;
        for (int i = 0; i < dataSet.size(); i++)
        {
            vector<int> tuple = dataSet[i];

            //if dimensions of tuple doesn't match constraints
            if (tuple.size() != query.size())
            {
                cout << "Major error";
                break;
            }

            //iterate through each tuple and check if all values match the constaints
            int numFails = 0;
            for (int j = 0; j < tuple.size(); j++)
            {
                int val = tuple[j];
                int dimMin = query[j].first;
                int dimMax = query[j].second;
                if (val < dimMin || val > dimMax)
                {
                    numFails++;
                }
            }

            //if tuple passes all checks
            if (numFails == 0)
            {
                returnData.push_back(tuple);
            }
        }
        sort(returnData.begin(), returnData.end());

        //print data
        for (int i = 0; i < returnData.size(); i++)
        {
            vector<int> tuple = returnData[i];
            int j = 0;
            for (j; j < tuple.size() - 1; j++)
            {
                cout << tuple[j] << ",";
            }
            cout << tuple[j] << endl;
        }
        cout << "number of queries: " << returnData.size() << endl << endl;
    }
}


int main(int argc, char* argv[]) {
    /*
    //TESTING, don't delete until all tests on serve pass

    //load data
    vector<vector<int>> dataSet; // data
    vector<pair<vector<int>, vector<int>*>> keyPointerPairs;
    vector<vector<pair<int, int>>> queries; // range query
    string dataFilepath = "data.txt";
    string queryFilepath = "query1.txt";
    int totalDim;
    int startDim = 0;
    int blockSize = 50;

    cout << "Reading in data..." << endl;
    GetDataFromFile(dataFilepath, dataSet, totalDim);
    MakeKeyPointerPairs(dataSet, keyPointerPairs);

    GetRangeQueryFromFile(queryFilepath, queries);

    cout << "Sequential Search:" << endl;
    SequentialSearch(dataSet, queries);
    cout << endl;

    /////////////////////////////////////////////////////////////////////////
    
    cout << "KD tree search:" << endl;
    KDtree kdTree = KDtree();
    cout << "Creating tree..." << endl;
    kdTree.myRoot = kdTree.CreateTree(keyPointerPairs, startDim, blockSize, totalDim);

    cout << "Range Queries:" << endl;
    kdTree.RangeQuery(queries);
    cout << endl;
    /////////////////////////////////////

    
    cout << "MYkd tree search:" << endl;
    KDtree myKDTree = KDtree();
    cout << "Creating tree..." << endl;
    myKDTree.myRoot = myKDTree.CreateMyTree(keyPointerPairs, startDim, blockSize, totalDim);

    cout << "Range Queries:" << endl;
    myKDTree.RangeQuery(queries);
    cout << endl;
    */


    //load data
    vector<vector<int>> dataSet; // data
    vector<vector<pair<int, int>>> queryConstraints; // range query
    vector<pair<vector<int>, vector<int>*>> keyPointerPairs;

    string dataFilepath = argv[2];
    string queryFilepath = argv[3];
    int totalDim;
    int startDim = 0;
    int blockSize;
    if (stoi(argv[1]) != 0)
    {
        blockSize = stoi(argv[4]);
    }

    //Read and process data
    cout << "Reading in data..." << endl << endl;
    GetDataFromFile(dataFilepath, dataSet, totalDim);

    MakeKeyPointerPairs(dataSet, keyPointerPairs);

    GetRangeQueryFromFile(queryFilepath, queryConstraints);

    //sequential search
    if (stoi(argv[1]) == 0)
    {
        cout << "Sequential Search:" << endl;
        SequentialSearch(dataSet, queryConstraints);
    }

    //KD search
    else if (stoi(argv[1]) == 1)
    {
        cout << "KD tree search:" << endl;
        KDtree kdTree = KDtree(); 
        cout << "Creating tree..." << endl;
        kdTree.myRoot = kdTree.CreateTree(keyPointerPairs, startDim, blockSize, totalDim); //create KDtree

        cout << "Range Query:" << endl;
        auto start = high_resolution_clock::now();
        kdTree.RangeQuery(queryConstraints); //Search tree for all range queries
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time for KD Tree Range Query: " << duration.count() << " microseconds" << endl;
    }

    //myKD search
    else if (stoi(argv[1]) == 2)
    {
        cout << "MYkd tree search:" << endl;
        KDtree myKDTree = KDtree();
        cout << "Creating tree..." << endl;
        myKDTree.myRoot = myKDTree.CreateMyTree(keyPointerPairs, startDim, blockSize, totalDim); //create Mykdtree

        cout << "Range Query:" << endl;
        auto start = high_resolution_clock::now();
        myKDTree.RangeQuery(queryConstraints); //Search tree for all range queries
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time for MyKD Tree Range Query: " << duration.count() << " microseconds" << endl;
    }

    else
    {
        cout << "Input number is invalid" << endl;
    }

    return 0;
}
