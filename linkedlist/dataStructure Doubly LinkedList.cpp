#include <iostream>
#include <cassert>
#include <climits>

#include <vector>		// for debug
#include <algorithm>
#include <sstream>
using namespace std;

struct Node {
	int data { };
	Node* next { };
	Node* prev { };	 // Previous node!

	Node(int data) :
			data(data) {
	}

	void set(Node* next, Node* prev) {
		this->next = next;
		this->prev = prev;
	}

	~Node() {
		cout << "Destroy value: " << data << " at address " << this << "\n";
	}
};

class LinkedList {
private:
	Node *head { };
	Node *tail { };
	int length = 0;
	// let's maintain how many nodes

	vector<Node*> debug_data;	// add/remove nodes you use

	void debug_add_node(Node* node) {
		debug_data.push_back(node);
	}
	void debug_remove_node(Node* node) {
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

		if (node->prev == nullptr)
			cout << "X\t";
		else
			cout << node->prev->data << "\t";

		cout << " <= [" << node->data << "]\t => \t";

		if (node->next == nullptr)
			cout << "X\t";
		else
			cout << node->next->data << "\t";

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
		cout << "************\n" << flush;
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
			assert(!head->prev);
			assert(!tail->next);
		}
		int len = 0;
		for (Node* cur = head; cur; cur = cur->next, len++) {
			if (len == length-1)	// make sure we end at tail
				assert(cur == tail);
		}

		assert(length == len);
		assert(length == (int )debug_data.size());

		len = 0;
		for (Node* cur = tail; cur; cur = cur->prev, len++) {
			if (len == length-1)	// make sure we end at head
				assert(cur == head);
		}
		//cout << "\n";
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

	void link(Node* first, Node*second) {
		if (first)
			first->next = second;
		if (second)
			second->prev = first;
	}

	void insert_end(int value) {
		Node* item = new Node(value);
		add_node(item);

		if (!head)
			head = tail = item;
		else {
			link(tail, item);
			tail = item;
		}
		debug_verify_data_integrity();
	}

	void insert_front(int value) {
		Node* item = new Node(value);
		add_node(item);

		if (!head)
			head = tail = item;
		else {
			link(item, head);
			head = item;
		}
		debug_verify_data_integrity();
	}

	void print_reversed() {
		for (Node* cur = tail; cur; cur = cur->prev)
			cout << cur->data << " ";
		cout << "\n";
	}

	void delete_front() {
		if (!head)
			return;
		Node* cur = head->next;
		delete_node(head);
		head = cur;

		// Integrity change
		if (head)
			head->prev = nullptr;
		else if (!length)
			tail = nullptr;

		debug_verify_data_integrity();
	}

	void delete_end() {
		if (!head)
			return;
		Node* cur = tail->prev;
		delete_node(tail);
		tail = cur;

		// Integrity change
		if (tail)
			tail->next = nullptr;
		else if (!length)
			head = nullptr;

		debug_verify_data_integrity();
	}

	Node* delete_and_link(Node* cur) {
		// remove this node, but connect its neighbors
		Node* ret = cur->prev;
		link(cur->prev, cur->next);
		delete_node(cur);

		return ret;
	}

	void delete_node_with_key(int value) {	// O(n) time - O(1) memory
		if (!length)
			cout << "Empty list!\n";
		else if (head->data == value)
			delete_front();
		else if (tail->data == value)
			delete_end();
		else {
			for (Node *cur = head; cur; cur = cur->next) {
				if (cur->data == value) {
					delete_and_link(cur);
					return;
				}
			}
			cout << "Value not found!\n";
		}
		debug_verify_data_integrity();
	}

	void embed_after(Node* node_before, int value) {
		// Add a node with value between node and its next
		Node* middle = new Node(value);
		++length;
		debug_add_node(middle);

		Node* node_after = node_before->next;
		link(node_before, middle);
		link(middle, node_after);
	}

	void insert_sorted(int value) {		// O(n) time - O(1) memory
		// 3 special cases for simplicity
		if (!length || value <= head->data)
			insert_front(value);
		else if (tail->data <= value)
			insert_end(value);
		else {
			// Find the node I am less than. Then I am before it
			for (Node *cur = head; cur; cur = cur->next) {
				if (value <= cur->data) {
					embed_after(cur->prev, value);
					break;
				}
			}
		}
		debug_verify_data_integrity();

		// This idea is used in Insertion Sort Algorithm
	}
	//////////////////////////////////////////
	void delete_all_nodes_with_key(int value) {
		if (!length)
			return;

		insert_front(-value);	// insert dummy. assume -value no overflow

		for (Node *cur = head; cur;) {
			if (cur->data == value) 
			{
				cur = delete_and_link(cur);
				if(!cur->next)
					tail = cur;
			}
			else
				cur = cur->next;
		}

		delete_front();

		debug_verify_data_integrity();
	}
	void delete_even_positions() 
	{
		if (length <= 1)
			return;
		//cur is always odd and ->next is even
		for(Node *cur = head; cur && cur->next; cur = cur->next) 
		{
			delete_and_link(cur->next);
			if (!cur->next)
				tail = cur;	// in even length, the tail will change
		}
		debug_verify_data_integrity();
	}
	void delete_odd_positions() 
	{
		insert_front(-12); 		// insert a dumpy node
		delete_even_positions();
		delete_front();
	}
	bool is_palindrome()
	{
		if(length<=1)
			return true;

		Node* temp1=head;
		Node* temp2=tail;
		int len=length/2;
		while(len--)
		{
			if(temp1->data != temp2->data)
				return false;
			temp1=temp1->next;
			temp2=temp2->prev;
		}
		return true;
	}
	int middle_value1() {	// iterate from both directions same time
		assert(head);

		Node *h = head, *t = tail;

		while (h != t && h->next != t)
			h = h->next, t = t->prev;
		return t->data;
	}
	int middle_value2()
	{
		assert(head);
		Node* slow=head,*fast=head;
		while( fast && fast->next) // why is fast important
		{
			slow=slow->next;
			fast=fast->next->next;
		}
		return slow->data;
	}
	
	Node* get_nth(int n) {
		int cnt = 0;
		for (Node* cur = head; cur; cur = cur->next)
			if (++cnt == n)
				return cur;

		return nullptr;	// still more steps needed - NOT found
	}
	void merge_2sorted_lists(LinkedList &other) // O(n+m)
	{	
		if (!other.head)
			return;

		if (head) 
		{
			Node* cur1 = head;
			Node* cur2 = other.head;
			Node* last=nullptr;
			head = nullptr;

			while (cur1 && cur2) 
			{
				Node* next { };
				// pick the smallest among the 2 lists and then link it
				if (cur1->data <= cur2->data) 
				{
					next = cur1;
					cur1 = cur1->next;
				} 
				else 
				{
					next = cur2;
					cur2 = cur2->next;
				}
				link(last, next);
				last = next;
				if (!head)	// first step is the head
					head = last;
			}
			if (cur2) // our tail is from 2nd
			{	
				tail = other.tail;
				link(last, cur2);
			} 
			else if (cur1) 
				link(last, cur1);
			
		} 
		else 
		{	// use its data
			head = other.head;
			tail = other.tail;
		}

		length += other.length;
		debug_data.insert(debug_data.end(), other.debug_data.begin(), other.debug_data.end());	// insert the node of other.debug_data at the end of the debug_data vector
		other.head = other.tail = nullptr;
		other.length = 0;
		other.debug_data.clear();

		debug_verify_data_integrity();
	}
};	

int main() {
	 LinkedList list1;
	list1.insert_end(1);
	list1.insert_end(2);
	list1.insert_end(3);
	LinkedList list2;
	list2.insert_end(4);
	list2.insert_end(5);
	list2.insert_end(6);
	list1.merge_2sorted_lists(list2);
	list1.print(); // 1 2 3 4 5 6
	cout<<"Antar\n";
	return 0;
}

