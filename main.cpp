/**
 * Author : Arda Akça Büyük
 * ID: 21802835
 * Section : 3
 * Assignment : 2
 */

#include <iostream>
#include <cmath>
#include "DecisionTree.h"
using namespace std;

int main() {
    DecisionTree tree;
    tree.train("train_data.txt", 498, 21);
    tree.print();
    cout << endl;
    cout << "Test Accuracy = " << tree.test("test_data.txt", 473) << endl;
}
