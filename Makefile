hw2: main.o DecisionTreeNode.o DecisionTree.o 
	g++ main.o DecisionTreeNode.o DecisionTree.o -o hw2

main.o: main.cpp
	g++ -c main.cpp

DecisionTreeNode.o: DecisionTreeNode.o
	g++ -c DecisionTreeNode.cpp

DecisionTree.o: DecisionTree.o
	g++ -c DecisionTree.cpp

clean: clean-custom
	rm* output
