//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 Resource.hpp
//【功能模块和目的】         项目资源类，描述可共享的实体资源
//-------------------------------------------------------------------------------------------------------------------

#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

//-------------------------------------------------------------------------------------------------------------------
//【类名】             Resource
//【功能】             独立于任务存在的资源实体
//【接口说明】         拥有名称、单位时间成本及 PPM 文件 ID
//-------------------------------------------------------------------------------------------------------------------
class Resource {
public:
    Resource(const std::string& strName, double rUnitCost, int iFileId);

    const std::string& GetName() const;
    double GetUnitCost() const;
    int GetFileId() const;

    void SetName(const std::string& strName);
    void SetUnitCost(double rUnitCost);
    void SetFileId(int iFileId);

private:
    std::string m_strName;    // 资源名称
    double m_rUnitCost;       // 单位时间成本
    int m_iFileId;            // PPM 文件中的资源 ID
};

#endif
