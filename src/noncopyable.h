#ifndef __RLEAVRS_BASEOBJECT_H__
#define __RLEAVRS_BASEOBJECT_H__

namespace rleavrs {

class Noncopyable{
    Noncopyable() = default;
    
    ~Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;

    Noncopyable& operator=(const Noncopyable&) = delete;
};

class Singleton {
public:
    


private:
    

};

}

#endif