//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 ProjectController.hpp
//【功能模块和目的】         业务流程控制器，单例模式，连接界面与模型
//-------------------------------------------------------------------------------------------------------------------

#ifndef PROJECT_CONTROLLER_HPP
#define PROJECT_CONTROLLER_HPP

#include "Project.hpp"
#include "Importer.hpp"
#include "Exporter.hpp"

#include <memory>
#include <string>
#include <vector>

//-------------------------------------------------------------------------------------------------------------------
//【类名】             ProjectController
//【功能】             项目调度器的业务流程控制器（MVC 中的 C）
//【接口说明】         单例模式，封装所有业务操作供界面层调用
//-------------------------------------------------------------------------------------------------------------------
class ProjectController {
public:
    //-------------------------------------------------------------------------------------------------------------------
    //【函数名称】       GetInstance
    //【函数功能】       获取控制器单例实例
    //【参数】           无
    //【返回值】         控制器引用
    //-------------------------------------------------------------------------------------------------------------------
    static ProjectController& GetInstance();

    // 禁止拷贝与赋值
    ProjectController(const ProjectController&) = delete;
    ProjectController& operator=(const ProjectController&) = delete;

    // 文件操作
    bool ImportProject(const std::string& strFileName);
    bool ExportProject(const std::string& strFileName);

    // 任务操作
    bool ListAllTasks(std::string& strOutput) const;
    bool RemoveTask(size_t uIndex);
    bool AddTask(const std::string& strName, int iDuration, size_t& uOutIndex);
    bool ListTaskRelations(size_t uIndex, std::string& strOutput) const;
    bool ModifyTask(size_t uIndex, const std::string& strName, int iDuration);

    // 依赖操作
    bool ListAllDependencies(std::string& strOutput) const;
    bool RemoveDependency(size_t uIndex);
    bool AddDependency(size_t uPredIndex, size_t uSuccIndex,
                       const std::string& strType, int iLag);

    // 资源操作
    bool ListAllResources(std::string& strOutput) const;
    bool AddResource(const std::string& strName, double rUnitCost);
    bool AssignResource(size_t uTaskIndex, size_t uResourceIndex, int iQuantity);

    // 统计与调度
    bool ShowStatistics(std::string& strOutput) const;
    bool ValidateProject(std::string& strOutput) const;
    bool RunInference(std::string& strOutput) const;

    // 项目访问
    const Project& GetProject() const;
    bool HasProject() const;

private:
    ProjectController();
    ~ProjectController();

    static std::string TrimPath(const std::string& strPath);
    static std::string GetBaseName(const std::string& strPath);

    static ProjectController* s_pInstance;

    Project m_project;                          // 当前项目模型
    std::unique_ptr<Importer> m_pImporter;      // 导入器（多态）
    std::unique_ptr<Exporter> m_pExporter;      // 导出器（多态）
    bool m_bLoaded;                             // 是否已加载项目
};

#endif
