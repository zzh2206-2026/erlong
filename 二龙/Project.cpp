//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 Project.cpp
//【功能模块和目的】         项目模型核心类实现
//-------------------------------------------------------------------------------------------------------------------

#include "Project.hpp"

#include <algorithm>
#include <queue>
#include <unordered_set>

Project::Project()
    : m_strName(""),
      m_iTotalDuration(0),
      m_bScheduleComputed(false) {
}

const std::string& Project::GetName() const {
    return m_strName;
}

void Project::SetName(const std::string& strName) {
    m_strName = strName;
}

void Project::Clear() {
    m_strName = "";
    m_vTasks.clear();
    m_vDependencies.clear();
    m_vResources.clear();
    m_vAssignments.clear();
    m_iTotalDuration = 0;
    m_bScheduleComputed = false;
}

size_t Project::GetTaskCount() const {
    return m_vTasks.size();
}

const Task* Project::GetTask(size_t uIndex) const {
    if (uIndex >= m_vTasks.size()) {
        return nullptr;
    }
    return m_vTasks[uIndex].get();
}

Task* Project::GetTask(size_t uIndex) {
    if (uIndex >= m_vTasks.size()) {
        return nullptr;
    }
    return m_vTasks[uIndex].get();
}

bool Project::IsTaskNameExists(const std::string& strName, size_t uExcludeIndex) const {
    for (size_t u = 0; u < m_vTasks.size(); u++) {
        if (u == uExcludeIndex) {
            continue;
        }
        if (m_vTasks[u]->GetName() == strName) {
            return true;
        }
    }
    return false;
}

void Project::ConvertTaskType(size_t uIndex, int iNewDuration) {
    if (uIndex >= m_vTasks.size()) {
        return;
    }

    Task* pTask = m_vTasks[uIndex].get();
    std::string strName = pTask->GetName();
    int iFileId = pTask->GetFileId();

    if (iNewDuration == 0) {
        m_vTasks[uIndex] = std::unique_ptr<Task>(new MilestoneTask(strName, iFileId));
    }
    else {
        m_vTasks[uIndex] = std::unique_ptr<Task>(new BasicTask(strName, iNewDuration, iFileId));
    }
}

bool Project::AddTask(const std::string& strName, int iDuration, size_t& uOutIndex) {
    if (IsTaskNameExists(strName, static_cast<size_t>(-1))) {
        return false;
    }

    int iFileId = static_cast<int>(m_vTasks.size()) + 1;
    return AddTaskWithFileId(strName, iDuration, iFileId, (iDuration == 0), uOutIndex);
}

bool Project::AddTaskWithFileId(const std::string& strName, int iDuration,
                                int iFileId, bool bIsMilestone, size_t& uOutIndex) {
    if (IsTaskNameExists(strName, static_cast<size_t>(-1))) {
        return false;
    }

    if (bIsMilestone || iDuration == 0) {
        m_vTasks.push_back(std::unique_ptr<Task>(new MilestoneTask(strName, iFileId)));
    }
    else {
        m_vTasks.push_back(std::unique_ptr<Task>(new BasicTask(strName, iDuration, iFileId)));
    }

    uOutIndex = m_vTasks.size() - 1;
    m_bScheduleComputed = false;
    return true;
}

bool Project::RemoveTask(size_t uIndex) {
    if (uIndex >= m_vTasks.size()) {
        return false;
    }

    // 删除相关依赖
    m_vDependencies.erase(
        std::remove_if(m_vDependencies.begin(), m_vDependencies.end(),
            [uIndex](const Dependency& dep) {
                return (static_cast<size_t>(dep.GetPredecessorIndex()) == uIndex)
                    || (static_cast<size_t>(dep.GetSuccessorIndex()) == uIndex);
            }),
        m_vDependencies.end());

    // 调整其余依赖索引
    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        m_vDependencies[u].AdjustIndex(static_cast<int>(uIndex));
    }

    // 删除资源分配并调整索引
    m_vAssignments.erase(
        std::remove_if(m_vAssignments.begin(), m_vAssignments.end(),
            [uIndex](const TaskResourceAssignment& assign) {
                return static_cast<size_t>(assign.GetTaskIndex()) == uIndex;
            }),
        m_vAssignments.end());

    for (size_t u = 0; u < m_vAssignments.size(); u++) {
        m_vAssignments[u].AdjustTaskIndex(static_cast<int>(uIndex));
    }

    m_vTasks.erase(m_vTasks.begin() + static_cast<std::ptrdiff_t>(uIndex));
    m_bScheduleComputed = false;
    return true;
}

bool Project::ModifyTask(size_t uIndex, const std::string& strName, int iDuration) {
    if (uIndex >= m_vTasks.size()) {
        return false;
    }

    if (IsTaskNameExists(strName, uIndex)) {
        return false;
    }

    Task* pTask = m_vTasks[uIndex].get();
    int iOldDuration = pTask->GetDuration();
    pTask->SetName(strName);

    if (iDuration != iOldDuration) {
        ConvertTaskType(uIndex, iDuration);

        // 若转为里程碑，清除其资源分配
        if (iDuration == 0) {
            m_vAssignments.erase(
                std::remove_if(m_vAssignments.begin(), m_vAssignments.end(),
                    [uIndex](const TaskResourceAssignment& assign) {
                        return static_cast<size_t>(assign.GetTaskIndex()) == uIndex;
                    }),
                m_vAssignments.end());
        }
    }

    m_bScheduleComputed = false;
    return true;
}

std::vector<size_t> Project::GetPredecessorIndices(size_t uTaskIndex) const {
    std::vector<size_t> vResult;
    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        if (static_cast<size_t>(m_vDependencies[u].GetSuccessorIndex()) == uTaskIndex) {
            vResult.push_back(static_cast<size_t>(m_vDependencies[u].GetPredecessorIndex()));
        }
    }
    return vResult;
}

std::vector<size_t> Project::GetSuccessorIndices(size_t uTaskIndex) const {
    std::vector<size_t> vResult;
    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        if (static_cast<size_t>(m_vDependencies[u].GetPredecessorIndex()) == uTaskIndex) {
            vResult.push_back(static_cast<size_t>(m_vDependencies[u].GetSuccessorIndex()));
        }
    }
    return vResult;
}

size_t Project::GetDependencyCount() const {
    return m_vDependencies.size();
}

const Dependency& Project::GetDependency(size_t uIndex) const {
    return m_vDependencies[uIndex];
}

bool Project::WouldCreateCycle(size_t uPredecessorIndex, size_t uSuccessorIndex) const {
    // 从后继出发 DFS，看能否到达前驱
    std::unordered_set<size_t> setVisited;
    std::vector<size_t> vStack;
    vStack.push_back(uSuccessorIndex);

    while (!vStack.empty()) {
        size_t uCurrent = vStack.back();
        vStack.pop_back();

        if (uCurrent == uPredecessorIndex) {
            return true;
        }

        if (setVisited.count(uCurrent) > 0) {
            continue;
        }
        setVisited.insert(uCurrent);

        std::vector<size_t> vSucc = GetSuccessorIndices(uCurrent);
        for (size_t u = 0; u < vSucc.size(); u++) {
            vStack.push_back(vSucc[u]);
        }
    }
    return false;
}

bool Project::AddDependency(size_t uPredecessorIndex, size_t uSuccessorIndex,
                            DependencyType::Type type, int iLag, size_t& uOutIndex) {
    if (uPredecessorIndex >= m_vTasks.size() || uSuccessorIndex >= m_vTasks.size()) {
        return false;
    }

    if (uPredecessorIndex == uSuccessorIndex) {
        return false;
    }

    // 检查重复
    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        if (m_vDependencies[u].IsSamePair(static_cast<int>(uPredecessorIndex),
                                          static_cast<int>(uSuccessorIndex))) {
            return false;
        }
    }

    // 检查环
    if (WouldCreateCycle(uPredecessorIndex, uSuccessorIndex)) {
        return false;
    }

    m_vDependencies.push_back(Dependency(static_cast<int>(uPredecessorIndex),
        static_cast<int>(uSuccessorIndex), type, iLag));
    uOutIndex = m_vDependencies.size() - 1;
    m_bScheduleComputed = false;
    return true;
}

bool Project::RemoveDependency(size_t uIndex) {
    if (uIndex >= m_vDependencies.size()) {
        return false;
    }
    m_vDependencies.erase(m_vDependencies.begin() + static_cast<std::ptrdiff_t>(uIndex));
    m_bScheduleComputed = false;
    return true;
}

bool Project::RemoveDependencyByPair(size_t uPredecessorIndex, size_t uSuccessorIndex) {
    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        if (m_vDependencies[u].IsSamePair(static_cast<int>(uPredecessorIndex),
                                          static_cast<int>(uSuccessorIndex))) {
            return RemoveDependency(u);
        }
    }
    return false;
}

size_t Project::GetResourceCount() const {
    return m_vResources.size();
}

const Resource& Project::GetResource(size_t uIndex) const {
    return m_vResources[uIndex];
}

bool Project::IsResourceNameExists(const std::string& strName) const {
    for (size_t u = 0; u < m_vResources.size(); u++) {
        if (m_vResources[u].GetName() == strName) {
            return true;
        }
    }
    return false;
}

bool Project::AddResource(const std::string& strName, double rUnitCost, size_t& uOutIndex) {
    if (IsResourceNameExists(strName)) {
        return false;
    }
    int iFileId = static_cast<int>(m_vResources.size()) + 101;
    return AddResourceWithFileId(strName, rUnitCost, iFileId, uOutIndex);
}

bool Project::AddResourceWithFileId(const std::string& strName, double rUnitCost,
                                    int iFileId, size_t& uOutIndex) {
    if (IsResourceNameExists(strName)) {
        return false;
    }
    m_vResources.push_back(Resource(strName, rUnitCost, iFileId));
    uOutIndex = m_vResources.size() - 1;
    return true;
}

bool Project::RemoveResource(size_t uIndex) {
    if (uIndex >= m_vResources.size()) {
        return false;
    }

    m_vAssignments.erase(
        std::remove_if(m_vAssignments.begin(), m_vAssignments.end(),
            [uIndex](const TaskResourceAssignment& assign) {
                return static_cast<size_t>(assign.GetResourceIndex()) == uIndex;
            }),
        m_vAssignments.end());

    for (size_t u = 0; u < m_vAssignments.size(); u++) {
        m_vAssignments[u].AdjustResourceIndex(static_cast<int>(uIndex));
    }

    m_vResources.erase(m_vResources.begin() + static_cast<std::ptrdiff_t>(uIndex));
    return true;
}

bool Project::AssignResourceToTask(size_t uTaskIndex, size_t uResourceIndex, int iQuantity) {
    if (uTaskIndex >= m_vTasks.size() || uResourceIndex >= m_vResources.size()) {
        return false;
    }

    if (iQuantity <= 0) {
        return false;
    }

    if (m_vTasks[uTaskIndex]->IsMilestone()) {
        return false;
    }

    m_vAssignments.push_back(TaskResourceAssignment(static_cast<int>(uTaskIndex),
        static_cast<int>(uResourceIndex), iQuantity));
    return true;
}

std::vector<TaskResourceAssignment> Project::GetTaskAssignments(size_t uTaskIndex) const {
    std::vector<TaskResourceAssignment> vResult;
    for (size_t u = 0; u < m_vAssignments.size(); u++) {
        if (static_cast<size_t>(m_vAssignments[u].GetTaskIndex()) == uTaskIndex) {
            vResult.push_back(m_vAssignments[u]);
        }
    }
    return vResult;
}

int Project::FindTaskIndexByFileId(int iFileId) const {
    for (size_t u = 0; u < m_vTasks.size(); u++) {
        if (m_vTasks[u]->GetFileId() == iFileId) {
            return static_cast<int>(u);
        }
    }
    return -1;
}

int Project::FindResourceIndexByFileId(int iFileId) const {
    for (size_t u = 0; u < m_vResources.size(); u++) {
        if (m_vResources[u].GetFileId() == iFileId) {
            return static_cast<int>(u);
        }
    }
    return -1;
}

bool Project::AddDependencyByFileId(int iPredFileId, int iSuccFileId,
                                    DependencyType::Type type, int iLag) {
    int iPredIndex = FindTaskIndexByFileId(iPredFileId);
    int iSuccIndex = FindTaskIndexByFileId(iSuccFileId);
    if (iPredIndex < 0 || iSuccIndex < 0) {
        return false;
    }
    size_t uOutIndex = 0;
    return AddDependency(static_cast<size_t>(iPredIndex),
        static_cast<size_t>(iSuccIndex), type, iLag, uOutIndex);
}

bool Project::AssignResourceByFileId(int iTaskFileId, int iResourceFileId, int iQuantity) {
    int iTaskIndex = FindTaskIndexByFileId(iTaskFileId);
    int iResIndex = FindResourceIndexByFileId(iResourceFileId);
    if (iTaskIndex < 0 || iResIndex < 0) {
        return false;
    }
    return AssignResourceToTask(static_cast<size_t>(iTaskIndex),
        static_cast<size_t>(iResIndex), iQuantity);
}

const std::vector<std::unique_ptr<Task>>& Project::GetTasks() const {
    return m_vTasks;
}

const std::vector<Dependency>& Project::GetDependencies() const {
    return m_vDependencies;
}

const std::vector<Resource>& Project::GetResources() const {
    return m_vResources;
}

const std::vector<TaskResourceAssignment>& Project::GetAssignments() const {
    return m_vAssignments;
}

bool Project::HasInvalidDependencyReference() const {
    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        int iPred = m_vDependencies[u].GetPredecessorIndex();
        int iSucc = m_vDependencies[u].GetSuccessorIndex();
        if (iPred < 0 || iSucc < 0) {
            return true;
        }
        if (static_cast<size_t>(iPred) >= m_vTasks.size()
            || static_cast<size_t>(iSucc) >= m_vTasks.size()) {
            return true;
        }
    }
    return false;
}

bool Project::ComputeTopologicalOrder(std::vector<size_t>& vOrder) const {
    size_t uCount = m_vTasks.size();
    std::vector<int> vInDegree(uCount, 0);

    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        int iSucc = m_vDependencies[u].GetSuccessorIndex();
        if (iSucc >= 0 && static_cast<size_t>(iSucc) < uCount) {
            vInDegree[static_cast<size_t>(iSucc)]++;
        }
    }

    std::queue<size_t> q;
    for (size_t u = 0; u < uCount; u++) {
        if (vInDegree[u] == 0) {
            q.push(u);
        }
    }

    vOrder.clear();
    while (!q.empty()) {
        size_t uNode = q.front();
        q.pop();
        vOrder.push_back(uNode);

        std::vector<size_t> vSucc = GetSuccessorIndices(uNode);
        for (size_t u = 0; u < vSucc.size(); u++) {
            size_t uSucc = vSucc[u];
            vInDegree[uSucc]--;
            if (vInDegree[uSucc] == 0) {
                q.push(uSucc);
            }
        }
    }

    return (vOrder.size() == uCount);
}

bool Project::HasIsolatedTask() const {
    if (m_vTasks.empty()) {
        return false;
    }

    if (m_vTasks.size() == 1) {
        return false;
    }

    size_t uCount = m_vTasks.size();

    // 弱连通性检查：忽略边的方向，图必须连通
    std::vector<bool> vVisited(uCount, false);
    std::vector<size_t> vStack;
    vStack.push_back(0);
    vVisited[0] = true;
    int iVisitedCount = 1;

    while (!vStack.empty()) {
        size_t uNode = vStack.back();
        vStack.pop_back();

        std::vector<size_t> vSucc = GetSuccessorIndices(uNode);
        for (size_t u = 0; u < vSucc.size(); u++) {
            if (!vVisited[vSucc[u]]) {
                vVisited[vSucc[u]] = true;
                iVisitedCount++;
                vStack.push_back(vSucc[u]);
            }
        }

        std::vector<size_t> vPred = GetPredecessorIndices(uNode);
        for (size_t u = 0; u < vPred.size(); u++) {
            if (!vVisited[vPred[u]]) {
                vVisited[vPred[u]] = true;
                iVisitedCount++;
                vStack.push_back(vPred[u]);
            }
        }
    }

    if (iVisitedCount != static_cast<int>(uCount)) {
        return true;
    }

    std::vector<bool> vForwardReach(uCount, false);
    std::vector<bool> vBackwardReach(uCount, false);

    // 从所有入度为零的节点正向 BFS
    std::queue<size_t> qForward;
    for (size_t u = 0; u < uCount; u++) {
        bool bHasPred = false;
        for (size_t d = 0; d < m_vDependencies.size(); d++) {
            if (static_cast<size_t>(m_vDependencies[d].GetSuccessorIndex()) == u) {
                bHasPred = true;
                break;
            }
        }
        if (!bHasPred) {
            qForward.push(u);
            vForwardReach[u] = true;
        }
    }

    while (!qForward.empty()) {
        size_t uNode = qForward.front();
        qForward.pop();
        std::vector<size_t> vSucc = GetSuccessorIndices(uNode);
        for (size_t u = 0; u < vSucc.size(); u++) {
            if (!vForwardReach[vSucc[u]]) {
                vForwardReach[vSucc[u]] = true;
                qForward.push(vSucc[u]);
            }
        }
    }

    // 从所有出度为零的节点反向 BFS
    std::queue<size_t> qBackward;
    for (size_t u = 0; u < uCount; u++) {
        bool bHasSucc = false;
        for (size_t d = 0; d < m_vDependencies.size(); d++) {
            if (static_cast<size_t>(m_vDependencies[d].GetPredecessorIndex()) == u) {
                bHasSucc = true;
                break;
            }
        }
        if (!bHasSucc) {
            qBackward.push(u);
            vBackwardReach[u] = true;
        }
    }

    while (!qBackward.empty()) {
        size_t uNode = qBackward.front();
        qBackward.pop();
        std::vector<size_t> vPred = GetPredecessorIndices(uNode);
        for (size_t u = 0; u < vPred.size(); u++) {
            if (!vBackwardReach[vPred[u]]) {
                vBackwardReach[vPred[u]] = true;
                qBackward.push(vPred[u]);
            }
        }
    }

    for (size_t u = 0; u < uCount; u++) {
        if (!vForwardReach[u] || !vBackwardReach[u]) {
            return true;
        }
    }
    return false;
}

bool Project::IsValid() const {
    if (HasInvalidDependencyReference()) {
        return false;
    }

    std::vector<size_t> vOrder;
    if (!ComputeTopologicalOrder(vOrder)) {
        return false;
    }

    if (HasIsolatedTask()) {
        return false;
    }

    return true;
}

int Project::ComputeEarliestStart(size_t uTaskIndex) const {
    int iMaxES = 0;
    bool bHasPred = false;

    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        if (static_cast<size_t>(m_vDependencies[u].GetSuccessorIndex()) != uTaskIndex) {
            continue;
        }

        bHasPred = true;
        size_t uPred = static_cast<size_t>(m_vDependencies[u].GetPredecessorIndex());
        const Task* pPred = m_vTasks[uPred].get();
        const Task* pSucc = m_vTasks[uTaskIndex].get();
        int iLag = m_vDependencies[u].GetLag();
        int iCandidate = 0;

        switch (m_vDependencies[u].GetType()) {
            case DependencyType::Type::FS:
                iCandidate = pPred->GetEF() + iLag;
                break;
            case DependencyType::Type::SS:
                iCandidate = pPred->GetES() + iLag;
                break;
            case DependencyType::Type::FF:
                iCandidate = (pPred->GetEF() + iLag) - pSucc->GetDuration();
                break;
            case DependencyType::Type::SF:
                iCandidate = (pPred->GetES() + iLag) - pSucc->GetDuration();
                break;
            default:
                iCandidate = 0;
                break;
        }

        if (iCandidate > iMaxES) {
            iMaxES = iCandidate;
        }
        bHasPred = true;
    }

    if (!bHasPred) {
        return 0;
    }
    return iMaxES;
}

void Project::ApplyForwardPass(const std::vector<size_t>& vOrder) const {
    for (size_t u = 0; u < vOrder.size(); u++) {
        size_t uIdx = vOrder[u];
        Task* pTask = m_vTasks[uIdx].get();
        pTask->ResetSchedule();

        int iES = ComputeEarliestStart(uIdx);
        pTask->SetES(iES);
        pTask->SetEF(iES + pTask->GetDuration());
    }
}

int Project::ComputeLatestFinish(size_t uTaskIndex, int iProjectDuration) const {
    int iMinLF = iProjectDuration;
    bool bHasSucc = false;

    for (size_t u = 0; u < m_vDependencies.size(); u++) {
        if (static_cast<size_t>(m_vDependencies[u].GetPredecessorIndex()) != uTaskIndex) {
            continue;
        }

        size_t uSucc = static_cast<size_t>(m_vDependencies[u].GetSuccessorIndex());
        const Task* pPred = m_vTasks[uTaskIndex].get();
        const Task* pSucc = m_vTasks[uSucc].get();
        int iLag = m_vDependencies[u].GetLag();
        int iCandidate = iProjectDuration;

        switch (m_vDependencies[u].GetType()) {
            case DependencyType::Type::FS:
                iCandidate = pSucc->GetLS() - iLag;
                break;
            case DependencyType::Type::SS:
                iCandidate = (pSucc->GetLS() - iLag) + pPred->GetDuration();
                break;
            case DependencyType::Type::FF:
                iCandidate = pSucc->GetLF() - iLag;
                break;
            case DependencyType::Type::SF:
                iCandidate = (pSucc->GetLF() - iLag) + pPred->GetDuration();
                break;
            default:
                iCandidate = iProjectDuration;
                break;
        }

        if (!bHasSucc || iCandidate < iMinLF) {
            iMinLF = iCandidate;
        }
        bHasSucc = true;
    }

    if (!bHasSucc) {
        return iProjectDuration;
    }
    return iMinLF;
}

void Project::ApplyBackwardPass(const std::vector<size_t>& vOrder,
                                int iProjectDuration) const {
    for (int i = static_cast<int>(vOrder.size()) - 1; i >= 0; i--) {
        size_t uIdx = vOrder[static_cast<size_t>(i)];
        Task* pTask = m_vTasks[uIdx].get();

        int iLF = ComputeLatestFinish(uIdx, iProjectDuration);
        pTask->SetLF(iLF);
        pTask->SetLS(iLF - pTask->GetDuration());
    }
}

bool Project::Inference(int& iTotalDuration,
                        std::vector<int>& vCriticalPathFileIds) const {
    vCriticalPathFileIds.clear();

    if (!IsValid()) {
        return false;
    }

    std::vector<size_t> vOrder;
    if (!ComputeTopologicalOrder(vOrder)) {
        return false;
    }

    // 重置并前向传播
    for (size_t u = 0; u < m_vTasks.size(); u++) {
        m_vTasks[u]->ResetSchedule();
    }
    ApplyForwardPass(vOrder);

    // 计算项目总工期
    iTotalDuration = 0;
    for (size_t u = 0; u < m_vTasks.size(); u++) {
        if (m_vTasks[u]->GetEF() > iTotalDuration) {
            iTotalDuration = m_vTasks[u]->GetEF();
        }
    }

    // 反向传播
    ApplyBackwardPass(vOrder, iTotalDuration);

    // 收集关键路径任务（EF == LF 或 ES == LS）
    for (size_t u = 0; u < vOrder.size(); u++) {
        size_t uIdx = vOrder[u];
        const Task* pTask = m_vTasks[uIdx].get();
        if ((pTask->GetEF() == pTask->GetLF()) || (pTask->GetES() == pTask->GetLS())) {
            vCriticalPathFileIds.push_back(pTask->GetFileId());
        }
    }

    m_iTotalDuration = iTotalDuration;
    m_bScheduleComputed = true;
    return true;
}

int Project::GetTotalDuration() const {
    if (!m_bScheduleComputed) {
        int iDuration = 0;
        std::vector<int> vCritical;
        Inference(iDuration, vCritical);
    }
    return m_iTotalDuration;
}
