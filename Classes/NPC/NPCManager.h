/****************************************************************
 * Project Name:  StardewValley
 * File Name:     NPCManager.cpp
 * File Function: NPCManager类的实现
 * Author:        赵睿妍、蔡锦慧
 * Update Date:   2025/12/24
 * License:       MIT License
 ****************************************************************/
#ifndef __NPC_MANAGER_H__
#define __NPC_MANAGER_H__

#include "cocos2d.h"
#include "Constant.h"
#include "Haley.h"
#include "Sam.h"
#include "Evelyn.h"

class NPCManager
{
public:
    // 获取单例
    static NPCManager* getInstance();
    // 销毁单例 (新增，防止内存泄漏)
    static void destroyInstance();

    // 获取所有NPC
    std::vector<NPCBase*> getAllNPCs();

    // 创建或获取已存在的NPC
    NPCBase* createNPC(const std::string& name);

    // 根据名字获取NPC
    NPCBase* getNPCByName(const std::string& name);

    void resumeAllNPCAnimations();

    void stopAllNPCAnimations();
private:
    // 私有构造
    NPCManager();
    // 私有析构 (在此处释放 map 中的对象)
    ~NPCManager();

private:
    static NPCManager* _instance;
    // 使用 std::map 管理对象，需要手动处理引用计数
    std::map<std::string, NPCBase*> _npcMap;
};

#endif // __NPC_MANAGER_H__