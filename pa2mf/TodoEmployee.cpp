/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/
#include <iostream>
#include "Employee.h"
using namespace std;
// Employee cost 5 salary 1.
// Employee 1 day work & 1 day rest.
const int EMPLOYEE_COST = 5;
const int EMPLOYEE_SALARY = 1; // Get paied when in both work and rest states.
const int EMPLOYEE_NUM_WORK_DAYS = 1;
const int EMPLOYEE_NUM_REST_DAYS = 1;
// All types of Employees have size 1.
const int ALLEMPLOYEE_SIZE_X = 1; const int ALLEMPLOYEE_SIZE_Y = 1;

Employee::Employee()
: Employee(EMPLOYEE_COST, EMPLOYEE_SALARY, EMPLOYEE_NUM_WORK_DAYS, EMPLOYEE_NUM_REST_DAYS) {
}

Employee::Employee(int cost, int salary, int ndays_work, int ndays_rest)
: m_cost(cost), m_salary(salary), m_ndays_work(ndays_work), m_ndays_rest(ndays_rest) {
    setSize(ALLEMPLOYEE_SIZE_X, ALLEMPLOYEE_SIZE_Y);
}

Employee::~Employee() {
    //Auto-generated destructor stub
}

void Employee::writeToStream(ostream& os) const {
	Object::writeToStream(os);
    os << "Work Age: " << getWorkAge() << endl;
    os << "Salary: " << getSalary() << endl;
}

// TODO: Start to implement your code.
int Employee::getCost() const{
	return m_cost;
}

//Return the number of work days of the employee.
int Employee::getNumWorkDays() const{
	return m_ndays_work;
}

//Return the number of rest days of the employee.
int Employee::getNumRestDays() const{
	return m_ndays_rest;
}

//Return the work age of the employee.
int Employee::getWorkAge() const{
	return m_work_age;
}

//Return the salary according to the specification.
int Employee::getSalary() const{
	return m_salary;
}

//Update the state: Work/Rest according to the m_work_age and the specification
//If the employee has not been assigned to a property,
//Its state is always NORMAL.
//If the employee has been assigned.
//Its state changes between WORK(first)/REST.
void Employee::updateState(){
	int cycle = this->m_ndays_rest + this->m_ndays_work;
	if(this->getState()==ObjectState::WORK){
		if(this->m_work_age % cycle == this->m_ndays_work){
			this->setState(ObjectState::REST);
		}
	}
	else if (this->getState()==ObjectState::REST){
		if(this->m_work_age % cycle == this->m_ndays_rest){
			this->setState(ObjectState::WORK);
		}
	}
}

//Increase the workage by one
void Employee::updateWorkAge(){
	m_work_age++;
}


//return the "Employee"
string Employee::getName() const{
	return "Employee";
}

//return the "e" (Captain Sensitive)
char Employee::getSymbol() const{
	return 'e';
}
