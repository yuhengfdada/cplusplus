/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/
#include <iostream>
#include "Property.h"
#include "Employee.h"
using namespace std;
void Property::writeToStream(ostream& os) const {
	Object::writeToStream(os);
	os << "Cost:" << getCost() << endl;
    os << "Upgrade Cost:" << getUpgradeCost() << endl;
    os << "Level:" << getLevel() << endl;
    os << "# of Employee:" << getNumEmployee() << " / " << getMaxNumEmployee() << endl;
}

void Property::upgrade() {
    m_level++;
}

void Property::getConstEmployeeList(const Employee**& employee) const {
	if (m_num_employee == 0) {
		employee =nullptr;
		return;
	}
	employee = new const Employee*[m_num_employee];
	for (int i = 0; i < m_num_employee; i++)
		employee[i] = m_employee_list[i];
}

// TODO: Start to implement your code.
// The first parameter is the cost to build the property.
// The second parameter is the cost to upgrade the property.
// The third parameter is the max number of employees.
Property::Property(int build_cost, int upgrade_cost, int max_num_employees):m_cost(build_cost),m_upgrade_cost(upgrade_cost),m_max_num_employee(max_num_employees){
	int x,y;
	this->getSize(x,y);
	this->m_employee_list = new Employee* [(x-2)*(y-2)];
}

Property::~Property(){
	for(int i=0;i<this->m_num_employee;++i){
		delete this->m_employee_list[i];
	}
	delete [] this->m_employee_list;
}

// Return the building cost of the property.
int Property::getCost() const{
	return m_cost;
}

// Return the upgrade cost of the property.
int Property::getUpgradeCost() const{
	return m_upgrade_cost;
}

// Return the level of the property.
int Property::getLevel() const{
	return m_level;
}

// Return the number of the employees assigned to the property.
int Property::getNumEmployee() const{
	return m_num_employee;
}

// Return the max number of the employees.
int Property::getMaxNumEmployee() const{
	return m_max_num_employee;
}

// Set the max number of the employees.
void Property::setMaxNumEmployee(int num){
	m_max_num_employee = num;
}

// Assign an employee to the property.
// You should update the m_employee_list (with shallow copying), m_num_employee
// and set the x,y of Employee.
// All the employees should always be inside the range of their property.
// If the Property, e.g.Farmland(R), is at (0, 3) with size (5, 5),
// the first employee should at (1, 4);
// the second employee should at (1, 5);
//  012346x
// 3RRRRR
// 4R147R
// 5R258R
// 6R369R
// 7RRRRR
// y
// If reaches m_max_num_employee, it fails to assign.
// If the employee already in the list, it fails to assign.
// If the employee is unsuitable to the property, it fails to assign.
// Return true, if assign successfully.
// Otherwise, return false.
bool Property::assignEmployee(Employee* newEmployee){
	if(!newEmployee) return false;
	if(this->m_num_employee>=this->m_max_num_employee) return false;
	if(!this->checkEmployee(newEmployee)) return false;
	//遍历数组看有没有一样的
	for(int i=0;i<this->m_num_employee;++i){
		if(this->m_employee_list[i]==newEmployee) return false;
	}
	//assign
	this->m_employee_list[this->m_num_employee] = newEmployee;
	m_num_employee++;
	//change state
	newEmployee->setState(ObjectState::WORK);
	//setXY
	int width,height;
	this->getSize(width,height);
	int px,py;
	this->getXY(px,py);
	int ex,ey;
	ex = px + 1 + (this->m_num_employee-1) / (width-2);
	ey = py + 1 + (this->m_num_employee-1) % (height-2);
	newEmployee->setXY(ex,ey);
	return true;

}

// Fire an employee.
// You should update the m_employee_list, m_num_employee
// and completely delete the employee.
// Assumption: You can assume the input parameter is in the m_employee_list if m_employee_list is not nullptr.
// Hint: You have to take care of the position of other employees after deleting one.
// Otherwise, it may cause a problem when you assigning new employees.
// Please note all the employees should always be inside their property.
// We gave some examples to clarify the "inside" and "outside" on the webpage.
// Return true, if assign successfully.
bool Property::fireEmployee(Employee* wannaFire){
	if(!wannaFire) return false;
	if(!this->m_employee_list) return false;

    if (m_num_employee == 1 && wannaFire == m_employee_list[0]) {
    	//cout << "ZhiShengYiGeLa!" << endl;
        m_num_employee = 0;
        delete wannaFire;
        delete [] m_employee_list;
    	int x,y;
    	this->getSize(x,y);
    	this->m_employee_list = new Employee* [(x-2)*(y-2)];
        return true;
    }
    int i;
	for (i = 0; i < m_num_employee; i++) {
		if (m_employee_list[i] == wannaFire) //
			break;
	}
	int a,b;
	wannaFire->getXY(a,b);
    Employee ** newObjects = new Employee*[m_num_employee--];
	for (int j = 0; j < m_num_employee; j++)
		newObjects[j] = m_employee_list[j];
	if (m_num_employee != i){
		newObjects[i] = m_employee_list[m_num_employee];
		newObjects[i]->setXY(a,b);
	}
	delete wannaFire;
    delete [] m_employee_list;
    m_employee_list = newObjects;
    return true;



	/*
	//search for specified employee
	for(int i=0; i<this->m_num_employee; ++i){
		if(wannaFire==this->m_employee_list[i]){
			//completely remove employee
			delete this->m_employee_list[i];
			this->m_employee_list[i] = nullptr;
			//reduce m_num_employee
			this->m_num_employee--;
			//rearrange
			for(int j=i; j<this->m_num_employee; ++j){
				this->m_employee_list[j] = this->m_employee_list[j+1];
			}
			break;
		}
	}
	return true;
*/
}
