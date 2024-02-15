#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
using namespace std;

class BinarySearchTree {
private:
	int data { };
	BinarySearchTree* left { };
	BinarySearchTree* right { };

public:

	BinarySearchTree(int data) :
			data(data) {
	}

	void print_inorder() {
		if (left)
			left->print_inorder();
		cout << data << " ";
		if (right)
			right->print_inorder();
	}
	////////////////////////////////////////////


	bool search(int target) // tail recursion
	{		
		//In case of equalit
		if (target == data)
			return true;
		//In case of <
		if (target < data)
			return left && left->search(target);
		//In case of >
		return right && right->search(target);
	}

};

int main() {	

	cout << "bye\n";

	return 0;
}

