/*
 * TimerTest.cpp
 *
 *  Created on: 2014年7月4日
 *  Author: jarek
 *  Brief: 
 */

#include <iostream>
#include <Language/TypeID.h>

using namespace std;


struct A
{
public:
    virtual ~A(){}
};

struct B : public A{};



int main()
{
    const A* var = new B;
    const A& var1 = *(new B);
    const int varConstInt = 1;
    int varInt = 1;
    cout << TypeID<B>::Name << endl;
    cout << VariableTypeName(var) << endl;
    cout << VariableTypeName(*var) << endl;
    cout << VariableTypeName(var1) << endl;
    cout << VariableTypeName(&varInt) << endl;
    cout << VariableTypeName(&varConstInt) << endl;
}


