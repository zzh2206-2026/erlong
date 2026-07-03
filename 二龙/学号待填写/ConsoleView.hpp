//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 ConsoleView.hpp
//【功能模块和目的】         控制台界面类，提供菜单驱动的用户交互
//-------------------------------------------------------------------------------------------------------------------

#ifndef CONSOLE_VIEW_HPP
#define CONSOLE_VIEW_HPP

#include <string>

//-------------------------------------------------------------------------------------------------------------------
//【类名】             ConsoleView
//【功能】             项目调度器的控制台用户界面（MVC 中的 V）
//【接口说明】         负责显示菜单、接收输入、调用控制器
//-------------------------------------------------------------------------------------------------------------------
class ConsoleView {
public:
    ConsoleView();
    void Run();

private:
    void ShowMainMenu() const;
    void HandleImport() const;
    void HandleExport() const;
    void HandleListTasks() const;
    void HandleRemoveTask() const;
    void HandleAddTask() const;
    void HandleListTaskRelations() const;
    void HandleModifyTask() const;
    void HandleListDependencies() const;
    void HandleRemoveDependency() const;
    void HandleAddDependency() const;
    void HandleListResources() const;
    void HandleAddResource() const;
    void HandleAssignResource() const;
    void HandleStatistics() const;
    void HandleValidate() const;
    void HandleInference() const;

    void Pause() const;
    static std::string ReadLine();
    static int ReadInt(const std::string& strPrompt);
    static double ReadDouble(const std::string& strPrompt);
};

#endif
