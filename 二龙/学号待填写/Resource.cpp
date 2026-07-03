//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 Resource.cpp
//【功能模块和目的】         资源类实现
//-------------------------------------------------------------------------------------------------------------------

#include "Resource.hpp"

Resource::Resource(const std::string& strName, double rUnitCost, int iFileId)
    : m_strName(strName),
      m_rUnitCost(rUnitCost),
      m_iFileId(iFileId) {
}

const std::string& Resource::GetName() const {
    return m_strName;
}

double Resource::GetUnitCost() const {
    return m_rUnitCost;
}

int Resource::GetFileId() const {
    return m_iFileId;
}

void Resource::SetName(const std::string& strName) {
    m_strName = strName;
}

void Resource::SetUnitCost(double rUnitCost) {
    m_rUnitCost = rUnitCost;
}

void Resource::SetFileId(int iFileId) {
    m_iFileId = iFileId;
}
