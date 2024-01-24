#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
using namespace std;

#include <stack>
using namespace std;

class BinaryTree {
private:
	char data { };
	BinaryTree* left { };
	BinaryTree* right { };

public:

	BinaryTree(int data) :
			data(data) {
	}

	bool is_just_num() {
		// If it has no children, then it is just a number
		return !left && !right;
	}

	void print_postorder() 
	{
		if (left)
			left->print_postorder();

		if (right)
			right->print_postorder();
		cout << data;
	}

	// Similar logic to postfix evaluation
	// If num, just push. If operator, pop last 2
	// https://www.krivalar.com/data-structures-expression-tree
	BinaryTree(string postfix) 
	{
		stack<BinaryTree*> tree;
    // 23+4*
		for (int i = 0; i < (int) postfix.size(); ++i) 
		{
			BinaryTree* cur = new BinaryTree(postfix[i]);

			if (!isdigit(postfix[i])) 
			{
				cur->right = tree.top();
				tree.pop();
				cur->left = tree.top();
				tree.pop();
			}
			tree.push(cur);
		}
		BinaryTree* root = tree.top();
		this->data = root->data;
		this->left = root->left;
		this->right = root->right;
	}

	void print_inorder_expression() 
	{
		if (left) 
		{
			if (!left->is_just_num()) // if left is a mathmatical operation
				cout << "(";
			left->print_inorder_expression();
			if (!left->is_just_num())
				cout << ")";
		}

		cout << data;

		if (right) 
		{
			if (!right->is_just_num()) // if right is a mathmatical operation
				cout << "(";

			right->print_inorder_expression();

			if (!right->is_just_num())
				cout << ")";
		}
	}
	void print_inorder()
	{
		if(left)
			left->print_inorder();
		cout<<data<<" ";
		if(right)
			right->print_inorder();
	}
};

void test1() {
	BinaryTree root("51+");

	cout << "infix expression: ";
	root.print_inorder_expression();
	cout << "\n";
	// infix expression: 5+1

}

void test2() {
	BinaryTree root("51+2/");

	cout << "infix expression: ";
	root.print_inorder_expression();
	cout << "\n";
	// infix expression: (5+1)/2
}

void test3() {
	BinaryTree root("23+4*");

	cout << "infix expression: ";
	root.print_inorder_expression();
	cout << "\n";
	// infix expression: (2+3)*4
}


void test4() {
	BinaryTree root("534*2^+");

	cout << "infix expression: ";
	root.print_inorder_expression();
	cout << "\n";
	// infix expression: 5+((3*4)^2)
}

int main() {
	// test1();
	// test2();
	// test3();
	// test4();
	BinaryTree tree("23+4*");
	tree.print_inorder();
	return 0;
}
