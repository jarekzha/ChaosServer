/*
 * EnumTest.cpp
 *
 *  Created on: 2015年11月3日
 *  Author: jarek
 *  Brief: 
 */

#include <stdio.h>
#include <Engine/Time/Time.h>
#include <Engine/Time/Timer.h>
#include <Engine/Time/TimerManager.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/FakeRandom.h>
#include <iostream>

using namespace std;

/////////////////////////////////////////////

enum SyncOption
{
    SyncOption_Yes = 0,
    SyncOption_No  = 1,
};


int main()
{

    SyncOption op = SyncOption_No;
    SyncOption op2= SyncOption_Yes;
    op2 = op;

    printf("syop: %d \n" , op);
    cout << "cout: op: "  << op <<  endl;
    cout << "------------------------" << endl;



    return 0;
}
