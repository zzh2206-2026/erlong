//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 TaskResourceAssignment.cpp
//【功能模块和目的】         任务资源分配关系实现
//-------------------------------------------------------------------------------------------------------------------

#include "TaskResourceAssignment.hpp"

TaskResourceAssignment::TaskResourceAssignment(int iTaskIndex, int iResourceIndex,
                                               int iQuantity)
    : m_iTaskIndex(iTaskIndex),
      m_iResourceIndex(iResourceIndex),
      m_iQuantity(iQuantity) {
}

int TaskResourceAssignment::GetTaskIndex() const {
    return m_iTaskIndex;
}

int TaskResourceAssignment::GetResourceIndex() const {
    return m_iResourceIndex;
}

int TaskResourceAssignment::GetQuantity() const {
    return m_iQuantity;
}

void TaskResourceAssignment::AdjustTaskIndex(int iRemovedIndex) {
    if (m_iTaskIndex > iRemovedIndex) {
        m_iTaskIndex--;
    }
}

void TaskResourceAssignment::AdjustResourceIndex(int iRemovedIndex) {
    if (m_iResourceIndex > iRemovedIndex) {
        m_iResourceIndex--;
    }
}
