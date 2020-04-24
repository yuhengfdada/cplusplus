/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoCow.h"
#include <cstdlib>

// Cattlefarm cost 80 upgrade cost 16
// Cattlefarm size 6, 6
// Cattlefarm init max num. of employee is CATTLEFARM_MAX_NUM_EMPLOYEE_MIN
// Cattlefarm Upgrade:
//      level increases by 1;
//      max num. of employees increases by 1 until CATTLEFARM_MAX_NUM_EMPLOYEE_MAX
// Cattlefarm Makemoney:
//      min(num. of cows, num. of feeders in work) x level x 10;

const int CATTLEFARM_COST = 80;
const int CATTLEFARM_UPGRADE_COST = 16;
const int CATTLEFARM_SIZE_X = 6; const int CATTLEFARM_SIZE_Y = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MIN = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MAX = (CATTLEFARM_SIZE_X-2) * (CATTLEFARM_SIZE_Y-2);

// TODO: Start to implement your code.
Cattlefarm::Cattlefarm(int x, int y):Property(CATTLEFARM_COST,CATTLEFARM_UPGRADE_COST,CATTLEFARM_MAX_NUM_EMPLOYEE_MIN){
	this->setSize(CATTLEFARM_SIZE_X,CATTLEFARM_SIZE_Y);
	this->setXY(x,y);
}
bool Cattlefarm::checkEmployee(Employee* employee) const{
	char symbol = employee->getSymbol();
	if (symbol=='d' || symbol=='c'){
		return true;
	}
	return false;
}
void Cattlefarm::upgrade(){
	Property::upgrade();
	if(this->getMaxNumEmployee() < CATTLEFARM_MAX_NUM_EMPLOYEE_MAX) this->setMaxNumEmployee(this->getMaxNumEmployee()+1);
}
int Cattlefarm::makeMoney() const{
	int num_cows = 0, num_feeders_in_work = 0;
	const Employee** temp;
	this->getConstEmployeeList(temp);
	for(int i=0;i<this->getNumEmployee();++i){
		if(temp[i]->getSymbol()=='c') num_cows++;
		if(temp[i]->getSymbol()=='d'&&temp[i]->getState()==ObjectState::WORK) num_feeders_in_work++;
	}
	int ret;
	if(num_cows<=num_feeders_in_work) ret = num_cows;
	else ret = num_feeders_in_work;
	delete [] temp;
	return ret * this->getLevel() * 10;
}
string Cattlefarm::getName() const{
	return "Cattlefarm";
}
char Cattlefarm::getSymbol() const{
	return 'C';
}
void Cattlefarm::removeDiedCow(){
	const Employee** temp;
	this->getConstEmployeeList(temp);
	//cout << this->getNumEmployee() << endl;
	//cout << "print cattlefarm employee" <<endl;
    //for(int i=0;i<this->getNumEmployee();++i){
    	//cout <<"#"<<i<<endl;
    	//cout << temp[i]->getSymbol()<<endl;
    //}
    //cout << "µ½µ×¼¸¸öemployee??  " << this->getNumEmployee() << endl;
    int num = this->getNumEmployee();
	for(int i=0;i<num;++i){
		//if(!temp[i]) break;
		//cout << "i = " << i << endl;
		//cout << "Symbol = " << temp[i]->getSymbol() << ", workage = " << temp[i]->getWorkAge() << endl;
		if(temp[i]->getSymbol()=='c'&&temp[i]->getWorkAge()>=7){
			Employee* temp2 = const_cast<Employee*>(temp[i]);

			//cout << "this->fireEmployee is called" << endl;
			this->fireEmployee(temp2);
		}
	}
	delete [] temp;
}
