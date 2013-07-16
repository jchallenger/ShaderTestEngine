
#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
protected:
    Singleton(void){ };
    Singleton(const T&);
    static T * instance;
public:
    ~Singleton(void){ };
    static T * getInstance(){
        if(instance == 0){ instance = new T; }
        return instance;
    };
};

template <typename T>
T * Singleton<T>::instance = 0;

#endif