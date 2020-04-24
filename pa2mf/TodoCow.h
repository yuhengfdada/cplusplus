/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#ifndef COW_H_
#define COW_H_

#include "Employee.h"
// TODO: Start to implement your code.
class Cow : public Employee{
public:
	Cow();
	~Cow();
	int getLifespan() const;
	bool isAlive() const;
	string getName() const;
	char getSymbol() const;
private:
	const int m_lifespan;
	void writeToStream(ostream& os) const override;
};

#endif /*COW_H_*/
