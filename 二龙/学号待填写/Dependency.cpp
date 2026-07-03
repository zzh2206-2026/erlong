//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 Dependency.cpp
//【功能模块和目的】         依赖关系类实现
//-------------------------------------------------------------------------------------------------------------------

#include "Dependency.hpp"

Dependency::Dependency(int iPredecessorIndex, int iSuccessorIndex,
                       DependencyType::Type type, int iLag)
    : m_iPredecessorIndex(iPredecessorIndex),
      m_iSuccessorIndex(iSuccessorIndex),
      m_eType(type),
      m_iLag(iLag) {
}

int Dependency::GetPredecessorIndex() const {
    return m_iPredecessorIndex;
}

int Dependency::GetSuccessorIndex() const {
    return m_iSuccessorIndex;
}

DependencyType::Type Dependency::GetType() const {
    return m_eType;
}

int Dependency::GetLag() const {
    return m_iLag;
}

void Dependency::SetPredecessorIndex(int iIndex) {
    m_iPredecessorIndex = iIndex;
}

void Dependency::SetSuccessorIndex(int iIndex) {
    m_iSuccessorIndex = iIndex;
}

void Dependency::AdjustIndex(int iRemovedIndex) {
    if (m_iPredecessorIndex > iRemovedIndex) {
        m_iPredecessorIndex--;
    }
    if (m_iSuccessorIndex > iRemovedIndex) {
        m_iSuccessorIndex--;
    }
}

bool Dependency::IsSamePair(int iPredecessorIndex, int iSuccessorIndex) const {
    return (m_iPredecessorIndex == iPredecessorIndex)
        && (m_iSuccessorIndex == iSuccessorIndex);
}
