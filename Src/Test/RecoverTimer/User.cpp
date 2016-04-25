/*
 * User.cpp
 *
 *  Created on: 2014年9月26日
 *      Author: kcinz
 */

#include "User.h"
#include <iostream>
#include <fstream>

using namespace std;

User::User() : m_timer(boost::bind(&User::TimerFunc, this), boost::bind(&User::RecoverTimerFunc, this, _1))
{
}

User::~User()
{
}

void User::TimerFunc()
{
    cout << "User::" << __FUNCTION__ << endl;
}

void User::RecoverTimerFunc(int count)
{
    printf("User::%s(%d)\n", __FUNCTION__, count);
}

void User::Save(const User& user)
{
    ofstream ofs("User.db");

    boost::archive::binary_oarchive oa(ofs);

    oa << user;
}

void User::BeginTimer()
{
    m_timer.Start(10000);
}

bool User::Load(User& user)
{
    ifstream ifs("User.db");

    if (!ifs.good())
        return false;

    boost::archive::binary_iarchive ia(ifs);

    ia >> user;

    return true;
}
