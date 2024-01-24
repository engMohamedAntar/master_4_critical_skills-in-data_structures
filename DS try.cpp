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

	void add(vector<int> values, vector<char> direction) 
	{
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

	int tree_height() 
	{
		int res = 0;
		if (left)
			res = 1 + left->tree_height();
		if (right)
			res = max(res, 1 + right->tree_height());
		return res;
	}

	void print_nodes_level(int level) 
	{
		if(level==0)
			cout<<data<<" ";
		else
		{
			if(left)
				left->print_nodes_level(level-1);
			if(right)
				right->print_nodes_level(level-1);			
		}
	}

	void level_order_traversal_recursive() // O(n^2)
	{	
		int h=tree_height();
		for(int i=0;i<=h;i++)
			print_nodes_level(i);
	}


	void level_order_traversal_spiral() {
		// It seems as if we need one level proceeded with a normal queue
		// But another with a stack
		// a deque allows both processing easily!

		deque<BinaryTree*> nodes_queue;
		nodes_queue.push_back(this);

		int level = 0;
		bool forward_level = true;

		while (!nodes_queue.empty()) {
			int sz = nodes_queue.size();

			cout << "Level " << level << ": ";
			while (sz--) {
				BinaryTree*cur;

				if (forward_level) {	// generate from front
					// Usual queue style: pop queue front, push queue rear: left, then right
					cur = nodes_queue.front();
					nodes_queue.pop_front();

					if (cur->left)
						nodes_queue.push_back(cur->left);

					if (cur->right)
						nodes_queue.push_back(cur->right);
				} else {				// generate  from back
					// like Stack: pop stack end, push stack front: right, then left
					cur = nodes_queue.back();
					nodes_queue.pop_back();

					// observe from the end, the right comes before left
					if (cur->right)
						nodes_queue.push_front(cur->right);

					if (cur->left)
						nodes_queue.push_front(cur->left);
				}
				cout << cur->data << " ";
			}
			forward_level = !forward_level;
			level++;
			cout << "\n";
		}
	}

	// https://leetcode.com/problems/check-completeness-of-a-binary-tree
	bool is_complete() {
		queue<BinaryTree*> nodes_queue;
		nodes_queue.push(this);

		// Once there is a single missing node (left before right)
		// Nothing else should be there: on this level or next level
		bool no_more_allowed = false;


		while (!nodes_queue.empty()) {
			int sz = nodes_queue.size();

			while (sz--) {
				BinaryTree*cur = nodes_queue.front();
				nodes_queue.pop();

				if (cur->left) {
					if (no_more_allowed)
						return false;
					nodes_queue.push(cur->left);
				} else
					no_more_allowed = true;

				if (cur->right) {
					if (no_more_allowed)
						return false;
					nodes_queue.push(cur->right);
				} else
					no_more_allowed = true;
			}
		}
		return true;
	}

};

void test1_level_order_traversal_spiral() {
	BinaryTree tree(1);

	tree.add( { 2, 4, 8 }, { 'L', 'L', 'L' });
	tree.add( { 2, 4, 9 }, { 'L', 'L', 'R' });
	tree.add( { 2, 5, 10 }, { 'L', 'R', 'L' });
	tree.add( { 2, 5, 11 }, { 'L', 'R', 'R' });

	tree.add( { 3, 6, 12 }, { 'R', 'L', 'L' });
	tree.add( { 3, 6, 13 }, { 'R', 'L', 'R' });
	tree.add( { 3, 7, 14 }, { 'R', 'R', 'L' });

	//tree.level_order_traversal_recursive();

	tree.level_order_traversal_spiral();
	// Level 0: 1
	// Level 1: 3 2
	// Level 2: 4 5 6 7
	// Level 3: 15 14 13 12 11 10 9 8
}

void test2_is_complete() {
	BinaryTree tree(1);

	assert(tree.is_complete());

	tree.add( { 2, 4}, { 'L', 'L'});
	tree.add( { 2, 5}, { 'L', 'R'});
	tree.add( { 3}, { 'R'});

	assert(tree.is_complete());

	tree.add( { 3, 7}, { 'R', 'R'});
	assert(!tree.is_complete());


	tree.add( { 2, 4, 8 }, { 'L', 'L', 'L' });
	tree.add( { 2, 4, 9 }, { 'L', 'L', 'R' });
	tree.add( { 2, 5, 10 }, { 'L', 'R', 'L' });
	tree.add( { 2, 5, 11 }, { 'L', 'R', 'R' });

	assert(!tree.is_complete());

	tree.add( { 3, 6, 12 }, { 'R', 'L', 'L' });
	tree.add( { 3, 6, 13 }, { 'R', 'L', 'R' });
	tree.add( { 3, 7, 14 }, { 'R', 'R', 'L' });
	tree.add( { 3, 7, 15 }, { 'R', 'R', 'R' });

	assert(tree.is_complete());



}

int main() {
	

	return 0;
}

