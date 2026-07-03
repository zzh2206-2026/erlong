//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 PpmExporter.cpp
//【功能模块和目的】         PPM 格式导出器实现
//-------------------------------------------------------------------------------------------------------------------

#include "PpmExporter.hpp"
#include "Project.hpp"

#include <fstream>
#include <iomanip>
#include <filesystem>

bool PpmExporter::Export(const std::string& strFileName, const Project& project) const {
    std::ofstream outFile;
    outFile.open(std::filesystem::path(strFileName));
    if (!outFile.is_open()) {
        return false;
    }

    // 写入项目头
    outFile << "P " << project.GetName() << "\n";

    // 写入任务
    const auto& vTasks = project.GetTasks();
    for (size_t u = 0; u < vTasks.size(); u++) {
        const Task* pTask = vTasks[u].get();
        if (pTask->IsMilestone()) {
            outFile << "M " << pTask->GetFileId() << " " << pTask->GetName() << " 0\n";
        }
        else {
            outFile << "T " << pTask->GetFileId() << " " << pTask->GetName()
                 << " " << pTask->GetDuration() << "\n";
        }
    }

    // 写入资源
    const auto& vResources = project.GetResources();
    for (size_t u = 0; u < vResources.size(); u++) {
        outFile << "R " << vResources[u].GetFileId() << " " << vResources[u].GetName()
             << " " << std::fixed << std::setprecision(1)
             << vResources[u].GetUnitCost() << "\n";
    }

    // 写入依赖
    const auto& vDeps = project.GetDependencies();
    for (size_t u = 0; u < vDeps.size(); u++) {
        outFile << "D " << vTasks[static_cast<size_t>(vDeps[u].GetPredecessorIndex())]->GetFileId()
             << " " << vTasks[static_cast<size_t>(vDeps[u].GetSuccessorIndex())]->GetFileId()
             << " " << DependencyType::ToString(vDeps[u].GetType())
             << " " << vDeps[u].GetLag() << "\n";
    }

    // 写入资源分配
    const auto& vAssigns = project.GetAssignments();
    for (size_t u = 0; u < vAssigns.size(); u++) {
        int iTaskFileId = vTasks[static_cast<size_t>(vAssigns[u].GetTaskIndex())]->GetFileId();
        int iResFileId = vResources[static_cast<size_t>(vAssigns[u].GetResourceIndex())].GetFileId();
        outFile << "A " << iTaskFileId << " " << iResFileId
             << " " << vAssigns[u].GetQuantity() << "\n";
    }

    return true;
}
