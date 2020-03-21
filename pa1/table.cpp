#include <iostream>
#include "table.h"

using namespace std;
/*begin debug
void print_row(const string* row,int numAttrs){
	for(int i=0;i<numAttrs-1;++i){
		cout << row[i]<< " | ";
	}
	cout << row[numAttrs-1]<< endl;
}

void Table::print() const{
	print_row(attrs,numAttrs);
	for (int i=0;i<numEntries;++i){
		print_row(entries[i],numAttrs);
	}
}
end debug*/


//default constructor
Table::Table(){
	attrs = nullptr;
	entries = nullptr;
	numAttrs = 0;
	numEntries = 0;
}

//copy constructor
Table::Table(const Table &another){
	attrs = new string[another.numAttrs];
	for(int i=0;i<another.numAttrs;++i){
		attrs[i] = another.attrs[i];
	}
	entries = new string*[another.numEntries];
	for(int i=0;i<another.numEntries;++i){
		entries[i] = new string[another.numAttrs];
		for(int j=0;j<another.numAttrs;++j){
			entries[i][j] = another.entries[i][j];
		}
	}
	numAttrs = another.numAttrs;
	numEntries = another.numEntries;
}

//Destructor
Table::~Table(){
	if(attrs) delete [] attrs;
	if(entries){
		for(int i=0;i<numEntries;++i){
			if(entries[i])	delete [] entries[i];
		}
		delete [] entries;
	}
}

/*
Add the attribute attr at index. Existing attributes are right shifted.
If the table has existing entries, the 2D array, entries, must be expanded. The values of the new attribute is default_value.
If index == -1, insert attribute at the end.
If index is out of range, return false immediately.
Otherwise, proceed and return true.
*/
bool Table::addAttribute(const string &attr, int index, const string &default_value){
	if(index<-1 || index>numAttrs) return false;
	if(index==-1) index = numAttrs;
	//����attrs��������û��entries��Ҫ���£�
	string* temp_attrs = new string[numAttrs+1]; //��һ��new
	for(int j=0;j<numAttrs+1;++j){
		if(j<index) {temp_attrs[j] = attrs[j];}
		if(j==index) {temp_attrs[j] = attr;}
		if(j>index) {temp_attrs[j] = attrs[j-1];}
	}
	if(attrs) delete [] attrs; //��һ��delete
	attrs = temp_attrs;

	//ֻ��ԭ����entries��ʱ����Ҫ����entries
	if(entries){
		string** temp_entries = new string*[numEntries]; //�ڶ���new
		for(int i=0;i<numEntries;++i){
			temp_entries[i] = new string[numAttrs+1]; //������new
			for(int j=0;j<numAttrs+1;++j){
				if(j<index) {temp_entries[i][j] = entries[i][j];}
				if(j==index) {temp_entries[i][j] = default_value;}
				if(j>index) {temp_entries[i][j] = entries[i][j-1];}
			}
		}
		//delete ԭ����entries
		for(int i=0;i<numEntries;++i){
				delete [] entries[i]; //�ڶ���delete
		}
		delete [] entries;  //������delete
		//����entries
		entries = temp_entries;
	}
	//����numAttrs
	numAttrs++;
	return true;
}

/*
Add the entry "entry" at index. Existing entries are down shifted.
If index == -1, insert "entry" at the end.
If index is out of range, return false immediately.
Otherwise, proceed and return true.
Input assertion: "entry" is guaranteed to have size of numAttrs.
Input assertion: Table has at least one attribute (numAttrs > 0 and attrs != nullptr).
 */
bool Table::addEntry(const string *entry, int index){
	if(index<-1 || index>numEntries) return false;
	if(index==-1) index = numEntries;
	string** temp_entries = new string*[numEntries+1];
	for(int i=0;i<numEntries+1;++i){
		temp_entries[i] = new string[numAttrs];
		if(i<index) {
			//temp_entries[i] = entries[i];
			for(int j=0;j<numAttrs;++j){
				temp_entries[i][j] = entries[i][j];
			}
		}
		if(i==index) {
			for(int j=0;j<numAttrs;++j){
				temp_entries[i][j] = entry[j]; //����ֱ�Ӱ�entry����string������,Ҫһ��һ����
			}
		}
		if(i>index) {
			//temp_entries[i] = entries[i-1];
			for(int j=0;j<numAttrs;++j){
				temp_entries[i][j] = entries[i-1][j];
			}
		}
	}
	//new�����Σ�����delete���Σ�ע��old entries��ʼ����Ϊnullptr��
	if(entries){
		for(int i=0;i<numEntries;++i){
				delete [] entries[i];
		}
		delete [] entries;
	}
	entries = temp_entries;
	numEntries++;
	return true;
}

/*
Delete the attribute at index.
If index is out of range, return false immediately.
Otherwise, proceed and return true.
If the table has existing entries, the 2D array, entries, must be shrinked.
If there are no attributes left, delete all remaining entries.
 */
bool Table::deleteAttribute(int index){
	if(index<0 || index>numAttrs-1) return false; //ԭ����û��attributeҲreturn false
	//ԭ��ֻ��һ��attribute��ɾ��֮��ȫɾ
	if(numAttrs==1){
		delete [] attrs;
		attrs = nullptr;
		if(entries){
			for(int i=0;i<numEntries;++i){
				delete [] entries[i];
			}
			delete [] entries;
			entries = nullptr;
		}
		numAttrs--;
		numEntries = 0;
		return true;
	}
	//һ�������ɾ�˻�ʣ��һ������attributes
	//������û��entries��Ҫ����attr
	string* temp_attrs = new string[numAttrs-1]; //��һ��new
	for(int j=0;j<numAttrs-1;++j){
		if(j<index) {temp_attrs[j] = attrs[j];}
		if(j>=index) {temp_attrs[j] = attrs[j+1];}
	}
	delete [] attrs; //��һ��delete
	attrs = temp_attrs;

	//ֻ��ԭ����entries��ʱ����Ҫ����entries
	if(entries){
		string** temp_entries = new string*[numEntries]; //�ڶ���new
		for(int i=0;i<numEntries;++i){
			temp_entries[i] = new string[numAttrs-1]; //������new
			for(int j=0;j<numAttrs-1;++j){
				if(j<index) {temp_entries[i][j] = entries[i][j];}
				if(j>=index) {temp_entries[i][j] = entries[i][j+1];}
			}
		}
		//delete ԭ����entries
		for(int i=0;i<numEntries;++i){
				delete [] entries[i]; //�ڶ���delete
		}
		delete [] entries;  //������delete
		//����entries
		entries = temp_entries;
	}
	numAttrs--;
	return true;
}

/*
Delete the entry at index.
If index is out of range, return false immediately.
Otherwise, proceed and return true.
Keep the attributes even though no entries remain.
 */
bool Table::deleteEntry(int index){
	if(index<0 || index>numEntries-1) return false;//ԭ����û��entryҲreturn false
	if(numEntries==1){//���ֻ��һ���Ͱ�entries���nullptr
		for(int i=0;i<numEntries;++i){
			delete [] entries[i];
		}
		delete [] entries;
		entries = nullptr;
		numEntries--;
		return true;
	}

	//һ�������delete֮��ʣ��һЩentries
	string** temp_entries = new string*[numEntries-1];
	for(int i=0;i<numEntries-1;++i){
		temp_entries[i] = new string[numAttrs];
		if(i<index) {
			for(int j=0;j<numAttrs;++j){
				temp_entries[i][j] = entries[i][j];
			}
		}
		if(i>=index) {
			//temp_entries[i] = entries[i+1];
			for(int j=0;j<numAttrs;++j){
				temp_entries[i][j] = entries[i+1][j];
			}
		}
	}
	//new�����Σ�����delete����
	for(int i=0;i<numEntries;++i){
		delete [] entries[i];
	}
	delete [] entries;
	entries = temp_entries;
	numEntries--;
	return true;
}

/*
If the current table and another have different attributes, return false immediately. The order of attributes has to be the same.
Otherwise, append entries of another, by adding the entries of another to the end of current table, and return true.
 */
bool Table::append(const Table &another){
	//check if the attributes are the same
	if(numAttrs==another.numAttrs){
		for(int i=0;i<numAttrs;++i){
			if(attrs[i]!=another.attrs[i]) return false;
		}
	}
	else return false;

	//if same, append("this" and "another"���п����ǿյ�)
	if(!entries&&!another.entries) return true; //���������û��entry��ֱ�ӷ���

	string** temp_entries = new string*[numEntries+another.numEntries];
	for(int i=0;i<numEntries;++i){
		temp_entries[i] = new string[numAttrs];
		for(int j=0;j<numAttrs;++j){
			temp_entries[i][j] = entries[i][j];
		}
	}
	for(int j=numEntries;j<numEntries+another.numEntries;++j){
		temp_entries[j] = new string[numAttrs];
		for(int i=0;i<numAttrs;++i){
			temp_entries[j][i] = another.entries[j-numEntries][i];
		}
	}
	//deleteԭ����entries��ԭ���ǿյĻ��Ͳ��ã�
	if(entries){
		for(int i=0;i<numEntries;++i){
				delete [] entries[i];
		}
		delete [] entries;
	}
	entries = temp_entries;
	numEntries = numEntries+another.numEntries;
	return true;
}




