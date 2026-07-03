//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 ConsoleView.cpp
//【功能模块和目的】         控制台界面类实现
//-------------------------------------------------------------------------------------------------------------------

#include "ConsoleView.hpp"
#include "ProjectController.hpp"

#include <iostream>
#include <limits>
#include <sstream>

ConsoleView::ConsoleView() {
}

void ConsoleView::ShowMainMenu() const {
    std::cout << "\n========================================\n";
    std::cout << "     项目调度器 Project Scheduler\n";
    std::cout << "========================================\n";
    std::cout << "  1.  导入 PPM 项目文件\n";
    std::cout << "  2.  导出 PPM 项目文件\n";
    std::cout << "  3.  列出所有任务\n";
    std::cout << "  4.  删除任务\n";
    std::cout << "  5.  添加任务\n";
    std::cout << "  6.  查看任务前驱/后继\n";
    std::cout << "  7.  修改任务\n";
    std::cout << "  8.  列出所有依赖\n";
    std::cout << "  9.  删除依赖\n";
    std::cout << " 10.  添加依赖\n";
    std::cout << " 11.  列出所有资源\n";
    std::cout << " 12.  添加资源\n";
    std::cout << " 13.  为任务分配资源\n";
    std::cout << " 14.  显示统计信息\n";
    std::cout << " 15.  验证项目合理性\n";
    std::cout << " 16.  执行关键路径计算\n";
    std::cout << "  0.  退出\n";
    std::cout << "========================================\n";
    std::cout << "请选择操作：";
}

std::string ConsoleView::ReadLine() {
    std::string strLine;
    std::getline(std::cin, strLine);
    return strLine;
}

int ConsoleView::ReadInt(const std::string& strPrompt) {
    std::cout << strPrompt;
    std::string strLine = ReadLine();
    std::istringstream iss(strLine);
    int iValue = 0;
    iss >> iValue;
    return iValue;
}

double ConsoleView::ReadDouble(const std::string& strPrompt) {
    std::cout << strPrompt;
    std::string strLine = ReadLine();
    std::istringstream iss(strLine);
    double rValue = 0.0;
    iss >> rValue;
    return rValue;
}

void ConsoleView::Pause() const {
    std::cout << "\n按回车键继续...";
    std::string strDummy;
    std::getline(std::cin, strDummy);
}

void ConsoleView::HandleImport() const {
    std::cout << "请输入 PPM 文件路径（推荐直接输入 simple.PPM）：";
    std::string strPath = ReadLine();
    ProjectController& controller = ProjectController::GetInstance();

    if (controller.ImportProject(strPath)) {
        std::cout << "导入成功！\n";
    }
    else {
        std::cout << "导入失败。\n";
        std::cout << "提示：若使用完整路径且含中文，请改为只输入 simple.PPM。\n";
    }
}

void ConsoleView::HandleExport() const {
    ProjectController& controller = ProjectController::GetInstance();
    if (!controller.HasProject()) {
        std::cout << "当前无项目可导出。\n";
        return;
    }

    std::cout << "请输入导出文件路径：";
    std::string strPath = ReadLine();

    if (controller.ExportProject(strPath)) {
        std::cout << "导出成功！\n";
    }
    else {
        std::cout << "导出失败。\n";
    }
}

void ConsoleView::HandleListTasks() const {
    std::string strOutput;
    ProjectController& controller = ProjectController::GetInstance();
    if (controller.ListAllTasks(strOutput)) {
        std::cout << strOutput;
    }
    else {
        std::cout << strOutput;
    }
}

void ConsoleView::HandleRemoveTask() const {
    int iIndex = ReadInt("请输入要删除的任务索引：");
    ProjectController& controller = ProjectController::GetInstance();

    if (controller.RemoveTask(static_cast<size_t>(iIndex))) {
        std::cout << "删除成功。\n";
    }
    else {
        std::cout << "删除失败，索引无效。\n";
    }
}

void ConsoleView::HandleAddTask() const {
    std::cout << "请输入任务名称：";
    std::string strName = ReadLine();
    int iDuration = ReadInt("请输入任务工期（天，0 表示里程碑）：");

    size_t uIndex = 0;
    ProjectController& controller = ProjectController::GetInstance();

    if (controller.AddTask(strName, iDuration, uIndex)) {
        std::cout << "添加成功，任务索引为 " << uIndex << "。\n";
    }
    else {
        std::cout << "添加失败，名称可能重复。\n";
    }
}

void ConsoleView::HandleListTaskRelations() const {
    int iIndex = ReadInt("请输入任务索引：");
    std::string strOutput;
    ProjectController& controller = ProjectController::GetInstance();

    if (controller.ListTaskRelations(static_cast<size_t>(iIndex), strOutput)) {
        std::cout << strOutput;
    }
    else {
        std::cout << strOutput;
    }
}

void ConsoleView::HandleModifyTask() const {
    int iIndex = ReadInt("请输入要修改的任务索引：");
    std::cout << "请输入新名称：";
    std::string strName = ReadLine();
    int iDuration = ReadInt("请输入新工期（天）：");

    ProjectController& controller = ProjectController::GetInstance();
    if (controller.ModifyTask(static_cast<size_t>(iIndex), strName, iDuration)) {
        std::cout << "修改成功。\n";
    }
    else {
        std::cout << "修改失败。\n";
    }
}

void ConsoleView::HandleListDependencies() const {
    std::string strOutput;
    ProjectController& controller = ProjectController::GetInstance();
    controller.ListAllDependencies(strOutput);
    std::cout << strOutput;
}

void ConsoleView::HandleRemoveDependency() const {
    int iIndex = ReadInt("请输入要删除的依赖序号：");
    ProjectController& controller = ProjectController::GetInstance();

    if (controller.RemoveDependency(static_cast<size_t>(iIndex))) {
        std::cout << "删除成功。\n";
    }
    else {
        std::cout << "删除失败。\n";
    }
}

void ConsoleView::HandleAddDependency() const {
    int iPred = ReadInt("请输入前置任务索引：");
    int iSucc = ReadInt("请输入后置任务索引：");
    std::cout << "请输入依赖类型（FS/SS/FF/SF）：";
    std::string strType = ReadLine();
    int iLag = ReadInt("请输入 Lag 值：");

    ProjectController& controller = ProjectController::GetInstance();
    if (controller.AddDependency(static_cast<size_t>(iPred),
        static_cast<size_t>(iSucc), strType, iLag)) {
        std::cout << "添加成功。\n";
    }
    else {
        std::cout << "添加失败（可能重复、产生环或索引无效）。\n";
    }
}

void ConsoleView::HandleListResources() const {
    std::string strOutput;
    ProjectController& controller = ProjectController::GetInstance();
    controller.ListAllResources(strOutput);
    std::cout << strOutput;
}

void ConsoleView::HandleAddResource() const {
    std::cout << "请输入资源名称：";
    std::string strName = ReadLine();
    double rCost = ReadDouble("请输入单位成本：");

    ProjectController& controller = ProjectController::GetInstance();
    if (controller.AddResource(strName, rCost)) {
        std::cout << "添加成功。\n";
    }
    else {
        std::cout << "添加失败，名称可能重复。\n";
    }
}

void ConsoleView::HandleAssignResource() const {
    int iTask = ReadInt("请输入任务索引：");
    int iRes = ReadInt("请输入资源索引：");
    int iQty = ReadInt("请输入占用数量：");

    ProjectController& controller = ProjectController::GetInstance();
    if (controller.AssignResource(static_cast<size_t>(iTask),
        static_cast<size_t>(iRes), iQty)) {
        std::cout << "分配成功。\n";
    }
    else {
        std::cout << "分配失败（里程碑不可分配资源或索引无效）。\n";
    }
}

void ConsoleView::HandleStatistics() const {
    std::string strOutput;
    ProjectController& controller = ProjectController::GetInstance();
    controller.ShowStatistics(strOutput);
    std::cout << strOutput;
}

void ConsoleView::HandleValidate() const {
    std::string strOutput;
    ProjectController& controller = ProjectController::GetInstance();
    controller.ValidateProject(strOutput);
    std::cout << strOutput;
}

void ConsoleView::HandleInference() const {
    std::string strOutput;
    ProjectController& controller = ProjectController::GetInstance();
    if (controller.RunInference(strOutput)) {
        std::cout << strOutput;
    }
    else {
        std::cout << strOutput;
    }
}

void ConsoleView::Run() {
    bool bRunning = true;

    while (bRunning) {
        ShowMainMenu();
        int iChoice = ReadInt("");

        switch (iChoice) {
            case 1:
                HandleImport();
                break;
            case 2:
                HandleExport();
                break;
            case 3:
                HandleListTasks();
                break;
            case 4:
                HandleRemoveTask();
                break;
            case 5:
                HandleAddTask();
                break;
            case 6:
                HandleListTaskRelations();
                break;
            case 7:
                HandleModifyTask();
                break;
            case 8:
                HandleListDependencies();
                break;
            case 9:
                HandleRemoveDependency();
                break;
            case 10:
                HandleAddDependency();
                break;
            case 11:
                HandleListResources();
                break;
            case 12:
                HandleAddResource();
                break;
            case 13:
                HandleAssignResource();
                break;
            case 14:
                HandleStatistics();
                break;
            case 15:
                HandleValidate();
                break;
            case 16:
                HandleInference();
                break;
            case 0:
                bRunning = false;
                std::cout << "感谢使用，再见！\n";
                break;
            default:
                std::cout << "无效选择，请重新输入。\n";
                break;
        }

        if (bRunning) {
            Pause();
        }
    }
}
