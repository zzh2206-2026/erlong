//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 BasicTask.hpp
//【功能模块和目的】         普通任务类，工期大于零，可分配资源
//-------------------------------------------------------------------------------------------------------------------

#ifndef BASIC_TASK_HPP
#define BASIC_TASK_HPP

#include "Task.hpp"

//-------------------------------------------------------------------------------------------------------------------
//【类名】             BasicTask
//【功能】             表示工期大于零的普通工作任务
//【接口说明】         继承 Task，实现普通任务特有行为
//-------------------------------------------------------------------------------------------------------------------
class BasicTask : public Task {
public:
    BasicTask(const std::string& strName, int iDuration, int iFileId);
    bool IsMilestone() const override;
    std::string GetTypeName() const override;
    std::unique_ptr<Task> Clone() const override;
};

#endif
