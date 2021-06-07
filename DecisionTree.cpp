/**
 * Author : Arda Akça Büyük
 * ID: 21802835
 * Section : 3
 * Assignment : 2
 */

#include "DecisionTree.h"
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

DecisionTree::DecisionTree(): root(NULL) {}

DecisionTree::DecisionTree(DecisionTreeNode *nodePtr): root(nodePtr) {}


DecisionTree::DecisionTree(const DecisionTree &tree) {
    copyTree(tree.root, root);
}

void DecisionTree::copyTree(DecisionTreeNode *treePtr, DecisionTreeNode *&newTreePtr) const {
    if (treePtr != NULL) {
        newTreePtr = new DecisionTreeNode(treePtr->feature, treePtr->decision, NULL, NULL);
        copyTree(treePtr->left, newTreePtr->left);
        copyTree(treePtr->right, newTreePtr->right);
    }
    else
        newTreePtr = NULL;
}

DecisionTree::~DecisionTree() {
    destroyTree(root);
}

void DecisionTree::destroyTree(DecisionTreeNode *& treePtr) {
    if (treePtr != NULL){
        destroyTree(treePtr->left);
        destroyTree(treePtr->right);
        delete treePtr;
        treePtr = NULL;
    }
}

double DecisionTree::calculateEntropy(const int* classCounts, const int numClasses) {
    int sum = 0;
    for (int i = 0; i < numClasses; i++) {
        sum += classCounts[i];
    }
    
    double entropy = 0;
    double Pi;
    //ΣPi*log(Pi)
    for (int i = 0; i < numClasses; i++) {
        if (classCounts[i] != 0) {
            Pi = (double) classCounts[i] / sum;
            entropy += Pi * log2(Pi);
        }
    }
    entropy *= -1;
    return entropy;
}

double DecisionTree::calculateInformationGain(const bool** data, const int* labels,
                                const int numSamples, const int numFeatures, const bool* usedSamples,
                                const int featureId) {
    
    int leftNode = 0;
    int rightNode = 0;
    int totalSamples = 0;
    int numClasses = 0;
    
    for (int i = 0; i < numSamples; i++) {
        if (usedSamples[i]) {
            if (labels[i] > numClasses)
                numClasses = labels[i];

            totalSamples++;
            if (data[i][featureId] == 0)
                leftNode++;
            else
                rightNode++;
        }
    }
    
    int *parentClasses = new int[numClasses];
    int *leftClasses = new int[numClasses];
    int *rightClasses = new int[numClasses];
    
    for (int i = 0; i < numClasses; i++) {
        parentClasses[i] = 0;
        leftClasses[i] = 0;
        rightClasses[i] = 0;
    }
    
    for (int i = 0; i < numSamples; i++) {
        if (usedSamples[i]) {
            parentClasses[labels[i]-1] += 1;
            
            if (data[i][featureId] == 0)
                leftClasses[labels[i]-1] += 1;
            else
                rightClasses[labels[i]-1] += 1;
        }
    }
    
    double HP;
    double HS;
    double HLeft;
    double HRight;
    
    HP = calculateEntropy(parentClasses, numClasses);
    HLeft = calculateEntropy(leftClasses, numClasses);
    HRight = calculateEntropy(rightClasses, numClasses);
    HS = (((double) leftNode / totalSamples) * HLeft) + (((double) rightNode / totalSamples) * HRight);
    
    return HP - HS;
}

int DecisionTree::chooseFeature(const bool **data, const int *labels,
                                const int numSamples, const int numFeatures,
                                const bool *usedSamples, bool *availableFeatures) {
    int maxIGfeature = -1;
    double maxIG = 0;
    for (int i = 0; i < numFeatures; i++) {
        if (availableFeatures[i]) {
            double curIG = calculateInformationGain(data, labels, numSamples, numFeatures, usedSamples, i);
            if (curIG > maxIG) {
                maxIG = curIG;
                maxIGfeature = i;
            }
        }
    }
    
    if (maxIGfeature != -1)
        availableFeatures[maxIGfeature] = false;
    
    return maxIGfeature;
}

void DecisionTree::split(const bool **data, const int *labels,
                         const int numSamples, const int numFeatures,
                         DecisionTreeNode *node, bool *usedSamples,
                         bool *availableFeatures, const int feature) {
    
    bool leaf = true;
    int temp = -1;
    for (int i = 0; i < numSamples; i++) {
        if (usedSamples[i]) {
            if (temp == -1)
                temp = labels[i];
            
            if (labels[i] != temp) {
                leaf = false;
                break;
            }
        }
    }
    
    bool noFeatureLeft = true;
    for (int i = 0; i < numFeatures; i++) {
        if (availableFeatures[i])
            noFeatureLeft = false;
    }
    
    if (leaf) {
        node->feature = -1;
        node->decision = temp;
        return;
    }
    
    if (noFeatureLeft) {
        node->feature = -1;
        
        int classCounts[numSamples];
        for (int i = 0; i < numSamples; i++)
            classCounts[i] = 0;
        
        for (int i = 0; i < numSamples; i++) {
            if (usedSamples[i])
                classCounts[i] += 1;
        }
        
        int majority = -1;
        for (int i = 0; i < numSamples; i++) {
            if (usedSamples[i]) {
                if (classCounts[i] > majority)
                    majority = classCounts[i];
            }
        }
        node->decision = majority;
        return;
    }
 
    bool *usedLeft = new bool[numSamples];
    bool *usedRight = new bool[numSamples];
    
    for (int i = 0; i < numSamples; i++) {
        usedLeft[i] = usedSamples[i];
        usedRight[i] = usedSamples[i];
    }
    
    for (int i = 0; i < numSamples; i++) {
        if (usedSamples[i]) {
            if (data[i][feature] == 0)
                usedRight[i] = false;
            else
                usedLeft[i] = false;
        }
    }
    
    int maxIGfeatureLeft = chooseFeature(data, labels, numSamples, numFeatures, usedLeft, availableFeatures);
    int maxIGfeatureRight = chooseFeature(data, labels, numSamples, numFeatures, usedRight, availableFeatures);
    
    node->left = new DecisionTreeNode(maxIGfeatureLeft, -1, NULL, NULL);
    node->right = new DecisionTreeNode(maxIGfeatureRight, -1, NULL, NULL);
    split(data, labels, numSamples, numFeatures, node->left, usedLeft, availableFeatures, maxIGfeatureLeft);
    split(data, labels, numSamples, numFeatures, node->right, usedRight, availableFeatures, maxIGfeatureRight);
    
}

void DecisionTree::train(const bool **data, const int *labels, const int numSamples, const int numFeatures) {
    bool *usedSamples = new bool[numSamples];
    bool *availableFeatures = new bool[numFeatures];
    
    for (int i = 0; i < numSamples; i++)
        usedSamples[i] = true;
    for (int i = 0; i < numFeatures; i++)
        availableFeatures[i] = true;
    
    int maxIGfeature = chooseFeature(data, labels, numSamples, numFeatures, usedSamples, availableFeatures);
    root = new DecisionTreeNode(maxIGfeature, -1, NULL, NULL);
    split(data, labels, numSamples, numFeatures, root, usedSamples, availableFeatures, maxIGfeature);
}

void DecisionTree::train(const string fileName, const int numSamples, const int numFeatures) {
    ifstream in;
    in.open("train_data.txt");
    if (!in)
        cout << "Can't read file!";
    
    bool **data = new bool *[numSamples];
    for (int i = 0; i < numSamples; i++)
        data[i] = new bool[numFeatures];
   
    int *labels = new int[numSamples];
    
    int temp;
    for( int i = 0; i < numSamples; i++) {
        for( int j = 0; j < numFeatures; j++) {
            in >> temp;
            if (temp)
                data[i][j] = true;
            else
                data[i][j] = false;
        }
        in >> labels[i];
    }

    train((const bool**) data, labels, numSamples, numFeatures);
    in.close();
}

int DecisionTree::predict(const bool *data) {
    DecisionTreeNode *tmp = root;
    
    while (!tmp->isLeaf()) {
        if (!data[tmp->feature])
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    return tmp->decision;
}

double DecisionTree::test(const bool** data, const int *labels, const int numSamples) {
    int correct = 0;
    
    for (int i = 0; i < numSamples; i++) {
        if (predict((const bool*) data[i]) == labels[i])
            correct++;
    }
    
    return (double) correct / numSamples;
}

double DecisionTree::test(const string fileName, const int numSamples) {
    ifstream in;
    in.open("test_data.txt");
    string line;
    getline(in, line);
    int numFeatures = (int) (line.size() - 1) / 2;
    in.close();
    
    int temp;
    in.open("test_data.txt");
    bool **data = new bool *[numSamples];
    for (int i = 0; i < numSamples; i++)
        data[i] = new bool[numFeatures];
    int *labels = new int[numSamples];

    for( int i = 0; i < numSamples; i++) {
        for (int j = 0; j < numFeatures; j++) {
            in >> temp;
            if (temp)
                data[i][j] = true;
            else
                data[i][j] = false;
        }
        in >> labels[i];
    }
    
    in.close();
    return test((const bool**) data, labels, numSamples);
}

void DecisionTree::print() {
    print(root, 0);
}

void DecisionTree::print(DecisionTreeNode *node, int level) {
    if (node->isLeaf()) {
        for (int i = 0; i < level; i++)
            cout << "\t";
        
        cout << "Decision Node: class = " << node->decision << endl;
        return;
    }
    
    for (int i = 0; i < level; i++)
        cout << "\t";
    
    cout << "Split Node: feature index = " << node->feature << endl;
    print(node->left, level + 1);
    print(node->right, level + 1);
}
