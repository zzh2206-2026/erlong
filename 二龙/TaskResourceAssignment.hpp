//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 TaskResourceAssignment.hpp
//【功能模块和目的】         任务与资源分配关系
//-------------------------------------------------------------------------------------------------------------------

#ifndef TASK_RESOURCE_ASSIGNMENT_HPP
#define TASK_RESOURCE_ASSIGNMENT_HPP

//-------------------------------------------------------------------------------------------------------------------
//【类名】             TaskResourceAssignment
//【功能】             记录任务对资源的占用数量
//【接口说明】         存储任务索引、资源索引及占用数量
//-------------------------------------------------------------------------------------------------------------------
class TaskResourceAssignment {
public:
    TaskResourceAssignment(int iTaskIndex, int iResourceIndex, int iQuantity);

    int GetTaskIndex() const;
    int GetResourceIndex() const;
    int GetQuantity() const;

    void AdjustTaskIndex(int iRemovedIndex);
    void AdjustResourceIndex(int iRemovedIndex);

private:
    int m_iTaskIndex;       // 任务容器索引
    int m_iResourceIndex;   // 资源容器索引
    int m_iQuantity;        // 占用资源数量
};

#endif
