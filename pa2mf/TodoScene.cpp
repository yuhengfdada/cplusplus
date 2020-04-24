/*
 * File Created: Mon Mar 16 2020
 * Author:
 * SID:
*/

#include "Scene.h"
#include "Property.h"
#include "TodoFarmland.h"
#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoFarmer.h"
#include "TodoFeeder.h"
#include "TodoCow.h"

const int SCENE_W = 45;
const int SCENE_H = 10;
const int PROPERTY_X = 35;
const int SCENE_INIT_MONEY = 200;

Scene::Scene()
: m_objects(nullptr), m_num_objects(0), m_money(SCENE_INIT_MONEY) {
    nextRound();
}

Scene::~Scene() {
    //shallowremove employees
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::EMPLOYEE)
            shallowRemoveEmployee(dynamic_cast<Employee*>(m_objects[i]));
    }
    // remove properties (also deep remove employees)
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::PROPERTY)
            delete m_objects[i];
    }
    delete [] m_objects;
}

void Scene::shallowRemoveEmployee(Employee* e) {
    if (m_num_objects <= 0) return;
    if (m_num_objects == 1 && e == m_objects[0]) {
        m_num_objects = 0;
        m_objects = nullptr;
        return;
    }
    int i;
	for (i = 0; i < m_num_objects; i++) {
		if (m_objects[i] == e) //
			break;
	}

    Object ** newObjects = new Object*[m_num_objects--];
	for (int j = 0; j < m_num_objects; j++)
		newObjects[j] = m_objects[j];
	if (m_num_objects != i){
		newObjects[i] = m_objects[m_num_objects];
	}
    delete [] m_objects;
    m_objects = newObjects;
}
/*
 void Scene::shallowRemoveEmployee(Employee* e) {
    if (m_num_objects <= 0) return;
    if (m_num_objects == 1 && e == m_objects[0]) {
        m_num_objects = 0;
        m_objects = nullptr;
        return;
    }
    int i;
 for (i = 0; i < m_num_objects; i++) {
  if (m_objects[i] == e)
   break;
 }
    Object ** newObjects = new Object*[m_num_objects--];
 for (int j = 0; j < m_num_objects; j++)
  newObjects[j] = m_objects[j];
 if (m_num_objects != i)
  newObjects[i] = m_objects[m_num_objects];
    delete [] m_objects;
    m_objects = newObjects;
}
 */
bool Scene::checkOverlap(const Property* newproperty) {
    int x, y, sz_x, sz_y;
    newproperty->getXY(x, y);
    newproperty->getSize(sz_x, sz_y);
    for(int xx=x; xx<x+sz_x; xx++)
        for(int yy=y; yy<y+sz_y; yy++)
            if(getObjectAt(xx, yy) != nullptr) return true;
    return false;
}


void Scene::addProperty(int property, int x, int y) {
    Property* newProperty = nullptr;
    switch(property) {
    case FARMLAND:
        newProperty = new Farmland(x,y);
        break;
    case CATTLEFARM:
        newProperty = new Cattlefarm(x, y);
        break;
    }
    if (newProperty == nullptr) return;
    if (newProperty->getCost() > m_money || checkOverlap(newProperty)) {
        delete newProperty;
        return;
    }
    addObject(newProperty);
    m_money -= newProperty->getCost();
}


bool Scene::hire(Property* p, int employee) {
    Employee* newEmployee = nullptr;
    switch(employee){
    case EMPLOYEE:
        newEmployee = new Employee();
        break;
    case FARMER:
        newEmployee = new Farmer();
        break;
    case FEEDER:
        newEmployee = new Feeder();
        break;
    case COW:
        newEmployee = new Cow();
        break;
    }
    if (newEmployee == nullptr) return false;
    if (newEmployee->getCost() > m_money || ! p->assignEmployee(newEmployee)) {
        delete newEmployee;
        return false;
    }
    addObject(newEmployee);
    m_money -= newEmployee->getCost();
    return true;
}


Object* Scene::getObjectAt(int s_x, int s_y) const {
	int x, y, sz_x, sz_y;
    // If employee is at s_x, s_y, get employee
    // else, get property
    // otherwise return null
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::PROPERTY)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	return nullptr;
}

//perform shallow copying
void Scene::addObject(Object* newobj) {
	Object** newobjects = new Object*[m_num_objects + 1];
	for (int i = 0; i < m_num_objects; i++)
		newobjects[i] = m_objects[i];
	newobjects[m_num_objects++] = newobj;
	if (m_num_objects != 1)
		delete [] m_objects;
	m_objects = newobjects;
}

void Scene::getConstObjects(const Object**& obj, int& count) const {
	count = m_num_objects;
	if (m_num_objects == 0) {
		obj =nullptr;
		return;
	}
	obj = new const Object*[m_num_objects];
	for (int i = 0; i < m_num_objects; i++)
		obj[i] = m_objects[i];
}

// TODO: Start to implement your code.

// If m_money < 0, return true.
bool Scene::isGameOver() const{
	if(m_money<0) return true;
	else return false;
}

// Remove the Property in the scene
//????
/*
 *
 *
 */
void Scene::removeProperty(Property* property){

	//shallow remove employees
	const Employee** temp_employee_list;//const????
	property->getConstEmployeeList(temp_employee_list);
	for(int i=0; i<property->getNumEmployee(); ++i){
		this->shallowRemoveEmployee(const_cast<Employee*>(temp_employee_list[i]));
	}
	//别忘了delete temp_employee_list
	delete [] temp_employee_list;
	//shallow remove property
    if (m_num_objects <= 0) return;
    if (m_num_objects == 1 && property == m_objects[0]) {
        m_num_objects = 0;
        delete m_objects[0];
        delete [] m_objects;
        m_objects = nullptr;
        return;
    }
    int i;
	for (i = 0; i < m_num_objects; i++) {
		if (m_objects[i] == property) //所以不同种类的pointer只要内容相同也可以直接相等(仅限inherited)
			break;
	}
    Object ** newObjects = new Object*[m_num_objects--];
	for (int j = 0; j < m_num_objects; j++)
		newObjects[j] = m_objects[j];
	if (m_num_objects != i)
		newObjects[i] = m_objects[m_num_objects]; //拿最后一个填第i个
    delete [] m_objects;
    m_objects = newObjects;
    //deep remove property
    delete property;
}

// Operations to finish this round before entering the next round.
// - Make money (property)
// - Compute salary  (employee.getsalary)
// - Update employee work age   (employee)
// - Update employee status  (employee)
// - Remove died cows  (Cattlefarm,有点烦！！)
void Scene::nextRound(){
	for(int i=0;i<this->m_num_objects;++i){
		if (m_objects[i]->getObjectType() == ObjectType::PROPERTY){
			Property* temp = dynamic_cast<Property*>(this->m_objects[i]);
			this->m_money += temp->makeMoney(); //make money
		}
		if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE){
			Employee* temp = dynamic_cast<Employee*>(this->m_objects[i]);
			this->m_money -= temp->getSalary(); //salary
			temp->updateWorkAge(); //workage
			temp->updateState(); //status
		}
	}
	//remove cows

	//第一个循环是会动object list的，所以要小心！
	for(int i=0;i<this->m_num_objects;++i){
		if(i==this->m_num_objects) break;
		if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE){
			Employee* temp = dynamic_cast<Employee*>(this->m_objects[i]);
			if(temp->getSymbol()=='c'&&temp->getWorkAge()>=7){
				this->shallowRemoveEmployee(temp);
				i--;
			}
		}
	}

	for(int i=0;i<this->m_num_objects;++i){
		if (m_objects[i]->getObjectType() == ObjectType::PROPERTY){
			Property* temp = dynamic_cast<Property*>(this->m_objects[i]);
			if(temp->getSymbol()=='C'){
				Cattlefarm* temp2 = dynamic_cast<Cattlefarm*>(temp);
				//cout << "remove died cow is called" << endl;
				temp2->removeDiedCow();
			}
		}
	}

}
// Upgrade property
bool Scene::upgrade(Property* property){
	if(!property) return false;
	if(m_money<property->getUpgradeCost()) return false;
	property->upgrade();
	m_money -= property->getUpgradeCost();
	return true;
}
// Property fires an employee.
// The first parameter is an Employee pointer.
// You should find its property and fire the employee from the property.
bool Scene::fire(Employee* wannaFire){
	if(!wannaFire) return false;
	int x, y, sz_x, sz_y;
	int s_x,s_y;
	Property* property = nullptr;
	wannaFire->getXY(s_x,s_y);
	//find parent property
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)  property = dynamic_cast<Property*> (m_objects[i]);
	}
	if(property->fireEmployee(wannaFire)){
		this->shallowRemoveEmployee(wannaFire);
		return true;
	}
	else return false;
}

