#ifndef __RLEAVRS_SINGLETON_H_
#define __RLEAVRS_SINGLETON_H_


#include <memory>
namespace rleavrs {

template<class T>
class Singleton {
public:
    static T* GetInstance() {
        static T v;
        return &v;
    }

};

template<class T> 
class SingletonPtr {
public:
    static T* GetInstance() {
        static std::shared_ptr<T> v(new T);
        return v;
    }  
};


}


#endif