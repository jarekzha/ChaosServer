#pragma once

template<typename T>
class Singleton
{
private:
    static T            obj;

protected:
                        Singleton()    {}
                        Singleton(const Singleton&)   {}
    Singleton&         operator=(const Singleton&)    {}

    virtual             ~Singleton()   {}
        
public:
    static T&           Instance()
    {
        return obj;
    }
};

template<typename T>
T Singleton<T>::obj;

