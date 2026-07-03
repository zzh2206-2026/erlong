//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 PpmImporter.hpp
//【功能模块和目的】         PPM 格式项目文件导入器
//-------------------------------------------------------------------------------------------------------------------

#ifndef PPM_IMPORTER_HPP
#define PPM_IMPORTER_HPP

#include "Importer.hpp"

//-------------------------------------------------------------------------------------------------------------------
//【类名】             PpmImporter
//【功能】             解析 PPM 格式文件并构建 Project 对象
//【接口说明】         继承 Importer，实现 PPM 专用导入
//-------------------------------------------------------------------------------------------------------------------
class PpmImporter : public Importer {
public:
    bool Import(const std::string& strFileName, Project& project) const override;

private:
    static std::string Trim(const std::string& strLine);
    static bool ParseLine(const std::string& strLine, Project& project,
                          bool& bHasProjectLine);
};

#endif
