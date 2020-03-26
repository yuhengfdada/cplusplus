#include <iostream>
#include "intermediate.h"
#include "table.h"
using namespace std;

/*
 struct EntryNode {
	string *entry; //ָ��һ��string���飨table�е�һ��entry��

	EntryNode *prev;
	EntryNode *next;
};

private:
	// Pointer to the 1D array of attributes
	// Not owned by intermediate
	const string *attrs;
	int numAttrs;

	// Doubly linked list
	EntryNode *head;
	EntryNode *tail;
 */

/*
//�ҷ��ˣ����ܷ���private member����ʱ���ٸİɣ�������
//�ú���������"intermediate"��deleteһ��EntryNode,��ʹpointer"entry"��Զָ��ɾ��node����һ��node.
void delete_node(Intermediate& intermediate, EntryNode*& current_node){
	if(current_node==this->head){
		EntryNode* next = current_node->next;
		this->head=next;
		delete current_node;
		if(next){
			next->prev=nullptr;
			current_node = next;
		}
		//ɾ����
		else this->tail=nullptr;
	}
	else if(current_node==this->tail){ //�����ǲ������ɾ�յ������
		EntryNode* prev = current_node->prev;
		this->tail=prev;
		delete current_node;
		prev->next=nullptr;
		current_node=nullptr;
	}
	else{
		EntryNode* prev = current_node->prev;
		EntryNode* next = current_node->next;
		prev->next=next;
		next->prev=prev;
		delete current_node;
		current_node=next;
	}

}
*/

//����array[i][attr_index]�������string������ѡ������
/*
void simple_sort(string* array[],int attr_index,enum order order,int array_size){
	if (order==ASCENDING){
		string min_string; //ÿһ����С��string
		int min_index; //min���ڵ�index
		for(int i=0;i<array_size;++i){
			min_string = array[i][attr_index];
			min_index = i;
			for(int j=i;j<array_size;++j){
				if(array[j][attr_index].compare(min_string)<0) {
					min_string = array[j][attr_index];
					min_index = j;
				}
			}
			if(min_index!=i){
				string* temp = array[min_index];
				array[min_index] = array[i];
				array[i] = temp;
			}
		}
	}
	if (order==DESCENDING){
		string max_string; //ÿһ������string
		int max_index; //max���ڵ�index
		for(int i=0;i<array_size;++i){
			max_string = array[i][attr_index];
			max_index = i;
			for(int j=i;j<array_size;++j){
				if(array[j][attr_index].compare(max_string)>0) {
					max_string = array[j][attr_index];
					max_index = j;
				}
				//begin debug
				if(array[j][2]=="Design and Analysis of Algorithms"){
					cout << j << endl;
				}
				//end debug

			}
			if(max_index!=i){
				string* temp = array[max_index];
				array[max_index] = array[i];
				array[i] = temp;
			}
		}
	}
}
*/
void simple_sort(string* array[],int attr_index,enum order order,int array_size){
	if(order==ASCENDING){
		for(int i=0;i<array_size;i++){//��i��
			for(int j=0;j<array_size-1;j++){//��j��Ԫ��
				if(array[j][attr_index].compare(array[j+1][attr_index])>0){
					string* temp=array[j];
					array[j]=array[j+1];
					array[j+1]=temp;
				}
			}
		}
	}
	if(order==DESCENDING){
		for(int i=0;i<array_size;i++){//��i��
			for(int j=0;j<array_size-1;j++){//��j��Ԫ��
				if(array[j][attr_index].compare(array[j+1][attr_index])<0){
					string* temp=array[j];
					array[j]=array[j+1];
					array[j+1]=temp;
				}
			}
		}
	}
}
//string����"row"������һ�е�����,max_len
void print_row(const string* row,int max_len[],int numAttrs){
	for(int i=0;i<numAttrs-1;++i){
		cout << _left_pad_until(row[i],max_len[i]) << " | ";
	}
	cout << _left_pad_until(row[numAttrs-1],max_len[numAttrs-1]) << endl;
}


Intermediate::Intermediate(const Table &table){
	//�ȸ㶨ǰ����
	attrs = table.attrs;
	numAttrs = table.numAttrs;
	//���������table�ǿյ�
	if(!table.entries){
		head = nullptr;
		tail = nullptr;
	}
	else{
		head = new EntryNode;
		head->entry = table.entries[0];
		head->prev = nullptr;
		EntryNode* last_pointer = head;
		for(int i=1;i<table.numEntries;++i){
			EntryNode* temp_pointer = new EntryNode;
			temp_pointer->entry = table.entries[i];
			temp_pointer->prev = last_pointer;
			last_pointer->next = temp_pointer;
			last_pointer = temp_pointer;
			//tail
			if(i==table.numEntries-1){
				tail = temp_pointer;
				tail->next = nullptr;
			}
		}
	}
}


Intermediate::~Intermediate(){
	//deallocate all memory held by the linked list
	if(head){
		EntryNode* current = head;
		while(current){
			EntryNode* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;
		tail = nullptr;
	}

}


Intermediate& Intermediate::where(const string &attr, enum compare mode, const string &value){
	//����������ձ�
	if(!head) return *this;
	//�ȼ������������attr�Ƿ���ڣ�����ֻ��һ����
	bool flag = false;
	int attr_index; //����������attr index
	for(int i=0;i<numAttrs;++i){
		if(attrs[i]==attr) {
			flag=true;
			attr_index = i;
			break;
		}
	}
	if(!flag) return *this;
	//mode��equal to
	if(mode==EQ){
		//�������attr���Ͱ�"attr"��ֵ������"value"��entry��ɾ��
		//����һ��
		EntryNode* current_node = head;
		while(current_node){
			if(current_node->entry[attr_index]!=value){
				//delete_node�������Զ���current_node���ƣ�����һ�仰����
				//begin function
				if(current_node==head){
					EntryNode* next = current_node->next;
					head=next;
					delete current_node;
					if(next){
						next->prev=nullptr;
						current_node = next;
					}
					//ɾ����
					else {tail=nullptr;
					current_node = next;}
				}
				else if(current_node==tail){ //�����ǲ������ɾ�յ������
					EntryNode* prev = current_node->prev;
					tail=prev;
					delete current_node;
					prev->next=nullptr;
					current_node=nullptr;
				}
				else{
					EntryNode* prev = current_node->prev;
					EntryNode* next = current_node->next;
					prev->next=next;
					next->prev=prev;
					delete current_node;
					current_node=next;
				}
				//end function
			}
			else current_node = current_node->next;
		}
		return *this;
	}
	if(mode==CONTAINS){
		//�������attr���Ͱ�"attr"��ֵ������"value"��entry��ɾ��
		//����һ��
		EntryNode* current_node = head;
		while(current_node){
			string value_to_examine = current_node->entry[attr_index];
			string::size_type position;
			position = value_to_examine.find(value);
			if(position==value_to_examine.npos){
				//delete_node�������Զ���current_node���ƣ�����һ�仰����
				//begin function
				if(current_node==head){
					EntryNode* next = current_node->next;
					head=next;
					delete current_node;
					if(next){
						next->prev=nullptr;
						current_node = next;
					}
					//ɾ����
					else {tail=nullptr;
					current_node = next;}
				}
				else if(current_node==tail){ //�����ǲ������ɾ�յ������
					EntryNode* prev = current_node->prev;
					tail=prev;
					delete current_node;
					prev->next=nullptr;
					current_node=nullptr;
				}
				else{
					EntryNode* prev = current_node->prev;
					EntryNode* next = current_node->next;
					prev->next=next;
					next->prev=prev;
					delete current_node;
					current_node=next;
				}
				//end function
			}
			else current_node = current_node->next;
		}
		return *this;

	}
	return *this;//��ʵû��Ҫ��ֻ�ǲ��뿴��warning
}

//˼·��1.����һ�飬������Ӧ��string���� 2.����string���� 3.����һ�飬��ֵ����ȥ
Intermediate& Intermediate::orderBy(const string &attr, enum order order){
	//����������ձ�
	if(!head) return *this;
	//�ȼ������������attr�Ƿ���ڣ�����ֻ��һ����
	bool flag = false;
	int attr_index; //����������attr index
	for(int i=0;i<numAttrs;++i){
		if(attrs[i]==attr) {
			flag=true;
			attr_index = i;
			break;
		}
	}
	if(!flag) return *this;
	//����entry��Ŀ���Ա㴴������
	int numEntries = 0;
	EntryNode* current_node = head;
	while(current_node){
		numEntries++;
		current_node = current_node->next;
	}
	//����string��Ӧ��array,���ҽ�δ�����linked list�����ȥ
	string* array[numEntries];
	current_node = head;
	int i=0;
	while(current_node){
		array[i] = current_node->entry;
		current_node = current_node->next;
		++i;
	}

	//sort array

	simple_sort(array,attr_index,order,numEntries);

	//��array��ֵ����ȥ
	i=0;
	current_node = head;
	while(current_node){
		current_node->entry = array[i];
		current_node = current_node->next;
		++i;
	}
	return *this;
}


Intermediate& Intermediate::limit(unsigned int limit){
	if(!head) return *this; //��intermediate
	//�������1:limit==0��дһ��destructor������
	if(limit==0){
		if(head){
			EntryNode* current = head;
			while(current){
				EntryNode* next = current->next;
				delete current;
				current = next;
			}
			head = nullptr;
			tail = nullptr;
		}
		return *this;
	}
	//�������2:limit>=numEntries��return *this�ͺ���
	unsigned int numEntries = 0;
	EntryNode* current_node = head;
	while(current_node){
		numEntries++;
		current_node = current_node->next;
	}
	if(limit>=numEntries){
		return *this;
	}
	//һ�����
	current_node = head;
	for(unsigned int count=1;count<limit;++count){//��λ��Ҫ���е���node�ġ�ǰһ����node
		current_node = current_node->next;
	}
	tail = current_node;
	EntryNode* temp_node = current_node->next;//��һ�����е���node
	current_node->next = nullptr;
	current_node = temp_node;
	//��current_nodeΪhead,���ñ��ε�destructorɾ����������
	while(current_node){
		EntryNode* next = current_node->next;
		delete current_node;
		current_node = next;
	}
	return *this;
}


void Intermediate::update(const string &attr, const string &new_value) const{
	//����������ձ�
	if(!head) return;
	//�ȼ������������attr�Ƿ���ڣ�����ֻ��һ����
	bool flag = false;
	int attr_index; //����������attr index
	for(int i=0;i<numAttrs;++i){
		if(attrs[i]==attr) flag=true;
		attr_index = i;
	}
	if(!flag) return;
	//������ڣ���������and update�ֶ�"attr"
	EntryNode* current_node = head;
	while(current_node){
		current_node->entry[attr_index] = new_value;
		current_node = current_node->next;
	}
}


void Intermediate::select(const string *attrs, int numAttrs) const{
//û��this->attrs, do nothing
	if(!this->attrs) return;
//attrs==nullptr, print all and return
	if(!attrs){
		numAttrs = this->numAttrs;
		int max_length[numAttrs];
		for(int i=0;i<numAttrs;++i){
			max_length[i] = this->attrs[i].length();
		}
		EntryNode* current_node = head;
		//��һ�α���������max_length[numAttrs]
		while(current_node){
			for(int i=0;i<numAttrs;++i){
				int this_length = current_node->entry[i].length();
				if(this_length > max_length[i]){
					max_length[i] = this_length;
				}
			}
			current_node = current_node->next;
		}

		print_row(this->attrs,max_length,numAttrs); //�Ȱ�attribute��ӡ����
		//cout << endl;
		current_node = head;
		//�ڶ��α���ֱ�Ӵ�ӡ
		while(current_node){
			print_row(current_node->entry,max_length,numAttrs);
			current_node = current_node->next;
			//if(current_node) cout<<endl;
		}
		return;
	}

//һ�����
	int attr_indices[numAttrs];
	int j=0;
	//attr_indices����attr��������ԭ�����ж�Ӧ��index
	//�ر�ע��attr��˳����Բ�ͬ��������
	while(j!=numAttrs){
		for(int i=0;i<this->numAttrs;++i){
			if(this->attrs[i]==attrs[j]) {
				attr_indices[j] = i;
				j++;
			}
		}
	}

	int max_length[numAttrs];

	for(int i=0;i<numAttrs;++i){
		max_length[i] = this->attrs[attr_indices[i]].length();
	}
	EntryNode* current_node = head;
	//��һ�α���������max_length[numAttrs]
	while(current_node){
		for(int i=0;i<numAttrs;++i){
			int this_length = current_node->entry[attr_indices[i]].length();
			if(this_length > max_length[i]){
				max_length[i] = this_length;
			}
		}
		current_node = current_node->next;
	}
	string temp[numAttrs];
	print_row(attrs,max_length,numAttrs); //�Ȱ�attribute��ӡ����
	//cout << endl;
	current_node = head;
	//�ڶ��α���ֱ�Ӵ�ӡ
	while(current_node){
		for(int i=0;i<numAttrs;++i){
			temp[i] = current_node->entry[attr_indices[i]];
		}
		print_row(temp,max_length,numAttrs);
		current_node = current_node->next;
		//if(current_node) cout<<endl;
	}
}
