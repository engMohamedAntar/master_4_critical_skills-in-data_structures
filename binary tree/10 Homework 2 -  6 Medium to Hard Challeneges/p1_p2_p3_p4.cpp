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
	//////////////////

	~BinaryTree() {
		clear();
	}

	void clear() {
		// Don't try to call clear for children and also delete. This deletes twice!
		if (left) {
			delete left;	// recursively delete
			left = nullptr;	// good practice
		}

		if (right) {
			delete right;
			left = right;
		}
	}






























	void print_inorder_iterative() {
		// Pair: node and it has been expanded or not so far
		stack<pair<BinaryTree*, bool> > nodes;

		// Just convert the recursion to calls
		nodes.push(make_pair(this, false));

		while (!nodes.empty()) 
		{
			BinaryTree* current = nodes.top().first;
			bool is_done = nodes.top().second;
			nodes.pop();

			// If expanded already and we are here, then we just returned from recursion. Print
			if (is_done)
				cout << current->data << " ";
			else 
			{
				// Push your children and mark yourself as expanded
				// Observe: put right, then left as stack verse things. We need left prossed first
				if (current->right)
					nodes.push(make_pair(current->right, false));

				nodes.push(make_pair(current, true));

				if (current->left)
					nodes.push(make_pair(current->left, false));
			}
		}
		cout << "\n";
		// There is another way for this problem based on tracing the nodes in order
		// e.g. keep expand left and add to stack
		// But approach here is nicer as it follows the recurrance itself (generic style)
	}

	void traverse_left_boundry() {
		cout << data << " ";

		if (left)
			left->traverse_left_boundry();
		else if (right)
			right->traverse_left_boundry();
	}

	// https://leetcode.com/problems/diameter-of-binary-tree
	pair<int, int> tree_diameter() {	// return diam & height
		pair<int, int> res = make_pair(0, 0);

		if (!left && !right)
			return res;

		pair<int, int> left_diam = make_pair(0, 0);
		pair<int, int> right_diam = make_pair(0, 0);

		// Diameter either pass with the root or not: 2 cases
		// diameter if passes with me: then it is the longest height from left to me to right height

		if (left)
			left_diam = left->tree_diameter(), res.first += 1 + left_diam.second;	// edge + its height
		if (right)
			right_diam = right->tree_diameter(), res.first += 1 + right_diam.second;

		// diameter if in one of my children subtrees
		res.first = max(res.first, max(left_diam.first, right_diam.first));

		// normal height computation
		res.second = 1 + max(left_diam.second, right_diam.second);

		return res;
	}


};
void test4_diameter() {
	BinaryTree tree(1);

	assert(tree.tree_diameter().first == 0);

	tree.add( { 2 }, { 'L' });
	tree.add( { 3 }, { 'R' });

	assert(tree.tree_diameter().first == 2);

	tree.add( { 2, 4, 7 }, { 'L', 'L', 'L' });
	tree.add( { 2, 4, 8 }, { 'L', 'L', 'R' });
	tree.add( { 2, 5, 9 }, { 'L', 'R', 'R' });
	tree.add( { 3, 6, 15 }, { 'R', 'R', 'L' });

	tree.add( { 2, 5, 13 }, { 'L', 'R', 'L' });
	tree.add( { 3, 6, 12 }, { 'R', 'R', 'R' });
	tree.add( { 3, 14, 15 }, { 'R', 'L', 'L' });
	tree.add( { 3, 14, 16 }, { 'R', 'L', 'R' });

	assert(tree.tree_diameter().first == 6);
}

void test5_boundry() {
	BinaryTree tree(1);

	tree.add( { 2, 4, 5, 6, 7, 9, 11 }, { 'L', 'L', 'R', 'R', 'L', 'L', 'R' });
	tree.add( { 2, 4, 5, 6, 8 }, { 'L', 'L', 'R', 'R', 'R' });
	tree.add( { 2, 4, 5, 6, 7, 10 }, { 'L', 'L', 'R', 'R', 'L', 'R' });
	tree.add( { 3 }, { 'R' });

	tree.traverse_left_boundry();
	// 1 2 4 5 6 7 9 11

	cout<<"\n";
	tree.print_inorder_iterative();
}


int main() {
	test4_diameter();
	test5_boundry();

	cout << "bye\n";

	return 0;
}

