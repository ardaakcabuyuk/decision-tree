/**
 * Author : Arda Akça Büyük
 * ID: 21802835
 * Section : 3
 * Assignment : 2
 */

#ifndef __DECISIONTREE_H
#define __DECISIONTREE_H

#include <iostream>
#include <string>
#include "DecisionTreeNode.h"
using namespace std;

class DecisionTree {
public:
    DecisionTree();
    DecisionTree(DecisionTreeNode *nodePtr);
    DecisionTree(const DecisionTree& tree);
    ~DecisionTree();
    void train(const bool**, const int*, const int, const int);
    void train(const string, const int, const int);
    int predict(const bool*);
    double test(const bool**, const int*, const int);
    double test(const string, const int);
    void print();

private:
    DecisionTreeNode *root;
    void copyTree(DecisionTreeNode *treePtr, DecisionTreeNode *&newTreePtr) const;
    void destroyTree(DecisionTreeNode *&treePtr);
    double calculateEntropy(const int* classCounts, const int numClasses);
    double calculateInformationGain(const bool** data, const int* labels,
                                    const int numSamples, const int numFeatures, const bool* usedSamples,
                                    const int featureId);
    void split(const bool**, const int*, const int, const int, DecisionTreeNode*, bool*, bool*, const int);
    int chooseFeature(const bool**, const int*, const int, const int, const bool*, bool*);
    void print(DecisionTreeNode *node, int level);
};

#endif
