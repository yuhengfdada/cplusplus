/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#ifndef FARMER_H_
#define FARMER_H_

#include "Employee.h"
// TODO: Start to implement your code.
class Farmer : public Employee{
public:
	Farmer();
	~Farmer();
	string getName() const;
	char getSymbol() const;
	virtual int getSalary() const;
};



#endif /*FARMER_H_*/
