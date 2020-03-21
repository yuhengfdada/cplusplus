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
	//更新attrs（不管有没有entries都要更新）
	string* temp_attrs = new string[numAttrs+1]; //第一次new
	for(int j=0;j<numAttrs+1;++j){
		if(j<index) {temp_attrs[j] = attrs[j];}
		if(j==index) {temp_attrs[j] = attr;}
		if(j>index) {temp_attrs[j] = attrs[j-1];}
	}
	if(attrs) delete [] attrs; //第一次delete
	attrs = temp_attrs;

	//只有原来有entries的时候需要更新entries
	if(entries){
		string** temp_entries = new string*[numEntries]; //第二次new
		for(int i=0;i<numEntries;++i){
			temp_entries[i] = new string[numAttrs+1]; //第三次new
			for(int j=0;j<numAttrs+1;++j){
				if(j<index) {temp_entries[i][j] = entries[i][j];}
				if(j==index) {temp_entries[i][j] = default_value;}
				if(j>index) {temp_entries[i][j] = entries[i][j-1];}
			}
		}
		//delete 原来的entries
		for(int i=0;i<numEntries;++i){
				delete [] entries[i]; //第二次delete
		}
		delete [] entries;  //第三次delete
		//更新entries
		entries = temp_entries;
	}
	//更新numAttrs
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
				temp_entries[i][j] = entry[j]; //不能直接把entry赋到string数组里,要一个一个来
			}
		}
		if(i>index) {
			//temp_entries[i] = entries[i-1];
			for(int j=0;j<numAttrs;++j){
				temp_entries[i][j] = entries[i-1][j];
			}
		}
	}
	//new了两次，所以delete两次（注意old entries开始可能为nullptr）
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
	if(index<0 || index>numAttrs-1) return false; //原来就没有attribute也return false
	//原来只有一个attribute，删了之后全删
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
	//一般情况：删了还剩下一个或多个attributes
	//不管有没有entries都要更新attr
	string* temp_attrs = new string[numAttrs-1]; //第一次new
	for(int j=0;j<numAttrs-1;++j){
		if(j<index) {temp_attrs[j] = attrs[j];}
		if(j>=index) {temp_attrs[j] = attrs[j+1];}
	}
	delete [] attrs; //第一次delete
	attrs = temp_attrs;

	//只有原来有entries的时候需要更新entries
	if(entries){
		string** temp_entries = new string*[numEntries]; //第二次new
		for(int i=0;i<numEntries;++i){
			temp_entries[i] = new string[numAttrs-1]; //第三次new
			for(int j=0;j<numAttrs-1;++j){
				if(j<index) {temp_entries[i][j] = entries[i][j];}
				if(j>=index) {temp_entries[i][j] = entries[i][j+1];}
			}
		}
		//delete 原来的entries
		for(int i=0;i<numEntries;++i){
				delete [] entries[i]; //第二次delete
		}
		delete [] entries;  //第三次delete
		//更新entries
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
	if(index<0 || index>numEntries-1) return false;//原来就没有entry也return false
	if(numEntries==1){//如果只有一个就把entries设成nullptr
		for(int i=0;i<numEntries;++i){
			delete [] entries[i];
		}
		delete [] entries;
		entries = nullptr;
		numEntries--;
		return true;
	}

	//一般情况：delete之后还剩下一些entries
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
	//new了两次，所以delete两次
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

	//if same, append("this" and "another"都有可能是空的)
	if(!entries&&!another.entries) return true; //如果两个都没有entry，直接返回

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
	//delete原来的entries（原来是空的话就不用）
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




