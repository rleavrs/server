#ifndef __RLEAVRS_CONFIG_H__
#define __RLEAVRS_CONFIG_H__

#include <memory>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace rleavrs {

class ConfigBase {
public:
    typedef std::shared_ptr<ConfigBase> ptr;

    ConfigBase(const std::string& name, const std::string& description = "");

    virtual ~ConfigBase() = 0;

    const std::string getName() const;

    const std::string getDescription() const;

    virtual std::string toString() = 0;

    virtual bool fromString(const std::string& val) = 0;


protected:
    std::string m_name;

    std::string m_description;

};

template<class T>
class JsonConfig : public ConfigBase {
public:
    JsonConfig(const std::string& name,const T& default_val ,const std::string& description = "");

    std::string toString() override;
    
    bool fromString(const std::string& val) override;


private:
    T m_val;

};


}


#endif