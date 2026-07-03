//-------------------------------------------------------------------------------------------------------------------
//【文件名】                 DependencyType.cpp
//【功能模块和目的】         依赖类型枚举的工具函数实现
//-------------------------------------------------------------------------------------------------------------------

#include "DependencyType.hpp"

std::string DependencyType::ToString(Type type) {
    switch (type) {
        case Type::FS:
            return "FS";
        case Type::SS:
            return "SS";
        case Type::FF:
            return "FF";
        case Type::SF:
            return "SF";
        default:
            return "FS";
    }
}

bool DependencyType::FromString(const std::string& strType, Type& type) {
    if (strType == "FS") {
        type = Type::FS;
        return true;
    }
    if (strType == "SS") {
        type = Type::SS;
        return true;
    }
    if (strType == "FF") {
        type = Type::FF;
        return true;
    }
    if (strType == "SF") {
        type = Type::SF;
        return true;
    }
    return false;
}
