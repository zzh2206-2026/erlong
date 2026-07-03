//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 Project.hpp
//【功能模块和目的】         项目模型核心类，管理任务、依赖、资源及 CPM 调度
//-------------------------------------------------------------------------------------------------------------------

#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "Task.hpp"
#include "BasicTask.hpp"
#include "MilestoneTask.hpp"
#include "Dependency.hpp"
#include "Resource.hpp"
#include "TaskResourceAssignment.hpp"

#include <memory>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------------------------------
//【类名】             Project
//【功能】             项目调度模型的核心容器与业务逻辑
//【接口说明】         提供任务/依赖/资源的增删改查、验证、CPM 计算等接口
//-------------------------------------------------------------------------------------------------------------------
class Project {
public:
    Project();

    // 项目基本信息
    const std::string& GetName() const;
    void SetName(const std::string& strName);
    void Clear();

    // 任务管理 2.3.1 ~ 2.3.5
    size_t GetTaskCount() const;
    const Task* GetTask(size_t uIndex) const;
    Task* GetTask(size_t uIndex);
    bool AddTask(const std::string& strName, int iDuration, size_t& uOutIndex);
    bool RemoveTask(size_t uIndex);
    bool ModifyTask(size_t uIndex, const std::string& strName, int iDuration);
    bool IsTaskNameExists(const std::string& strName, size_t uExcludeIndex) const;
    std::vector<size_t> GetPredecessorIndices(size_t uTaskIndex) const;
    std::vector<size_t> GetSuccessorIndices(size_t uTaskIndex) const;

    // 依赖管理 2.3.6 ~ 2.3.8
    size_t GetDependencyCount() const;
    const Dependency& GetDependency(size_t uIndex) const;
    bool AddDependency(size_t uPredecessorIndex, size_t uSuccessorIndex,
                       DependencyType::Type type, int iLag, size_t& uOutIndex);
    bool RemoveDependency(size_t uIndex);
    bool RemoveDependencyByPair(size_t uPredecessorIndex, size_t uSuccessorIndex);

    // 资源管理 2.3.9 ~ 2.3.11
    size_t GetResourceCount() const;
    const Resource& GetResource(size_t uIndex) const;
    bool AddResource(const std::string& strName, double rUnitCost, size_t& uOutIndex);
    bool RemoveResource(size_t uIndex);
    bool IsResourceNameExists(const std::string& strName) const;
    bool AssignResourceToTask(size_t uTaskIndex, size_t uResourceIndex, int iQuantity);
    std::vector<TaskResourceAssignment> GetTaskAssignments(size_t uTaskIndex) const;

    // 统计与调度 2.5
    int GetTotalDuration() const;
    bool IsValid() const;
    bool Inference(int& iTotalDuration, std::vector<int>& vCriticalPathFileIds) const;

    // 导入导出辅助：按文件 ID 查找容器索引
    int FindTaskIndexByFileId(int iFileId) const;
    int FindResourceIndexByFileId(int iFileId) const;
    bool AddTaskWithFileId(const std::string& strName, int iDuration,
                           int iFileId, bool bIsMilestone, size_t& uOutIndex);
    bool AddResourceWithFileId(const std::string& strName, double rUnitCost,
                               int iFileId, size_t& uOutIndex);
    bool AddDependencyByFileId(int iPredFileId, int iSuccFileId,
                               DependencyType::Type type, int iLag);
    bool AssignResourceByFileId(int iTaskFileId, int iResourceFileId, int iQuantity);

    // 直接访问容器（供 Exporter 使用）
    const std::vector<std::unique_ptr<Task>>& GetTasks() const;
    const std::vector<Dependency>& GetDependencies() const;
    const std::vector<Resource>& GetResources() const;
    const std::vector<TaskResourceAssignment>& GetAssignments() const;

private:
    void ConvertTaskType(size_t uIndex, int iNewDuration);
    bool WouldCreateCycle(size_t uPredecessorIndex, size_t uSuccessorIndex) const;
    bool HasIsolatedTask() const;
    bool HasInvalidDependencyReference() const;
    bool ComputeTopologicalOrder(std::vector<size_t>& vOrder) const;
    void ApplyForwardPass(const std::vector<size_t>& vOrder) const;
    void ApplyBackwardPass(const std::vector<size_t>& vOrder, int iProjectDuration) const;
    int ComputeEarliestStart(size_t uTaskIndex) const;
    int ComputeLatestFinish(size_t uTaskIndex, int iProjectDuration) const;

    std::string m_strName;                                      // 项目名称
    std::vector<std::unique_ptr<Task>> m_vTasks;                // 任务容器
    std::vector<Dependency> m_vDependencies;                    // 依赖容器
    std::vector<Resource> m_vResources;                         // 资源容器
    std::vector<TaskResourceAssignment> m_vAssignments;       // 资源分配
    mutable int m_iTotalDuration;                               // 项目总工期
    mutable bool m_bScheduleComputed;                           // 是否已计算调度
};

#endif
