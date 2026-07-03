//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 Dependency.hpp
//【功能模块和目的】         任务间依赖关系类
//-------------------------------------------------------------------------------------------------------------------

#ifndef DEPENDENCY_HPP
#define DEPENDENCY_HPP

#include "DependencyType.hpp"

//-------------------------------------------------------------------------------------------------------------------
//【类名】             Dependency
//【功能】             描述两个任务之间的时序约束关系
//【接口说明】         存储前置/后置任务索引、依赖类型及 Lag 值
//-------------------------------------------------------------------------------------------------------------------
class Dependency {
public:
    Dependency(int iPredecessorIndex, int iSuccessorIndex,
               DependencyType::Type type, int iLag);

    int GetPredecessorIndex() const;
    int GetSuccessorIndex() const;
    DependencyType::Type GetType() const;
    int GetLag() const;

    void SetPredecessorIndex(int iIndex);
    void SetSuccessorIndex(int iIndex);
    void AdjustIndex(int iRemovedIndex);

    bool IsSamePair(int iPredecessorIndex, int iSuccessorIndex) const;

private:
    int m_iPredecessorIndex;              // 前置任务容器索引
    int m_iSuccessorIndex;                // 后置任务容器索引
    DependencyType::Type m_eType;         // 依赖类型
    int m_iLag;                           // 滞后/提前量
};

#endif
