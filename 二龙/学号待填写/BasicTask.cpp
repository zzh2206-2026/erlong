//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 BasicTask.cpp
//【功能模块和目的】         普通任务类实现
//-------------------------------------------------------------------------------------------------------------------

#include "BasicTask.hpp"

BasicTask::BasicTask(const std::string& strName, int iDuration, int iFileId)
    : Task(strName, iDuration, iFileId) {
}

bool BasicTask::IsMilestone() const {
    return false;
}

std::string BasicTask::GetTypeName() const {
    return "BasicTask";
}

std::unique_ptr<Task> BasicTask::Clone() const {
    return std::unique_ptr<Task>(new BasicTask(m_strName, m_iDuration, m_iFileId));
}
