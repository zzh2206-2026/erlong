//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 Task.cpp
//【功能模块和目的】         任务抽象基类实现
//-------------------------------------------------------------------------------------------------------------------

#include "Task.hpp"

Task::Task(const std::string& strName, int iDuration, int iFileId)
    : m_strName(strName),
      m_iDuration(iDuration),
      m_iFileId(iFileId),
      m_iES(0),
      m_iEF(0),
      m_iLS(0),
      m_iLF(0) {
}

const std::string& Task::GetName() const {
    return m_strName;
}

void Task::SetName(const std::string& strName) {
    m_strName = strName;
}

int Task::GetDuration() const {
    return m_iDuration;
}

void Task::SetDuration(int iDuration) {
    m_iDuration = iDuration;
}

int Task::GetFileId() const {
    return m_iFileId;
}

void Task::SetFileId(int iFileId) {
    m_iFileId = iFileId;
}

int Task::GetES() const {
    return m_iES;
}

int Task::GetEF() const {
    return m_iEF;
}

int Task::GetLS() const {
    return m_iLS;
}

int Task::GetLF() const {
    return m_iLF;
}

void Task::SetES(int iES) {
    m_iES = iES;
}

void Task::SetEF(int iEF) {
    m_iEF = iEF;
}

void Task::SetLS(int iLS) {
    m_iLS = iLS;
}

void Task::SetLF(int iLF) {
    m_iLF = iLF;
}

void Task::ResetSchedule() const {
    m_iES = 0;
    m_iEF = 0;
    m_iLS = 0;
    m_iLF = 0;
}
