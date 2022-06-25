#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include<string>
#include <sstream>
using namespace std;

class KDtree
{
public: 

    struct KDTreeNode
    {
        //pertains to leaf nodes only
        int pageSize;
        vector<pair<vector<int>, vector<int>*>> leafData;
        //


        int dimension; //dimension identifier
        int dimValue; //median to split on

        KDTreeNode* left;
        KDTreeNode* right;
        KDTreeNode() : dimension(0), dimValue(0), left(nullptr), right(nullptr) {}
        KDTreeNode(int x, int x2) : dimension(x), dimValue(x2), left(nullptr), right(nullptr) {}
        KDTreeNode(int x, int x2, KDTreeNode* left, KDTreeNode* right) : dimension(x), dimValue(x2), left(left), right(right) {}
    };

    //variables
    KDTreeNode* myRoot;

    //constructor
    KDtree();

    //Create Trees
    KDTreeNode* CreateTree(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension, int& blockSize, int& numDimensions);
    KDTreeNode* CreateMyTree(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension, int& blockSize, int& numDimensions);

    //RangeQuery
    void RangeQuery(vector<vector<pair<int, int>>>& queries);
    void RangeQuerySearch(vector<vector<int>*>* returnValues, vector<pair<int, int>>& query, KDTreeNode* root);

    //helper functions
    vector <int> ExtractDimensionalData(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension);
    int FindMedian(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension);
    vector<pair<vector<int>, vector<int>*>> SplitDataLeft(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension, int& median);
    vector<pair<vector<int>, vector<int>*>> SplitDataRight(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension, int& median);
    int FindLargestRange(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& numDimensions);
};

