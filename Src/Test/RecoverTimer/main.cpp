/*
 * main.cpp
 *
 *  Created on: 2014年9月26日
 *      Author: kcinz
 */

#include <iostream>
#include <Time/TimerManager.h>
#include <Time/Time.h>
#include <signal.h>

#include "User.h"

using namespace std;

User user;

bool isQuit = false;

static void Quit(int arg)
{
    isQuit = true;
}

int main()
{
    signal(SIGUSR1, Quit);

    TimerManager& timerManager = TimerManager::Instance();
    timerManager.Initialize();

    if (User::Load(user) == false)
        user.BeginTimer();

    while(!isQuit)
    {
        Time::UpdateTime();
        timerManager.Run();
    }

    User::Save(user);
}

