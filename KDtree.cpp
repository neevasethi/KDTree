#include "KDtree.h"
using namespace std;

//All Helper Functions

//returns a vector of all data of a specified dimension
vector <int> KDtree::ExtractDimensionalData(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension)
{
    vector<int> result;
    for (int i = 0; i < keyPointerPairs.size(); i++)
    {
        vector<int> tmp = keyPointerPairs[i].first;
        int valueToExtract = tmp[dimension];
        result.push_back(valueToExtract);
    }
    return result;
}

//returns the median of the data based on a specified dimension 
int KDtree::FindMedian(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension)
{
    //get all values of specified dimension
    vector<int> dimValues = ExtractDimensionalData(keyPointerPairs, dimension);
    sort(dimValues.begin(), dimValues.end());

    //if odd size
    if (dimValues.size() % 2 == 1)
    {
        return dimValues[(dimValues.size() / 2)];
    }

    //if even size
    else
    {
        return (dimValues[(dimValues.size() / 2)] + dimValues[(dimValues.size() / 2 - 1)]) / 2;
    }
}

//returns all tuples < median based on dimension
vector<pair<vector<int>, vector<int>*>> KDtree::SplitDataLeft(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension, int& median)
{
    vector<pair<vector<int>, vector<int>*>> left;
    for (int i = 0; i < keyPointerPairs.size(); i++)
    {
        vector<int> tmp = keyPointerPairs[i].first;
        if (tmp[dimension] < median)
        {
            left.push_back(keyPointerPairs[i]);
        }
    }
    return left;
}

//return all tuples >= median based on dimension
vector<pair<vector<int>, vector<int>*>> KDtree::SplitDataRight(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension, int& median)
{
    vector<pair<vector<int>, vector<int>*>> right;
    for (int i = 0; i < keyPointerPairs.size(); i++)
    {
        vector<int> tmp = keyPointerPairs[i].first;
        if (tmp[dimension] >= median)
        {
            right.push_back(keyPointerPairs[i]);
        }
    }
    return right;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// KDtree Functions

KDtree::KDtree()
{
    myRoot = nullptr;
}

KDtree::KDTreeNode* KDtree::CreateTree(vector<pair<vector<int>,vector<int>*>>& keyPointerPairs, int& dimension, int& blockSize, int& numDimensions)
{
    if (keyPointerPairs.size() <= blockSize)
    {
        //make a leaf node
        KDTreeNode* leaf = new KDTreeNode();
        leaf->pageSize = blockSize;
        leaf->leafData = keyPointerPairs;
        return leaf;
    }

    else
    {
        //make an internal node
        int median = FindMedian(keyPointerPairs, dimension);
        KDTreeNode* root = new KDTreeNode(dimension, median);


        //change dimension to split on
        int newDim = dimension + 1;
        if (newDim == numDimensions)
        {
            newDim = 0;
        }

        //split data
        vector<pair<vector<int>,vector<int>*>> leftDataSet = SplitDataLeft(keyPointerPairs, dimension, median);
        vector<pair<vector<int>,vector<int>*>> rightDataSet = SplitDataRight(keyPointerPairs, dimension, median);
        
        //assign new children
        root->left = CreateTree(leftDataSet, newDim, blockSize, numDimensions);
        root->right = CreateTree(rightDataSet, newDim, blockSize, numDimensions);
        return root;
    }
    cout << "error in creating tree";
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////
//MykdTree Functions

int KDtree::FindLargestRange(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& numDimensions)
{
    int maxRange = 0;
    int newDim = 0;
    //get all values of specified dimension
    for (int i = 0; i < numDimensions; i++)
    {
        vector<int> values = ExtractDimensionalData(keyPointerPairs, i);
        sort(values.begin(), values.end());

        if (values.at(values.size() - 1) - values.at(0) > maxRange)
        {
            maxRange = values.at(values.size() - 1) - values.at(0);
            newDim = i;
        }
    }
    return newDim;
}

KDtree::KDTreeNode* KDtree::CreateMyTree(vector<pair<vector<int>, vector<int>*>>& keyPointerPairs, int& dimension, int& blockSize, int& numDimensions)
{
    if (keyPointerPairs.size() <= blockSize)
    {
        //make a leaf node
        KDTreeNode* leaf = new KDTreeNode();
        leaf->pageSize = blockSize;
        leaf->leafData = keyPointerPairs;
        return leaf;
    }

    else
    {

        //make an internal node
        int median = FindMedian(keyPointerPairs, dimension);
        KDTreeNode* root = new KDTreeNode(dimension, median);

        //split data
        vector<pair<vector<int>, vector<int>*>> leftDataSet = SplitDataLeft(keyPointerPairs, dimension, median);
        vector<pair<vector<int>, vector<int>*>> rightDataSet = SplitDataRight(keyPointerPairs, dimension, median);

        //go through both your left and right data sets and find which dimension has the largest range -> split on that range
        int newDimLeft = FindLargestRange(leftDataSet, numDimensions);
        int newDimRight = FindLargestRange(rightDataSet, numDimensions);

        //assign new children
        root->left = CreateMyTree(leftDataSet, newDimLeft, blockSize, numDimensions);
        root->right = CreateMyTree(rightDataSet, newDimRight, blockSize, numDimensions);
        return root;
    }
    cout << "error in creating tree";
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////
//Range Query Functions
void KDtree::RangeQuery(vector<vector<pair<int, int>>>& queries)
{
    for (int k = 0; k < queries.size(); k++)
    {
        vector<vector<int>*> returnData;
        vector<pair<int, int>> myQuery = queries[k];
        //call helper function
        RangeQuerySearch(&returnData, myQuery, myRoot);

        //sort data to easily compare outputs

        //Load data from "disk" and sort
        vector<vector<int>> finalReturnData;
        for (int i = 0; i < returnData.size(); i++)
        {
            finalReturnData.push_back(*(returnData[i]));
        }
        sort(finalReturnData.begin(), finalReturnData.end());

        //print data
        cout << "Query: ";
        for (auto x : myQuery)
        {
            cout << x.first << " ";
            cout << x.second << " ";
        }
        cout << endl;

        for (int i = 0; i < finalReturnData.size(); i++)
        {
            vector<int> tuple = finalReturnData[i];
            int j = 0;
            for (j; j < tuple.size() - 1; j++)
            {
                cout << tuple[j] << ",";
            }
            cout << tuple[j] << endl;
        }
        cout << "number of queries: " << finalReturnData.size() << endl << endl;
    }
    
}

void KDtree::RangeQuerySearch(vector<vector<int>*>* returnValues, vector<pair<int, int>>& query, KDTreeNode* root)
{
    //check if node is a leaf node
    if (root->leafData.size() != 0)
    {
        //place all valid tuples in return vector

        vector<pair<vector<int>, vector<int>*>>& leafData = root->leafData;
        for (int i = 0; i < leafData.size(); i++)
        {
            vector<int> tuple = leafData[i].first;

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

            //add to return vector if tuple passes all checks
            if (numFails == 0)
            {
                returnValues->push_back(leafData[i].second);
            }
        }
    }

    else
    {
        //get basic data
        int dimension = root->dimension;
        int dimVal = root->dimValue;
        int dimMin = query[dimension].first;
        int dimMax = query[dimension].second;

        //check if you can traverse left
        if (dimMin < dimVal && root->left != nullptr)
        {
            RangeQuerySearch(returnValues, query, root->left);
        }

        //check if you can traverse right
        if (dimMax >= dimVal && root->right != nullptr)
        {
            RangeQuerySearch(returnValues, query, root->right);
        }
    }
}




