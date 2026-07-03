//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 MilestoneTask.cpp
//【功能模块和目的】         里程碑任务类实现
//-------------------------------------------------------------------------------------------------------------------

#include "MilestoneTask.hpp"

MilestoneTask::MilestoneTask(const std::string& strName, int iFileId)
    : Task(strName, 0, iFileId) {
}

int MilestoneTask::GetDuration() const {
    return 0;
}

void MilestoneTask::SetDuration(int iDuration) {
    // 里程碑任务工期恒为零
    (void)iDuration;
    m_iDuration = 0;
}

bool MilestoneTask::IsMilestone() const {
    return true;
}

std::string MilestoneTask::GetTypeName() const {
    return "MilestoneTask";
}

std::unique_ptr<Task> MilestoneTask::Clone() const {
    return std::unique_ptr<Task>(new MilestoneTask(m_strName, m_iFileId));
}
