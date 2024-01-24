#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
using namespace std;

class BinaryTree {
private:
	int data { };
	BinaryTree* left { };
	BinaryTree* right { };

public:

	BinaryTree(int data) :
			data(data) {
	}

	void add(vector<int> values, vector<char> direction) {
		assert(values.size() == direction.size());
		BinaryTree* current = this;
		// iterate on the path, create all necessary nodes
		for (int i = 0; i < (int) values.size(); ++i) {
			if (direction[i] == 'L') {
				if (!current->left)
					current->left = new BinaryTree(values[i]);
				else
					assert(current->left->data == values[i]);
				current = current->left;
			} else {
				if (!current->right)
					current->right = new BinaryTree(values[i]);
				else
					assert(current->right->data == values[i]);
				current = current->right;
			}
		}
	}

	void print_inorder() {
		if (left)
			left->print_inorder();
		cout << data << " ";
		if (right)
			right->print_inorder();
	}
	////////////////////////////////////////////

	int tree_max() {
		int res = data;
		if (left)
			res = max(res, left->tree_max());
		if (right)
			res = max(res, right->tree_max());
		return res;
	}

	int tree_height() {
		int res = 0;
		if (left)
			res = 1 + left->tree_height();
		if (right)
			res = max(res, 1 + right->tree_height());
		return res;
	}
	
	int total_nodes() {
		int res = 1;
		if (left)
			res += left->total_nodes();
		if (right)
			res += right->total_nodes();
		return res;
	}
	
	int leafs_count() {
		int res = !left && !right;
		if (left)
			res += left->leafs_count();
		if (right)
			res += right->leafs_count();
		return res;
	}

	bool is_exists(int value) {
		bool res = value == data;
		if (!res && left)
			res = left->is_exists(value);
		if (!res && right)
			res = right->is_exists(value);
		return res;
	}

	bool is_perfect(int h = -1) 
	{
		if (h == -1)	// first call
			h = tree_height();

		// All leaves must be on the same last level
		if (!left && !right)
			return h == 0;

		if (!left && right || left && !right)
			return false;	// one child!

		// perfect on level h is perfect on h-1 for children
		return left->is_perfect(h - 1) && right->is_perfect(h - 1);
	}
	
	bool is_perfect_fomula() {
		int h = tree_height();
		int n = total_nodes();
		return  n==pow(2, h + 1) - 1;
	}
};

int main() {

	cout << "bye\n";

	return 0;
}

