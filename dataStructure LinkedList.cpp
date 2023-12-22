#include <iostream>
#include <cassert>
#include <climits>
#include<cmath>
#include <vector>		// for debug
#include <algorithm>
#include <sstream>
using namespace std;

struct Node {
	int data { };
	Node* next { };
	Node(int data) : data(data) {}
	~Node() {
		cout << "Destroy value: " << data <<" at address "<< this<< "\n";
	}
};

class LinkedList {
private:
	Node *head { };
	Node *tail { };
	int length = 0;	// let's maintain how many nodes

	vector<Node*> debug_data;	// add/remove nodes you use

	void debug_add_node(Node* node) {
		debug_data.push_back(node);
	}
	void debug_remove_node(Node* node)
	{
		auto it = std::find(debug_data.begin(), debug_data.end(), node);
		if (it == debug_data.end())
			cout << "Node does not exist\n";
		else
			debug_data.erase(it);
	}

public:

	// Below 2 deletes prevent copy and assign to avoid this mistake
	LinkedList() {
	}
	LinkedList(const LinkedList&) = delete;
	LinkedList &operator=(const LinkedList &another) = delete;

	void debug_print_address() {
		for (Node* cur = head; cur; cur = cur->next)
			cout << cur << "," << cur->data << "\t";
		cout << "\n";
	}

	void debug_print_node(Node* node, bool is_seperate = false) {
		if (is_seperate)
			cout << "Sep: ";
		if (node == nullptr) {
			cout << "nullptr\n";
			return;
		}
		cout << node->data << " ";
		if (node->next == nullptr)
			cout << "X ";
		else
			cout << node->next->data << " ";

		if (node == head)
			cout << "head\n";
		else if (node == tail)
			cout << "tail\n";
		else
			cout << "\n";
	}
	void debug_print_list(string msg = "") {
		if (msg != "")
			cout << msg << "\n";
		for (int i = 0; i < (int) debug_data.size(); ++i)
			debug_print_node(debug_data[i]);
		cout << "************\n"<<flush;
	}
	string debug_to_string() {
		if (length == 0)
			return "";
		ostringstream oss;
		for (Node* cur = head; cur; cur = cur->next) {
			oss << cur->data;
			if (cur->next)
				oss << " ";
		}
		return oss.str();
	}

	void debug_verify_data_integrity() {
		if (length == 0) {
			assert(head == nullptr);
			assert(tail == nullptr);
		} else {
			assert(head != nullptr);
			assert(tail != nullptr);
			if (length == 1)
				assert(head == tail);
			else
				assert(head != tail);
			assert(!tail->next);
		}
		int len = 0;
		for (Node* cur = head; cur; cur = cur->next, len++)
			assert(len < 10000);	// Consider infinite cycle?
		assert(length == len);
		assert(length == (int)debug_data.size());
	}

	////////////////////////////////////////////////////////////

	void print() {
		for (Node* cur = head; cur; cur = cur->next)
			cout << cur->data << " ";
		cout << "\n";
	}

	// These 2 simple functions just to not forget changing the vector and length
	void delete_node(Node* node) {
		debug_remove_node(node);
		--length;
		delete node;
	}

	void add_node(Node* node) {
		debug_add_node(node);
		++length;
	}

	void insert_end(int value) {
		Node* item = new Node(value);
		add_node(item);

		if (!head)
			head = tail = item;
		else
			tail->next = item, tail = item;
	}

	void insert_front(int value) {
		Node* item = new Node(value);
		add_node(item);

		item->next = head;
		head = item;

		if(length == 1)
			tail = head;
	}
	void delete_front() {
		assert(length);
		Node* cur = head->next;
		delete_node(head);
		head = cur;
	}
	Node* get_previous(Node* target) // O(n) time - O(1) memory
	{
		for (Node *cur = head, *prv = nullptr; cur; prv = cur, cur = cur->next)
		{
			if (cur == target)	// memory wise
				return prv;
		}
		return nullptr;	// still more steps needed - NOT found
	}
	////////////////////////////////////////////////////////////
	Node* get_nth(int n) {		// O(n) time - O(1) memory
		int cnt = 0;
		for (Node* cur = head; cur; cur = cur->next)
			if (++cnt == n)
				return cur;

		return nullptr;	// still more steps needed - NOT found
	}
	Node* get_nth_back(int n)
	{
		int cnt=1;
		int pos=length-n+1;
		for(Node* cur=head; cur ;cur=cur->next)
		{
			if(cnt++==pos)
				return cur;
		}
		return nullptr;
	}
	bool is_same(const LinkedList &list2)
	{
		if(length!=list2.length)
			return false;
		Node* another_cur = list2.head;

		for(Node *cur=head; cur ; cur=cur->next)
		{
			if(cur->data!= another_cur->data)
				return false;
			another_cur=another_cur->next;
		}
		return true;
	}
	bool is_same1(const LinkedList& list2)
	{
		Node* h1=head,*h2=list2.head;
		while(h1&&h2)
		{
			if(h1->data!=h2->data)
				return false;
			h1=h1->next, h2=h2->next;
		}
		return !h1 && !h2; //make sure both ends together
	}
	void delete_first()
	{
		if(head)
		{
			Node* cur= head;
			head=head->next;
			delete_node(cur);
			if(!head)
				tail=nullptr;
			debug_verify_data_integrity();
		}
	}
	void delete_end()
	{
		for(Node* cur=head ; cur ; cur=cur->next)
		{
			if(cur->next==tail)
			{
				Node* last=cur;
				delete_node(tail);
				tail=last;
			}
		}
	}
	void delete_last()
	{
		if (length <= 1) {
			delete_first();
			return;
		}
		// Get the node before tail: its order is length-1 node
		Node* previous = get_nth(length - 1);

		delete_node(tail);
		tail = previous;
		tail->next = nullptr;

		debug_verify_data_integrity();
	}
	void delete_nth_node(int n) {
		if (n < 1 || n > length)
			cout << "Error. No such nth node\n";
		else if (n == 1)
			delete_first();
		else {
			// Connect the node before nth with node after nth
			Node* before_nth = get_nth(n - 1);
			Node* nth = before_nth->next;
			bool is_tail = nth == tail; //check wheater it is the last node
			// connect before node with after
			before_nth->next = nth->next;
			if (is_tail)
				tail = before_nth;

			delete_node(nth);

			debug_verify_data_integrity();
		}
	}

	void delete_next_node(Node* node)
	{
		//deletes the node and connect previous to next
		Node* to_delete = node->next;
		bool is_tail = to_delete == tail;
		// node->next in middle to delete
		node->next = node->next->next;
		delete_node(to_delete);

		if(is_tail)
			tail = node;
	}

	void delete_node_with_key(int value) // O(n) time - O(1) memory
	{
		if (!length)
			cout << "Empty list!\n";
		else if (head->data == value)
			delete_first();
		else
		{
			for (Node *cur = head, *prv = nullptr; cur; prv = cur, cur = cur->next)
			{
				if(cur->data == value)
				{
					delete_next_node(prv);	// connect prv with after
					return;
				}
			}
			cout<<"Value not found!\n";
		}
		debug_verify_data_integrity();
	}
	void swap_pairs()
	{
		for(Node* cur=head;cur;cur=cur->next->next)
		{
			if(cur->next)
				swap(cur->data,cur->next->data);
		}
	}
	void reverse()
	{
		if(length==1)
			return;
		else
		{
			tail=head;
			Node* prev=head;
			head=head->next;

			while(head)
			{
				//store next and reserve link
				Node* next=head->next;
				head->next=prev;

				//move a step forward
				prev=head;
				head=next;
			}
			head=prev;
			tail->next=nullptr;

			debug_verify_data_integrity();
		}
	}
	void delete_even_positions() // O(n) time - O(1) memory
	{		
		if (length <= 1)
			return;
		// maintain previous and delete its next (even order)
		for(Node *cur = head->next, *prv = head;cur;) 
		{
			delete_next_node(prv);	// prev is odd, prev-next is even
			if (!prv->next)	// tail
				break;
			cur = prv->next->next;
			prv = prv->next;
		}
		debug_verify_data_integrity();
	}

	void embed_after(Node* node, int value)
    {
		// Add a node with value between node and its next
		Node* item = new Node(value);
		++length;
		debug_add_node(item);

		item->next = node->next;
		node->next = item;
	}

	void insert_sorted(int value) {		// O(n) time - O(1) memory
		// 3 special cases for simplicity
		if (!length || value <= head->data)
			insert_front(value);
		else if (value >= tail->data)
			insert_end(value);
		else {
			// Find the node I am less than. Then I am before it
			for (Node *cur = head, *prv = nullptr; cur; prv = cur, cur = cur->next)
			{
				if (cur->data > value ) //once you find the node that has a data bigger than the value
				{						// insert it before it directly(after the prv node).
					embed_after(prv, value);
					break;
				}
			}
		}
		debug_verify_data_integrity();

		// This idea is used in Insertion Sort Algorithm
	}
	void Swap_head() //O(n) time - O(1) memory
	{
		if (!head || !head->next)
			return;

		if (length == 2) {	// Fix
			swap(tail, head); //tail will look to head and head will look at tail
			head->next = tail;
			tail->next = nullptr;
			return;
		}

		Node* temp1=head->next;
		Node* temp2= get_previous(tail);

		tail->next=temp1;
		temp2->next=head;
		head->next=nullptr;
		//swap head and tail
		Node*temp3=head;
		head=tail;
		tail=temp3;
	}
	void left_rotate(int k) 	// O(n) time - O(1) memory
	{
		if(length <= 1 || k % length == 0)
			return;
		k %= length;	// Remove useless cycles

		Node* nth = get_nth(k);
		tail->next = head;		// create cycle

		// Reset tail/head
		tail = nth;
		head = nth->next;

		tail->next = nullptr;	// disconnect cycle
		debug_verify_data_integrity();
	}
	void remove_duplicates()
	{
		for(Node* cur = head; cur; cur = cur->next)
		{
			for(Node* cur2 = cur->next, *prev = cur; cur2;)
			{
				if(cur->data == cur2->data)
				{
					delete_next_node(prev);
					cur2 = prev->next;
				}
				else
				{
					prev = cur2;
					cur2 = cur2->next;
				}
			}
		}
		debug_verify_data_integrity();
	}
	void delete_last_occurrence(int target) // O(n) time - O(1) memory
	{
		if (!length)
			return;

		Node* delete_my_next_node = nullptr;
		bool is_found = false;

		// Find the last one and remove it
		Node* prv=nullptr;
		for (Node *cur = head; cur; prv = cur, cur = cur->next)
		{
			if (cur->data == target)
				is_found = true;
		}

		if (is_found)
		{
			if (delete_my_next_node)
				delete_next_node(prv);
			else
				// if prv is null, then found at head
				delete_front();
		}
		debug_verify_data_integrity();
	}

	Node* move_to_end(Node* cur, Node* prv)
	{
		if (cur == tail)	// Fix
			return cur;	// already at the end(doesn't matter wheater to return cur or nullptr because if the loop reached to the tail this means it is the last iteration)

		Node* next = cur->next;
		tail->next = cur;

		if (prv)
			prv->next = next;
		else
			head = next;	// cur was head (prv will still nullptr)
		//update tail
		tail = cur;
		tail->next = nullptr;
		return next;
	}

	void move_key_occurance_to_end(int key) // O(n) time - O(1) memory
	{
		if (length <= 1)
			return;

		// Simple idea with tail: for each matching key, remove it and add to the end!

		int len = length;	// iterate length steps, don't go infinte with added ones
		for (Node* cur = head, *prv = nullptr; len--;)
		{
			if (cur->data == key)	// no change for prv
				cur = move_to_end(cur, prv);
			else
				prv = cur, cur = cur->next;	// normal step
		}
		debug_verify_data_integrity();
	}
	int max(Node* head = nullptr, bool is_first_call = true) {		// O(n) time - O(n) memory
		// It is tricky to have function name same as standard
		// use std for standard and this-> for the class for clarity
		if (is_first_call)
			return this->max(this->head, false);

		if (head == nullptr) // means that we ended the the linked list
			return INT_MIN;		// initial min value
		return std::max(head->data, this->max(head->next, false));
	}
	void odd_pos_even_pos() // O(n) time - O(1) memory
	{		
        int len = length / 2;
        for (Node* cur = head->next, *prv = head;len--;) 
        {
            cur = move_to_end(cur,prv);
            prv = cur;
            cur = cur->next;
        }
		 debug_verify_data_integrity();
    }
	
	void insert_after(Node *src, Node* target) 
	{
		// Given node src, link target after it with my list before/after
		assert(src && target);
		target->next = src->next;
		src->next = target;
		debug_add_node(target);
		++length;
	}
	void insert_alternate(LinkedList &another) // O(n) time - O(1) memory
	{		
		if (!another.length)
			return;

		if (!length) {
			// copy data
			head = another.head;
			tail = another.tail;
			length = another.length;
			debug_data = another.debug_data;
		}
		else 
		{
			// Iterate one by one, add node in right place
			// If this ended first, then we link the remain in this list
			Node* cur2 = another.head;
			for (Node* cur1 = head; cur1 && cur2;) 
			{
				Node* cur2_next_temp = cur2->next;
				insert_after(cur1, cur2);
				another.length--;
				cur2 = cur2_next_temp;

				if (cur1 == tail)// means that cur1 ended first
				{
					tail = another.tail;
					cur1->next->next = cur2; // let last node of this look at the next node of another
					length += another.length;
					break;
					// there is a missing thing here
					// we need to add remaining nodes to debug data
					// let's skip
				}
				cur1 = cur1->next->next;// cur1->next (target) target->next
			}
		}
		//clear the data of another linkedlist
		another.head = another.tail = nullptr;
		another.length = 0;
		another.debug_data.clear();
		debug_verify_data_integrity();
	}
	void add_num(LinkedList &another) {
		// let X = max(length, another.length)
		// let Y = max(length, another.length) - min(length, another.length)
		// O(X) time - O(Y) memory
		if (!another.length)
			return;

		Node* my_cur = head;
		Node* his_cur = another.head;
		int carry = 0, my_value, his_value;
		
		// refresh first adding 2 numbers and handling their carry
		// Iterate sequentially over both
		// take the current values, add and compute the value and the carry

		while (my_cur || his_cur) 
		{
			my_value = 0, his_value = 0;
			if (my_cur)
				my_value = my_cur->data;
			if (his_cur) 
			{
				his_value = his_cur->data;
				his_cur = his_cur->next;
			}

			my_value += his_value + carry;
			carry = my_value / 10;
			my_value %= 10;

			if(my_cur) 
			{
				my_cur->data = my_value;
				my_cur = my_cur->next;
			}
			else
				insert_end(my_value);
			// we can even stop earlier
		}
		if(carry)
			insert_end(carry);

		debug_verify_data_integrity();
	}
	Node* move_and_delete(Node* node) {
		Node* temp = node->next;
		delete_node(node);
		return temp;
	}

	void remove_all_repeated_from_sorted() {	// O(n) time - O(1) memory
		if (length <= 1)
			return;

		// Add dummy head for easier prv linking
		insert_front(-1234);

		tail = head;
		Node *previous = head;
		Node *cur = head->next;	// Our actual head

		while (cur) 
		{
			// 2 cases: Either blocks of repeated so remove it. Or single node, keep it

			// keep removing blocks of SAME value
			bool any_removed = false;
			while (cur && cur->next && cur->data == cur->next->data) 
			{
				int block_value = cur->data;
				any_removed = true;
				while (cur && cur->data == block_value)
					cur = move_and_delete(cur);
			}
			if (any_removed)  
			{
				if (!cur)
					tail = previous;
				previous->next = cur;	// link after the removed nodes
				previous = cur;
			} 
			else {
				// No duplicates. Connect and use as tail for now
				tail = cur;
				previous = cur;
				cur = cur->next;
			}
		}
		previous = head->next;	// actual head
		delete_front();		// remove dummy head
		head = previous;
		if (!head)
			tail = head;
		debug_verify_data_integrity();
	}
	pair<Node*, pair<Node*, Node*>> reverse_subchain(Node* cur_head, int k) // O(n) time - O(1) memory
	{	
		// Given a node: reverse only a chain of k nodes
		// return: current chain head, current chain tail, next head
		// Logic same as normal reverse function

		Node* cur_tail = cur_head;	// the head is tail once reversed
		Node* prv = cur_head;
		cur_head = cur_head->next;	// Start from next

		for (int s = 1; s < k && cur_head; ++s) {	//k links reversed
			// store & reverse link
			Node* next = cur_head->next;
			cur_head->next = prv;

			// move step
			prv = cur_head;
			cur_head = next;
		}
		return make_pair(prv, make_pair(cur_tail, cur_head));	// C++17 can use tie/tuple
	}
	void reverse_chains(int k) 
	{
		// Reverse each K consecutive nodes
		if (length <= 1 || k == 1)
			return;

		Node* last_tail = nullptr;
		Node* next_chain_head = head;
		head = nullptr;

		while(next_chain_head) 
		{
			// Keep reversing a chain of K nodes, and link with prv chain
			pair<Node*, pair<Node*, Node*>> p = reverse_subchain(next_chain_head, k);
			Node* chain_head = p.first;
			Node* chain_tail = p.second.first;
			next_chain_head = p.second.second;
			tail = chain_tail;

			if(!head)	// first chain
				head = chain_head;
			else	// connect last chain tail with next chain head
				last_tail->next = chain_head;
			last_tail = chain_tail; // if this is the first chain then last_tail=chain_tail
		}
		tail->next = nullptr;
		debug_verify_data_integrity();
	}
};
void test1() {
	cout << "\n\ntest1\n";
	LinkedList list;

	list.insert_end(1);
	assert(list.get_nth_back(1)->data == 1);
	list.insert_end(2);
	list.insert_end(3);
	assert(list.get_nth_back(1)->data == 3);
	assert(list.get_nth_back(3)->data == 1);
}
int main() {
	LinkedList list1;
	list1.insert_end(1);
	list1.insert_end(2);
	list1.insert_end(3);
	list1.insert_end(4);
	list1.insert_end(5);
	list1.insert_end(6);
	list1.insert_end(7);
	list1.reverse_chains(3);
	list1.print();

	return 0;
}
