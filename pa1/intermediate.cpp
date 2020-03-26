#include <iostream>
#include "intermediate.h"
#include "table.h"
using namespace std;

/*
 struct EntryNode {
	string *entry; //指向一个string数组（table中的一条entry）

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
//我佛了，不能访问private member。到时候再改吧，烦死了
//该函数从链表"intermediate"中delete一个EntryNode,并使pointer"entry"永远指向被删除node的下一个node.
void delete_node(Intermediate& intermediate, EntryNode*& current_node){
	if(current_node==this->head){
		EntryNode* next = current_node->next;
		this->head=next;
		delete current_node;
		if(next){
			next->prev=nullptr;
			current_node = next;
		}
		//删空了
		else this->tail=nullptr;
	}
	else if(current_node==this->tail){ //这里是不会出现删空的情况的
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

//根据array[i][attr_index]所代表的string来排序。选择排序。
/*
void simple_sort(string* array[],int attr_index,enum order order,int array_size){
	if (order==ASCENDING){
		string min_string; //每一轮最小的string
		int min_index; //min所在的index
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
		string max_string; //每一轮最大的string
		int max_index; //max所在的index
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
		for(int i=0;i<array_size;i++){//第i趟
			for(int j=0;j<array_size-1;j++){//第j个元素
				if(array[j][attr_index].compare(array[j+1][attr_index])>0){
					string* temp=array[j];
					array[j]=array[j+1];
					array[j+1]=temp;
				}
			}
		}
	}
	if(order==DESCENDING){
		for(int i=0;i<array_size;i++){//第i趟
			for(int j=0;j<array_size-1;j++){//第j个元素
				if(array[j][attr_index].compare(array[j+1][attr_index])<0){
					string* temp=array[j];
					array[j]=array[j+1];
					array[j+1]=temp;
				}
			}
		}
	}
}
//string数组"row"包含了一行的数据,max_len
void print_row(const string* row,int max_len[],int numAttrs){
	for(int i=0;i<numAttrs-1;++i){
		cout << _left_pad_until(row[i],max_len[i]) << " | ";
	}
	cout << _left_pad_until(row[numAttrs-1],max_len[numAttrs-1]) << endl;
}


Intermediate::Intermediate(const Table &table){
	//先搞定前两个
	attrs = table.attrs;
	numAttrs = table.numAttrs;
	//特殊情况：table是空的
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
	//特殊情况：空表
	if(!head) return *this;
	//先检查满足条件的attr是否存在（有且只有一个）
	bool flag = false;
	int attr_index; //符合条件的attr index
	for(int i=0;i<numAttrs;++i){
		if(attrs[i]==attr) {
			flag=true;
			attr_index = i;
			break;
		}
	}
	if(!flag) return *this;
	//mode是equal to
	if(mode==EQ){
		//如果存在attr，就把"attr"的值不等于"value"的entry都删了
		//遍历一遍
		EntryNode* current_node = head;
		while(current_node){
			if(current_node->entry[attr_index]!=value){
				//delete_node函数会自动将current_node后移，所以一句话即可
				//begin function
				if(current_node==head){
					EntryNode* next = current_node->next;
					head=next;
					delete current_node;
					if(next){
						next->prev=nullptr;
						current_node = next;
					}
					//删空了
					else {tail=nullptr;
					current_node = next;}
				}
				else if(current_node==tail){ //这里是不会出现删空的情况的
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
		//如果存在attr，就把"attr"的值不包含"value"的entry都删了
		//遍历一遍
		EntryNode* current_node = head;
		while(current_node){
			string value_to_examine = current_node->entry[attr_index];
			string::size_type position;
			position = value_to_examine.find(value);
			if(position==value_to_examine.npos){
				//delete_node函数会自动将current_node后移，所以一句话即可
				//begin function
				if(current_node==head){
					EntryNode* next = current_node->next;
					head=next;
					delete current_node;
					if(next){
						next->prev=nullptr;
						current_node = next;
					}
					//删空了
					else {tail=nullptr;
					current_node = next;}
				}
				else if(current_node==tail){ //这里是不会出现删空的情况的
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
	return *this;//其实没必要，只是不想看到warning
}

//思路：1.遍历一遍，创建对应的string数组 2.排序string数组 3.遍历一遍，把值赋回去
Intermediate& Intermediate::orderBy(const string &attr, enum order order){
	//特殊情况：空表
	if(!head) return *this;
	//先检查满足条件的attr是否存在（有且只有一个）
	bool flag = false;
	int attr_index; //符合条件的attr index
	for(int i=0;i<numAttrs;++i){
		if(attrs[i]==attr) {
			flag=true;
			attr_index = i;
			break;
		}
	}
	if(!flag) return *this;
	//计算entry数目，以便创建数组
	int numEntries = 0;
	EntryNode* current_node = head;
	while(current_node){
		numEntries++;
		current_node = current_node->next;
	}
	//创建string对应的array,并且将未排序的linked list储存进去
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

	//把array的值赋回去
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
	if(!head) return *this; //空intermediate
	//特殊情况1:limit==0，写一遍destructor就行了
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
	//特殊情况2:limit>=numEntries，return *this就好了
	unsigned int numEntries = 0;
	EntryNode* current_node = head;
	while(current_node){
		numEntries++;
		current_node = current_node->next;
	}
	if(limit>=numEntries){
		return *this;
	}
	//一般情况
	current_node = head;
	for(unsigned int count=1;count<limit;++count){//定位到要被切掉的node的“前一个”node
		current_node = current_node->next;
	}
	tail = current_node;
	EntryNode* temp_node = current_node->next;//第一个被切掉的node
	current_node->next = nullptr;
	current_node = temp_node;
	//以current_node为head,调用变形的destructor删掉后面所有
	while(current_node){
		EntryNode* next = current_node->next;
		delete current_node;
		current_node = next;
	}
	return *this;
}


void Intermediate::update(const string &attr, const string &new_value) const{
	//特殊情况：空表
	if(!head) return;
	//先检查满足条件的attr是否存在（有且只有一个）
	bool flag = false;
	int attr_index; //符合条件的attr index
	for(int i=0;i<numAttrs;++i){
		if(attrs[i]==attr) flag=true;
		attr_index = i;
	}
	if(!flag) return;
	//如果存在，遍历链表and update字段"attr"
	EntryNode* current_node = head;
	while(current_node){
		current_node->entry[attr_index] = new_value;
		current_node = current_node->next;
	}
}


void Intermediate::select(const string *attrs, int numAttrs) const{
//没有this->attrs, do nothing
	if(!this->attrs) return;
//attrs==nullptr, print all and return
	if(!attrs){
		numAttrs = this->numAttrs;
		int max_length[numAttrs];
		for(int i=0;i<numAttrs;++i){
			max_length[i] = this->attrs[i].length();
		}
		EntryNode* current_node = head;
		//第一次遍历，更新max_length[numAttrs]
		while(current_node){
			for(int i=0;i<numAttrs;++i){
				int this_length = current_node->entry[i].length();
				if(this_length > max_length[i]){
					max_length[i] = this_length;
				}
			}
			current_node = current_node->next;
		}

		print_row(this->attrs,max_length,numAttrs); //先把attribute打印出来
		//cout << endl;
		current_node = head;
		//第二次遍历直接打印
		while(current_node){
			print_row(current_node->entry,max_length,numAttrs);
			current_node = current_node->next;
			//if(current_node) cout<<endl;
		}
		return;
	}

//一般情况
	int attr_indices[numAttrs];
	int j=0;
	//attr_indices储存attr子数组在原数组中对应的index
	//特别注意attr的顺序可以不同！！！！
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
	//第一次遍历，更新max_length[numAttrs]
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
	print_row(attrs,max_length,numAttrs); //先把attribute打印出来
	//cout << endl;
	current_node = head;
	//第二次遍历直接打印
	while(current_node){
		for(int i=0;i<numAttrs;++i){
			temp[i] = current_node->entry[attr_indices[i]];
		}
		print_row(temp,max_length,numAttrs);
		current_node = current_node->next;
		//if(current_node) cout<<endl;
	}
}
