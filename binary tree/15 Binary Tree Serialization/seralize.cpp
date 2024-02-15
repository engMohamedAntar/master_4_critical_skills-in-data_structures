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

	void print_preorder_complete() {
		cout<<data<<" ";

		if (left)
			left->print_preorder_complete();
		else
			cout<<"-1 ";	// 2 null pointers

		if (right)
			right->print_preorder_complete();
		else
			cout<<"-1 ";	// 2 null pointers
	}

	string parenthesize() {
		string repr = "(" + toStr(data);

		if (left)
			repr += left->parenthesize();
		else
			repr += "()";	// null: no child

		if (right)
			repr += right->parenthesize();
		else
			repr += "()";	// null: no child
		
		repr += ")";

		return repr;
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
		for (int i = 0; i < (int)v.size(); ++i)
			repr += v[i];

		repr += ")";

		return repr;
	}

};






void test1() {
	BinaryTree tree(1);

	tree.add( { 2}, { 'L'});
	tree.add( { 3 }, { 'R' });


	cout<<tree.parenthesize();;

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

}

int main() {
	test1();

	cout << "bye\n";

	return 0;
}

