#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
using namespace std;

deque<int> traversal;

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
		traversal.push_back(data);
		if (right)
			right->print_inorder();
	}

	void print_preorder() {
		cout << data << " ";
		traversal.push_back(data);
		if (left)
			left->print_preorder();
		if (right)
			right->print_preorder();
	}

	void print_postorder() {
		if (left)
			left->print_postorder();
		if (right)
			right->print_postorder();
		cout << data << " ";
		traversal.push_back(data);
	}
	////////////////////////////////////////////

	BinaryTree(deque<int> &preorder, deque<int> &inorder, int inorder_start = 0, int inorder_end = -1) 
	{
		if (inorder_end == -1)
			inorder_end = (int) inorder.size() - 1;

		// next preorder is the current root
		data = preorder[0];
		preorder.pop_front();

		// Find where is this root in the inorder current sequence
		for (int i = inorder_start; i <= inorder_end; ++i) 
		{
			if (inorder[i] == data) 
			{
				if ( i > inorder_start )	// there is a left tree
					left = new BinaryTree(preorder, inorder, inorder_start, i - 1);
				if (i < inorder_end)		// there is a right tree
					right = new BinaryTree(preorder, inorder, i + 1, inorder_end);
				break;
			}
		}
	}

	// Construct tree from its preorder info and nodes status
	// queue of: pair of (value, is_leaf)
	BinaryTree(queue<pair<int, int>> &preorder_queue) 
	{
		// Follows preorder VLR order
		pair<int, int> p = preorder_queue.front();
		preorder_queue.pop();
		data = p.first;

		if (!p.second) 	// non-leaf
		{ 
			if(preorder_queue.size())
				left = new BinaryTree(preorder_queue);
			if(preorder_queue.size())
				right = new BinaryTree(preorder_queue);
		}
	}

	// preorder pair of (value, is_leaf)
	void build_preorder(queue<pair<int, int> > &preorder_queue) 
	{
		preorder_queue.push(make_pair(data, !left && !right));

		if (left)
			left->build_preorder(preorder_queue);

		if (right)
			right->build_preorder(preorder_queue);
	}
};


void test1() {
	BinaryTree tree(1);

	tree.add( { 2}, { 'L'});
	tree.add( { 3}, { 'R'});

	traversal.clear();
	tree.print_preorder();
	cout << "\n";
	deque<int> preorder = traversal;

	traversal.clear();
	tree.print_inorder();
	cout << "\n";
	deque<int> inorder = traversal;

	BinaryTree tree_created(preorder, inorder);
	tree_created.print_preorder();
}

void test2() {
	BinaryTree tree(1);

	tree.add( { 2, 4, 7 }, { 'L', 'L', 'L' });
	tree.add( { 2, 4, 8 }, { 'L', 'L', 'R' });
	tree.add( { 2, 5, 9 }, { 'L', 'R', 'R' });
	tree.add( { 3, 6, 15 }, { 'R', 'R', 'L' });

	tree.add( { 2, 5, 13 }, { 'L', 'R', 'L' });
	tree.add( { 3, 6, 12 }, { 'R', 'R', 'R' });
	tree.add( { 3, 14, 15 }, { 'R', 'L', 'L' });
	tree.add( { 3, 14, 16 }, { 'R', 'L', 'R' });

	traversal.clear();
	tree.print_preorder();
	cout << "\n";
	deque<int> preorder = traversal;

	traversal.clear();
	tree.print_inorder();
	cout << "\n";
	deque<int> inorder = traversal;

	BinaryTree tree_created(preorder, inorder);
	tree_created.print_preorder();
}

void test3_from_is_leaf() {
	BinaryTree tree(1);

	tree.add( { 2, 4 }, { 'L', 'L'});
	tree.add( { 3 }, { 'R'});


	tree.add( { 2, 5, 6 }, { 'L', 'R', 'L' });

	tree.add( { 2, 5, 7, 8, 10 }, { 'L', 'R', 'R', 'L', 'L' });

	tree.add( { 2, 5, 7, 8, 11 }, { 'L', 'R', 'R', 'L', 'R' });

	tree.add( { 2, 5, 7, 9 }, { 'L', 'R', 'R', 'R' });



	queue<pair<int, int> > preorder_queue;
	tree.build_preorder(preorder_queue);
	tree.print_preorder();
	cout<<"\n";


	BinaryTree tree_created(preorder_queue);
	tree_created.print_preorder();

}



int main() {
	test3_from_is_leaf();

	cout << "bye\n";

	return 0;
}

