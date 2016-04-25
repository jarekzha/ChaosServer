/*
 * User.h
 *
 *  Created on: 2014年9月26日
 *      Author: kcinz
 */

#ifndef USER_H_
#define USER_H_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <Time/RecoverTimer.h>

class User
{
public:
    User();
    virtual ~User();

    void TimerFunc();
    void BeginTimer();

    void RecoverTimerFunc(int count);

    static void Save(const User& user);
    static bool Load(User& user);

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & m_timer;
    }

private:
    RecoverTimer m_timer;
};

#endif /* USER_H_ */
