#include "heap.h"
#include "BSTree.h"
#include "AVLTree.h"
#include "RBTree.h"
int main() {

	//HEAP::TestTopKSmall(1'000'000, 10);

	/*vector<int> v = { 12,5,45,13,1,5,321,684,35,15,6,3,4,5,4,6 };
	for (const auto& e : v) cout << e << " ";
	heapSort(v);
	cout << endl;
	for (const auto& e : v) cout << e << " ";*/

	//BST::dictionary();
	//BST::testErase();
	//AVL::testAVL();
	RBT::testRBT();
	return 0;
}