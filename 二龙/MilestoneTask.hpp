//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 MilestoneTask.hpp
//【功能模块和目的】         里程碑任务类，工期为零，不可分配资源
//-------------------------------------------------------------------------------------------------------------------

#ifndef MILESTONE_TASK_HPP
#define MILESTONE_TASK_HPP

#include "Task.hpp"

//-------------------------------------------------------------------------------------------------------------------
//【类名】             MilestoneTask
//【功能】             表示工期为零的里程碑节点
//【接口说明】         继承 Task，强制工期为零
//-------------------------------------------------------------------------------------------------------------------
class MilestoneTask : public Task {
public:
    MilestoneTask(const std::string& strName, int iFileId);
    int GetDuration() const override;
    void SetDuration(int iDuration) override;
    bool IsMilestone() const override;
    std::string GetTypeName() const override;
    std::unique_ptr<Task> Clone() const override;
};

#endif
