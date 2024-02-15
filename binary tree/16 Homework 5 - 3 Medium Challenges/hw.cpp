#include <iostream>
#include <sstream>	// ostringstream
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

string toStr(int n) {
	// Convert integer to string
	ostringstream oss;
	oss << n;
	return oss.str();
}

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
		for (int i = 0; i < (int) values.size(); ++i) 
		{
			if (direction[i] == 'L') 
			{
				if (!current->left)
					current->left = new BinaryTree(values[i]);
				else
					assert(current->left->data == values[i]);
				current = current->left;
			} 
			else 
			{
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

	void print_preorder() {
		cout << data << " ";
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
	}
	////////////////////////////////////////////

	bool is_mirror(BinaryTree* first, BinaryTree* second) {	// O(n). Think like preorder traversal
		if (!first && !second)
			return true;

		if (!first && second || first && !second || first->data != second->data)
			return false;	// one subtree only or different values

		// 2 trees will have 4 sub-trees. We need to make sure of 2 mirroring
		return is_mirror(first->left, second->right) && is_mirror(first->right, second->left);
	}

	bool is_symmetric_recursive () {
		return is_mirror(left, right);
	}

	string parenthesize(bool left_first) {	// O(n)
		string repr = "(" + toStr(data);

		if (left_first) 
		{
			if (left)
				repr += left->parenthesize(left_first);
			else
				repr += "()";	// null: no child

			if (right)
				repr += right->parenthesize(left_first);
			else
				repr += "()";	// null: no child
		} 
		else 
		{
			if (right)
				repr += right->parenthesize(left_first);
			else
				repr += "()";	// null: no child

			if (left)
				repr += left->parenthesize(left_first);
			else
				repr += "()";	// null: no child
		}
		repr += ")";

		return repr;
	}

	bool is_symmetric_traversal() {
		// We can prepare representation to the left/right and comapre!
		if (!left && !right)
			return true;

		if (!left && right || left && !right)
			return false;	// one subtree only

		// construct right subtrees, but always start with right to flip it
		return left->parenthesize(true) == right->parenthesize(false);
	}

	string parenthesize_canonical() {
		string repr = "(" + toStr(data);

		vector<string> v;

		if (left)
			v.push_back(left->parenthesize_canonical());
		else
			v.push_back("()");

		if (right)
			v.push_back(right->parenthesize_canonical());
		else
			v.push_back("()");

		sort(v.begin(), v.end());
		for (int i = 0; i < (int) v.size(); ++i)
			repr += v[i];
		repr += ")";

		return repr;
	}
	bool is_flip_equiv(BinaryTree* other) 
	{
		// This is a direct canonical problem!
		return parenthesize_canonical() == other->parenthesize_canonical();
		// We can also develop a recursive code that is based on
		// is_flip_equiv(left, other->left) && is_flip_equiv(right, other->right) ||
		// is_flip_equiv(left, other->right) && is_flip_equiv(right, other->left);
		// O(min(n, m)) where n & m are the 2 trees number of nodes
		// This is NOT O(n^2) code. Think like preorder traversal
	}

	string parenthesize(vector<string>& all_repres) 
	{
		// save the representation of every subtree
		string repr = "(" + toStr(data);

		if (left)
			repr += left->parenthesize(all_repres);
		else
			repr += "()";

		if (right)
			repr += right->parenthesize(all_repres);
		else
			repr += "()";

		repr += ")";

		if (left || right)	// nodes > 1
			all_repres.push_back(repr);  

		return repr;
	}

	// save all subtrees
	// sort them
	// group them and print duplicates
	void print_duplicate_subtrees() 
	{
		vector<string> all_repres;
		parenthesize(all_repres);

		sort(all_repres.begin(), all_repres.end());
		all_repres.push_back("#");
		//iterate over the for loop
		for (int i = 0; i < (int) all_repres.size();) 
		{
			// Find a group of equal strings
			int j = i+1;
			while(j < (int) all_repres.size() && all_repres[i] == all_repres[j])
				j++;

			if (j > i+1)
				cout << all_repres[i] << "\n";
			i = j;
		}
	}
};

void test1_is_symmetric() {
	BinaryTree tree(1);

	assert(tree.is_symmetric_traversal() == 1);

	tree.add( { 2 }, { 'L' });
	tree.add( { 2 }, { 'R' });

	assert(tree.is_symmetric_traversal() == 1);

	tree.add( { 2, 3, 5 }, { 'L', 'L', 'L' });
	tree.add( { 2, 3, 6 }, { 'L', 'L', 'R' });
	tree.add( { 2, 4, 7 }, { 'L', 'R', 'L' });
	tree.add( { 2, 4, 8 }, { 'L', 'R', 'R' });

	assert(tree.is_symmetric_traversal() == 0);

	tree.add( { 2, 4, 8 }, { 'R', 'L', 'L' });
	tree.add( { 2, 4, 7 }, { 'R', 'L', 'R' });
	tree.add( { 2, 3, 6 }, { 'R', 'R', 'L' });

	assert(tree.is_symmetric_traversal() == 0);

	tree.add( { 2, 3, 5 }, { 'R', 'R', 'R' });

	assert(tree.is_symmetric_traversal() == 1);
}

void test2_is_symmetric() {
	BinaryTree tree(1);

	tree.add( { 2, 3 }, { 'L', 'L' });
	tree.add( { 2, 4, 5 }, { 'L', 'R', 'L' });
	tree.add( { 2, 4, 6 }, { 'L', 'R', 'R' });

	tree.add( { 2, 4, 6 }, { 'R', 'L', 'L' });
	tree.add( { 2, 4, 5 }, { 'R', 'L', 'R' });
	tree.add( { 2, 3 }, { 'R', 'R' });

	assert(tree.is_symmetric_traversal() == 1);
}

void test_all_duplicates() {
	BinaryTree tree(1);
	/*
	tree.add( { 2, 4, 8 }, { 'L', 'L', 'L' });
	tree.add( { 2, 4, 9 }, { 'L', 'L', 'R' });
	tree.add( { 2, 5, 10 }, { 'L', 'R', 'L' });
	tree.add( { 2, 5, 11 }, { 'L', 'R', 'R' });

	tree.add( { 3, 6, 12 }, { 'R', 'L', 'L' });
	tree.add( { 3, 6, 13 }, { 'R', 'L', 'R' });
	tree.add( { 3, 7, 14 }, { 'R', 'R', 'L' });
	tree.add( { 3, 7, 15 }, { 'R', 'R', 'R' });
	tree.add( { 3, 7, 15, 16 }, { 'R', 'R', 'R', 'R' });
	*/

	tree.add( { 2, 3 }, { 'L', 'L'});
	tree.add( { 4, 5, 6, 8, 9 }, { 'R', 'R', 'R', 'R', 'R' });
	tree.add( { 4, 2, 3 }, { 'R', 'L', 'L'});
	tree.add( { 4, 5, 6, 7 }, { 'R', 'R', 'L', 'L'});
	tree.add( { 4, 5, 6, 8, 9 }, { 'R', 'R', 'L', 'R', 'R'});
	tree.add( { 4, 5, 6, 7 }, { 'R', 'R', 'R', 'L'});

	// (2(3()())())
	// (6(7()())(8()(9()())))
	// (8()(9()()))


	tree.print_duplicate_subtrees();
}

int main() {
	test_all_duplicates();

	cout << "bye\n";

	return 0;
}

