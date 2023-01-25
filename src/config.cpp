#include "config.h"

namespace rleavrs{
ConfigBase::ConfigBase(const std::string& name, const std::string& description):
    m_name(name),
    m_description(description) {}

const std::string ConfigBase::getName() const {
    return m_name;
}

const std::string ConfigBase::getDescription() const {
    return m_description;
}


template<class T>
JsonConfig<T>::JsonConfigVar(const std::string& name, const T& default_val,const std::string& description = "") {

}

template<class T>
std::string JsonConfig<T>::toString() override {

}

template<class T>
bool JsonConfig<T>::fromString(const std::string& val) override {


}










}