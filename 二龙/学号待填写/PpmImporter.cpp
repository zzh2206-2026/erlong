//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 PpmImporter.cpp
//【功能模块和目的】         PPM 格式导入器实现
//-------------------------------------------------------------------------------------------------------------------

#include "PpmImporter.hpp"
#include "Project.hpp"
#include "DependencyType.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

std::string PpmImporter::Trim(const std::string& strLine) {
    std::string strResult = strLine;

    // 去除 UTF-8 BOM
    if ((strResult.size() >= 3)
        && (static_cast<unsigned char>(strResult[0]) == 0xEF)
        && (static_cast<unsigned char>(strResult[1]) == 0xBB)
        && (static_cast<unsigned char>(strResult[2]) == 0xBF)) {
        strResult = strResult.substr(3);
    }

    size_t uStart = 0;
    while ((uStart < strResult.size())
        && (strResult[uStart] == ' ' || strResult[uStart] == '\t')) {
        uStart++;
    }

    size_t uEnd = strResult.size();
    while ((uEnd > uStart) && (strResult[uEnd - 1] == ' ' || strResult[uEnd - 1] == '\t'
        || strResult[uEnd - 1] == '\r')) {
        uEnd--;
    }

    return strResult.substr(uStart, uEnd - uStart);
}

bool PpmImporter::ParseLine(const std::string& strLine, Project& project,
                            bool& bHasProjectLine) {
    if (strLine.empty()) {
        return true;
    }

    // 跳过注释行
    if (strLine[0] == '#') {
        return true;
    }

    std::istringstream iss(strLine);
    char cType = '\0';
    iss >> cType;

    if (cType == 'P') {
        std::string strName;
        iss >> strName;
        project.SetName(strName);
        bHasProjectLine = true;
        return true;
    }

    if (cType == 'T') {
        int iId = 0;
        std::string strName;
        int iDuration = 0;
        iss >> iId >> strName >> iDuration;
        size_t uIndex = 0;
        return project.AddTaskWithFileId(strName, iDuration, iId, false, uIndex);
    }

    if (cType == 'M') {
        int iId = 0;
        std::string strName;
        int iDuration = 0;
        iss >> iId >> strName >> iDuration;
        size_t uIndex = 0;
        return project.AddTaskWithFileId(strName, 0, iId, true, uIndex);
    }

    if (cType == 'R') {
        int iId = 0;
        std::string strName;
        double rCost = 0.0;
        iss >> iId >> strName >> rCost;
        size_t uIndex = 0;
        return project.AddResourceWithFileId(strName, rCost, iId, uIndex);
    }

    if (cType == 'D') {
        int iPredId = 0;
        int iSuccId = 0;
        std::string strType;
        int iLag = 0;
        iss >> iPredId >> iSuccId >> strType >> iLag;
        DependencyType::Type eType = DependencyType::Type::FS;
        if (!DependencyType::FromString(strType, eType)) {
            return false;
        }
        return project.AddDependencyByFileId(iPredId, iSuccId, eType, iLag);
    }

    if (cType == 'A') {
        int iTaskId = 0;
        int iResId = 0;
        int iQty = 0;
        iss >> iTaskId >> iResId >> iQty;
        return project.AssignResourceByFileId(iTaskId, iResId, iQty);
    }

    return false;
}

bool PpmImporter::Import(const std::string& strFileName, Project& project) const {
    std::ifstream inFile;
    inFile.open(std::filesystem::path(strFileName));
    if (!inFile.is_open()) {
        return false;
    }

    project.Clear();
    bool bHasProjectLine = false;
    std::string strLine;

    while (std::getline(inFile, strLine)) {
        strLine = Trim(strLine);
        if (strLine.empty()) {
            continue;
        }
        if (!ParseLine(strLine, project, bHasProjectLine)) {
            project.Clear();
            return false;
        }
    }

    if (!bHasProjectLine) {
        project.Clear();
        return false;
    }

    return true;
}
