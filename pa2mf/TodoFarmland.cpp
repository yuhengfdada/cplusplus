/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#include "TodoFarmland.h"
#include "Employee.h"
#include <cstdlib>

// Farmland cost 50 upgrade cost 10
// Farmland size 5, 5
// Farmland init max num. of employee is FARMLAND_MAX_NUM_EMPLOYEE_MIN
// Farmland Upgrade:
//      level increases by 1;
//      max num. of employee increases by 1 until FARMLAND_MAX_NUM_EMPLOYEE_MAX
// Farmland Makemoney:
//      getRandInt(0, num. of employees in work x 2 + num. of farmers in work x 6 + level x 3)

const int FARMLAND_COST = 50;
const int FARMLAND_UPGRADE_COST = 10;
const int FARMLAND_SIZE_X = 5; const int FARMLAND_SIZE_Y = 5;
const int FARMLAND_MAX_NUM_EMPLOYEE_MIN = 3;
const int FARMLAND_MAX_NUM_EMPLOYEE_MAX = (FARMLAND_SIZE_X-2) * (FARMLAND_SIZE_Y-2);

#ifdef __HARVEST_MOON_DEBUG__
    inline void debug_getRandInt(string s) {cout << s << endl;};
#else
    inline void debug_getRandInt(string s) {};
#endif

//return random int in [low, high).
int getRandInt(int low, int high) {
    int res = rand() % high + low;
    //debug_getRandInt("getRandInt: low is "+to_string(low));
    //debug_getRandInt("getRandInt: high is "+to_string(high));
    //debug_getRandInt("getRandInt: res is "+to_string(res));
    return res;
}

// TODO: Start to implement your code.
Farmland::Farmland(int x, int y):Property(FARMLAND_COST,FARMLAND_UPGRADE_COST,FARMLAND_MAX_NUM_EMPLOYEE_MIN){
	this->setSize(FARMLAND_SIZE_X,FARMLAND_SIZE_Y);
	this->setXY(x,y);
}
bool Farmland::checkEmployee(Employee* employee) const{
	char symbol = employee->getSymbol();
	if (symbol=='r' || symbol=='e'){
		return true;
	}
	return false;
}
void Farmland::upgrade(){
	Property::upgrade();
	if (this->getMaxNumEmployee() < FARMLAND_MAX_NUM_EMPLOYEE_MAX){
		this->setMaxNumEmployee(this->getMaxNumEmployee()+1);
	}
}
int Farmland::makeMoney() const{
	int num_employees_in_work = 0, num_farmers_in_work = 0;
	const Employee** temp;
	this->getConstEmployeeList(temp);
	for(int i=0;i<this->getNumEmployee();++i){
			if(temp[i]->getSymbol()=='e'&&temp[i]->getState()==ObjectState::WORK) num_employees_in_work++;
			if(temp[i]->getSymbol()=='f'&&temp[i]->getState()==ObjectState::WORK) num_farmers_in_work++;
		}
	delete [] temp;
	return getRandInt(0, num_employees_in_work * 2 + num_farmers_in_work * 6 + this->getLevel() * 3);
}
string Farmland::getName() const{
	return "Farmland";
}
char Farmland::getSymbol() const{
	return 'R';
}
