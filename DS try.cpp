#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

// return the index of the first element great than v[i]
// If no such element, use v.size()
deque<int> next_greater_idx(deque<int> v) 
{
	stack<int> pos;

	for(int i=0; i< (int) v.size();i++)
	{
		while(!pos.empty() &&v[i] > v[pos.top()])
		{
			v[pos.top()]=i;
			pos.pop();
		}
		pos.push(i);
	}

	while (!pos.empty())
	{
		v[pos.top()]=v.size();
		pos.pop();
	}
	return v;
}

class BinarySearchTree {
private:
	int data { };
	BinarySearchTree* left { };
	BinarySearchTree* right { };

public:

	BinarySearchTree(int data, BinarySearchTree* left = nullptr, BinarySearchTree* right = nullptr) :
			data(data), left(left), right(right) {
	}

	void print_inorder() {
		if (left)
			left->print_inorder();

		cout << data << " ";
		if (right)
			right->print_inorder();
	}

	void insert(int target) {
		if (target < data) {
			if (!left) {
				left = new BinarySearchTree(target);
			} else
				left->insert(target);
		} else if (target > data) {
			if (!right)
				right = new BinarySearchTree(target);
			else
				right->insert(target);
		} // else: exists already
	}

	////////////////////////////////////////////

	void build_preorder(deque<int> &preorder_queue) {
		preorder_queue.push_back(data);

		if (left)
			left->build_preorder(preorder_queue);

		if (right)
			right->build_preorder(preorder_queue);
	}

	// We will improve the previous code by replacing O(n)
	// We know this code just find the next greater element!
	bool next_between(deque<int>&preorder,int min,int max)
	{
		if(preorder.empty())
			return false;
		return preorder[0]>min&&preorder[0]<max;
	}
	BinarySearchTree(deque<int> &preorder,int min=0,int max=1000 ) {
		int data=preorder[0];
		preorder.pop_front();

		if(next_between(preorder,data,max)) // there is a left
			left=new BinarySearchTree(preorder,data,max);
			
		if(next_between(preorder,min,data)) // there is a right
			right=new BinarySearchTree(preorder,min,data);
	}


};



int main() {


	cout << "bye\n";

	return 0;
}
