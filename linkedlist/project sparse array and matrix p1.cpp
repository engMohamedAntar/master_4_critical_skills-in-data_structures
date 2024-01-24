#include<iostream> 
using namespace std;
struct ArrayNode
{
    int data {};
    int index {};
    ArrayNode *next{};
    ArrayNode *prev{};
    ArrayNode(int data,int index):data(data),index(index)
    {}
};
class ArrayLinkedList 
{
    private:
    ArrayNode *head{};
    ArrayNode *tail{};
    int length= 0;
    int array_length { };
    
    void link(ArrayNode* first,ArrayNode* second)
    {
        if(first)
            first->next=second;
        if(second)
            second->prev=first;
    }
    ArrayNode* embed_after(ArrayNode* node_before, int data, int index)
    {
        length++; 
        ArrayNode* middle=new ArrayNode(data,index);
        ArrayNode* node_after=node_before->next;
        
        link(node_before,middle);
        if(!node_after)
            tail=middle;
        else
            link(middle,node_after);
        return middle;
    }
    ArrayNode* get_index(int index, bool is_create_if_missing)
    {
        ArrayNode* prev_index=head;
        //Find the node with index-1
        while(prev_index->next&& prev_index->next->index < index)// this loop will end in two cases
            prev_index= prev_index->next;                        // 1- prev_index->next =nullptr;   
                                                                 // 2- if prev_index->next > index
        bool found= prev_index->next && prev_index->next->index==index;
        
        if(found)
            return prev_index->next;
        if(!is_create_if_missing)
            return nullptr;
        return embed_after(prev_index,0,index); // this would be executed if not found
    }
    public:
    int get_value(int index)
    {
        ArrayNode *node = get_index(index,false);
        if(!node)
            return 0;
        else
            return node->data;
    }
    void set_value(int value,int index)
    {
        get_index(index,true)->data=value; // get_index returns a node
    }
    void print_array_nonzero()
    {
        for(ArrayNode* cur=head;cur;cur=cur->next)
            cout<<cur->data<<" ";
        cout<<endl;
    }
    void print_array()
    {
        ArrayNode* cur=head->next;
        for(int c=0;c<array_length;c++)
        {
            if(cur && cur->index==c)
            {
                cout<<cur->data;
                cur=cur->next;
            }
            else
                cout<<"0 ";
        }
        cout<<endl;
    }

	void add(ArrayLinkedList &other)
	{
		for(ArrayNode* other_cur= other.head->next; other_cur ; other_cur=other_cur->next)
        {
            ArrayNode* this_index= get_index(other_cur->index, true);// if found then it will be returned,
            this_index->data += other_cur->data;                       // if not a new node will be created            
        }
	}
};

int main()
{
    cout<<"Antar nouh\n";
    return 0;
}