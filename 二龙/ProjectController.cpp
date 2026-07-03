//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 ProjectController.cpp
//【功能模块和目的】         业务流程控制器实现
//-------------------------------------------------------------------------------------------------------------------

#include "ProjectController.hpp"
#include "PpmImporter.hpp"
#include "PpmExporter.hpp"

#include <sstream>

ProjectController* ProjectController::s_pInstance = nullptr;

std::string ProjectController::TrimPath(const std::string& strPath) {
    size_t uStart = 0;
    while ((uStart < strPath.size())
        && (strPath[uStart] == ' ' || strPath[uStart] == '\t')) {
        uStart++;
    }

    size_t uEnd = strPath.size();
    while ((uEnd > uStart)
        && (strPath[uEnd - 1] == ' ' || strPath[uEnd - 1] == '\t'
        || strPath[uEnd - 1] == '\r')) {
        uEnd--;
    }

    std::string strResult = strPath.substr(uStart, uEnd - uStart);
    if ((strResult.size() >= 2)
        && ((strResult.front() == '"') && (strResult.back() == '"'))) {
        strResult = strResult.substr(1, strResult.size() - 2);
    }
    return strResult;
}

std::string ProjectController::GetBaseName(const std::string& strPath) {
    size_t uPos = strPath.find_last_of("\\/");
    if (uPos == std::string::npos) {
        return strPath;
    }
    return strPath.substr(uPos + 1);
}

ProjectController::ProjectController()
    : m_pImporter(new PpmImporter()),
      m_pExporter(new PpmExporter()),
      m_bLoaded(false) {
}

ProjectController::~ProjectController() {
}

ProjectController& ProjectController::GetInstance() {
    if (s_pInstance == nullptr) {
        s_pInstance = new ProjectController();
    }
    return *s_pInstance;
}

bool ProjectController::ImportProject(const std::string& strFileName) {
    if (m_pImporter == nullptr) {
        return false;
    }

    std::string strPath = TrimPath(strFileName);
    if (strPath.empty()) {
        strPath = "simple.PPM";
    }

    m_project.Clear();

    if (m_pImporter->Import(strPath, m_project)) {
        m_bLoaded = true;
        return true;
    }

    // 完整路径失败时，尝试仅使用文件名（程序与 PPM 同目录时可用）
    std::string strBaseName = GetBaseName(strPath);
    if ((strBaseName != strPath) && m_pImporter->Import(strBaseName, m_project)) {
        m_bLoaded = true;
        return true;
    }

    m_bLoaded = false;
    return false;
}

bool ProjectController::ExportProject(const std::string& strFileName) {
    if ((m_pExporter == nullptr) || !m_bLoaded) {
        return false;
    }
    return m_pExporter->Export(strFileName, m_project);
}

bool ProjectController::ListAllTasks(std::string& strOutput) const {
    if (!m_bLoaded) {
        strOutput = "当前无项目，请先导入或创建项目。\n";
        return false;
    }

    std::ostringstream oss;
    oss << "========== 任务列表 ==========\n";
    oss << "索引\t名称\t\t工期\t类型\t前置\t后继\n";
    oss << "--------------------------------------------------\n";

    for (size_t u = 0; u < m_project.GetTaskCount(); u++) {
        const Task* pTask = m_project.GetTask(u);
        if (pTask == nullptr) {
            continue;
        }

        oss << u << "\t" << pTask->GetName() << "\t\t"
            << pTask->GetDuration() << "\t"
            << pTask->GetTypeName() << "\t";

        // 前置任务
        std::vector<size_t> vPred = m_project.GetPredecessorIndices(u);
        for (size_t p = 0; p < vPred.size(); p++) {
            if (p > 0) {
                oss << ",";
            }
            oss << vPred[p];
        }
        oss << "\t";

        // 后继任务
        std::vector<size_t> vSucc = m_project.GetSuccessorIndices(u);
        for (size_t s = 0; s < vSucc.size(); s++) {
            if (s > 0) {
                oss << ",";
            }
            oss << vSucc[s];
        }
        oss << "\n";
    }

    strOutput = oss.str();
    return true;
}

bool ProjectController::RemoveTask(size_t uIndex) {
    if (!m_bLoaded) {
        return false;
    }
    return m_project.RemoveTask(uIndex);
}

bool ProjectController::AddTask(const std::string& strName, int iDuration, size_t& uOutIndex) {
    if (!m_bLoaded) {
        // 若无项目则创建空项目
        m_project.Clear();
        m_project.SetName("NewProject");
        m_bLoaded = true;
    }
    return m_project.AddTask(strName, iDuration, uOutIndex);
}

bool ProjectController::ListTaskRelations(size_t uIndex, std::string& strOutput) const {
    if (!m_bLoaded || uIndex >= m_project.GetTaskCount()) {
        strOutput = "无效的任务索引。\n";
        return false;
    }

    std::ostringstream oss;
    const Task* pTask = m_project.GetTask(uIndex);
    oss << "任务 [" << uIndex << "] " << pTask->GetName() << "\n";

    oss << "前驱任务：";
    std::vector<size_t> vPred = m_project.GetPredecessorIndices(uIndex);
    if (vPred.empty()) {
        oss << "无";
    }
    else {
        for (size_t u = 0; u < vPred.size(); u++) {
            const Task* pPred = m_project.GetTask(vPred[u]);
            if (u > 0) {
                oss << ", ";
            }
            oss << "[" << vPred[u] << "] " << pPred->GetName();
        }
    }
    oss << "\n";

    oss << "后继任务：";
    std::vector<size_t> vSucc = m_project.GetSuccessorIndices(uIndex);
    if (vSucc.empty()) {
        oss << "无";
    }
    else {
        for (size_t u = 0; u < vSucc.size(); u++) {
            const Task* pSucc = m_project.GetTask(vSucc[u]);
            if (u > 0) {
                oss << ", ";
            }
            oss << "[" << vSucc[u] << "] " << pSucc->GetName();
        }
    }
    oss << "\n";

    strOutput = oss.str();
    return true;
}

bool ProjectController::ModifyTask(size_t uIndex, const std::string& strName, int iDuration) {
    if (!m_bLoaded) {
        return false;
    }
    return m_project.ModifyTask(uIndex, strName, iDuration);
}

bool ProjectController::ListAllDependencies(std::string& strOutput) const {
    if (!m_bLoaded) {
        strOutput = "当前无项目。\n";
        return false;
    }

    std::ostringstream oss;
    oss << "========== 依赖列表 ==========\n";
    oss << "序号\t前置索引\t后置索引\t类型\tLag\n";
    oss << "--------------------------------------------------\n";

    for (size_t u = 0; u < m_project.GetDependencyCount(); u++) {
        const Dependency& dep = m_project.GetDependency(u);
        oss << u << "\t" << dep.GetPredecessorIndex() << "\t\t"
            << dep.GetSuccessorIndex() << "\t\t"
            << DependencyType::ToString(dep.GetType()) << "\t"
            << dep.GetLag() << "\n";
    }

    strOutput = oss.str();
    return true;
}

bool ProjectController::RemoveDependency(size_t uIndex) {
    if (!m_bLoaded) {
        return false;
    }
    return m_project.RemoveDependency(uIndex);
}

bool ProjectController::AddDependency(size_t uPredIndex, size_t uSuccIndex,
                                      const std::string& strType, int iLag) {
    if (!m_bLoaded) {
        return false;
    }

    DependencyType::Type eType = DependencyType::Type::FS;
    if (!DependencyType::FromString(strType, eType)) {
        return false;
    }

    size_t uOutIndex = 0;
    return m_project.AddDependency(uPredIndex, uSuccIndex, eType, iLag, uOutIndex);
}

bool ProjectController::ListAllResources(std::string& strOutput) const {
    if (!m_bLoaded) {
        strOutput = "当前无项目。\n";
        return false;
    }

    std::ostringstream oss;
    oss << "========== 资源列表 ==========\n";
    oss << "索引\t名称\t\t单位成本\n";
    oss << "--------------------------------------------------\n";

    for (size_t u = 0; u < m_project.GetResourceCount(); u++) {
        const Resource& res = m_project.GetResource(u);
        oss << u << "\t" << res.GetName() << "\t\t"
            << res.GetUnitCost() << "\n";
    }

    strOutput = oss.str();
    return true;
}

bool ProjectController::AddResource(const std::string& strName, double rUnitCost) {
    if (!m_bLoaded) {
        m_project.Clear();
        m_project.SetName("NewProject");
        m_bLoaded = true;
    }
    size_t uIndex = 0;
    return m_project.AddResource(strName, rUnitCost, uIndex);
}

bool ProjectController::AssignResource(size_t uTaskIndex, size_t uResourceIndex, int iQuantity) {
    if (!m_bLoaded) {
        return false;
    }
    return m_project.AssignResourceToTask(uTaskIndex, uResourceIndex, iQuantity);
}

bool ProjectController::ShowStatistics(std::string& strOutput) const {
    if (!m_bLoaded) {
        strOutput = "当前无项目。\n";
        return false;
    }

    std::ostringstream oss;
    oss << "========== 项目统计 ==========\n";
    oss << "项目名称：" << m_project.GetName() << "\n";
    oss << "任务总数：" << m_project.GetTaskCount() << "\n";
    oss << "依赖总数：" << m_project.GetDependencyCount() << "\n";
    oss << "资源总数：" << m_project.GetResourceCount() << "\n";
    oss << "总工期（关键路径长度）：" << m_project.GetTotalDuration() << " 天\n";

    strOutput = oss.str();
    return true;
}

bool ProjectController::ValidateProject(std::string& strOutput) const {
    if (!m_bLoaded) {
        strOutput = "当前无项目。\n";
        return false;
    }

    bool bValid = m_project.IsValid();
    std::ostringstream oss;
    oss << "项目验证结果：" << (bValid ? "合理（IsValid = true）" : "不合理（IsValid = false）") << "\n";

    if (!bValid) {
        oss << "可能原因：依赖图存在环、存在孤立节点、或依赖引用了不存在的任务。\n";
    }

    strOutput = oss.str();
    return bValid;
}

bool ProjectController::RunInference(std::string& strOutput) const {
    if (!m_bLoaded) {
        strOutput = "当前无项目。\n";
        return false;
    }

    int iTotalDuration = 0;
    std::vector<int> vCriticalPath;
    bool bOk = m_project.Inference(iTotalDuration, vCriticalPath);

    std::ostringstream oss;
    if (!bOk) {
        oss << "调度计算失败：项目不合理，请先验证项目。\n";
        strOutput = oss.str();
        return false;
    }

    oss << "========== CPM 关键路径计算结果 ==========\n";
    oss << "项目总工期：" << iTotalDuration << " 天\n\n";

    oss << "各任务调度参数：\n";
    oss << "索引\t名称\t\tES\tEF\tLS\tLF\n";
    for (size_t u = 0; u < m_project.GetTaskCount(); u++) {
        const Task* pTask = m_project.GetTask(u);
        oss << u << "\t" << pTask->GetName() << "\t\t"
            << pTask->GetES() << "\t" << pTask->GetEF() << "\t"
            << pTask->GetLS() << "\t" << pTask->GetLF() << "\n";
    }

    oss << "\n关键路径任务 ID（按拓扑顺序）：";
    for (size_t u = 0; u < vCriticalPath.size(); u++) {
        if (u > 0) {
            oss << " -> ";
        }
        oss << vCriticalPath[u];
    }
    oss << "\n";

    strOutput = oss.str();
    return true;
}

const Project& ProjectController::GetProject() const {
    return m_project;
}

bool ProjectController::HasProject() const {
    return m_bLoaded;
}
