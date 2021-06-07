/**
 * Author : Arda Akça Büyük
 * ID: 21802835
 * Section : 3
 * Assignment : 2
 */

#ifndef __DECISIONTREENODE_H
#define __DECISIONTREENODE_H

#include <iostream>

class DecisionTreeNode {
private:
    DecisionTreeNode(){}
    DecisionTreeNode(const int feature = -1,
                     const int decision = -1,
                     DecisionTreeNode *leftPtr = NULL,
                     DecisionTreeNode *rightPtr = NULL);
    
    int feature;
    DecisionTreeNode *left;
    DecisionTreeNode *right;
    int decision;
    
    bool isLeaf();
    friend class DecisionTree;
};

#endif
