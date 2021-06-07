/**
 * Author : Arda Akça Büyük
 * ID: 21802835
 * Section : 3
 * Assignment : 2
 */

#include "DecisionTreeNode.h"

DecisionTreeNode::DecisionTreeNode(int feature,
                                   int decision,
                                   DecisionTreeNode *leftPtr,
                                   DecisionTreeNode *rightPtr): feature(feature), decision(decision), left(leftPtr), right(rightPtr){}

bool DecisionTreeNode::isLeaf() {
    if (decision != -1)
        return true;
    return false;
}
