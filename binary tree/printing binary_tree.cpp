#include <iostream>
#include <cassert>
using namespace std;
struct Node
{
	string value;
	Node* right {};
	Node* left {};
	public:
	Node(string value):value(value){
	}
};
void print_postorder(Node* current)
{
	if(!current)
		return;
	print_postorder(current->left);
	print_postorder(current->right);
	cout<<current->value<<" ";
}
void print_inorder(Node* current)
{
	if(!current)
		return;
	print_inorder(current->left);
	cout<<current->value;
	print_inorder(current->right);
}
void print_preorder(Node* current)
{
	if(!current)
		return;
	cout<<current->value;
	print_inorder(current->left);
	print_inorder(current->right);
}
int main() {
	Node* one= new Node("1");
	Node* two= new Node("2");
	Node* three= new Node("3");
	Node* four= new Node("4");
	Node* five= new Node("5");
	Node* six= new Node("6");
	Node* seven= new Node("7");
	Node* eight= new Node("8");
	Node* nine= new Node("9");
	Node* ten= new Node("10");
	one->left=two;
	two->left=four;
	four->left=seven;
	four->right=eight;
	two->right=five;
	five->right=nine;
	one->right=three;
	three->right=six;
	six->left=ten;
	print_postorder(one);
	return 0;
}
