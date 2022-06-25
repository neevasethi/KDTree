# KDTree and MYKDTree
This project was developed by myself and Neeva Sethi. I developed the KD Tree methods and functions, range query functions, and the helper methods in the main.cpp file. Neeva developed the MyKD Tree methods and functions as well as the main function in main.cpp. I have included the main file, the KDTree header and cpp, which includes MyKD Tree, along with a dataset and range query.

About the KD Tree:
A KD tree is a data structure for organizing k-dimensional data points. The tree takes the first dimension, finds the median, and splits the data on this median at the node. The next level splits the data on the next dimension, and so on. Once all dimensions are split on, the order circles back to the first dimension. This process is repeated until the entirety of the data fits in all leaf nodes. Once the data is stored in the tree, range queries can be called on the database to find all values within the query. 

The MyKD tree implementation is a variation of the KD tree, where instead of incrementing the dimension to split the data on, the dimension is chosen based on which has the largest range of values, therefore more efficiently partitioning the data. MyKD Tree showed more efficient run times than the kd tree as the range size increased. 
