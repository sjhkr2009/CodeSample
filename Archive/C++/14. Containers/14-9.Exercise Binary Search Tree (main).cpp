#include <iostream>
#include "Tree.h"

using namespace MyTree;

int main()
{
	BST myTree;

	myTree.Insert(3);
	myTree.Insert(5);
	myTree.Insert(4);
	myTree.Insert(4);
	myTree.Insert(1);
	myTree.Insert(2);
	myTree.Insert(7);
	myTree.Insert(6);

	myTree.Traversal();
	myTree.Traversal(BST::PreOrder);
	myTree.Traversal(BST::PostOrder);

	myTree.Find(4);
	myTree.Find(8);

	myTree.Traversal();

	myTree.Erase(3);
	myTree.Erase(1);

	myTree.Traversal();

	myTree.Clear();

	myTree.Traversal();
}