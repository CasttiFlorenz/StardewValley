/*********************************************************
* Project Name : StardewValley
* File Name : NPCManager.cpp
* File Function : NPCManager类的实现
* Author : 赵睿妍、蔡锦慧
* Update Date : 2025 / 12 / 20
* License : MIT License
* ***************************************************************/
#include "NPCManager.h"

NPCManager* NPCManager::_instance = nullptr;

NPCManager* NPCManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = new (std::nothrow) NPCManager();
    }
    return _instance;
}

void NPCManager::destroyInstance()
{
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

NPCManager::NPCManager()
{
}

NPCManager::~NPCManager(){
    // 对象生命周期管理: 释放 map 中持有的对象
    for (auto& pair : _npcMap) {
        NPCBase* npc = pair.second;
        if (npc != nullptr) {
            // 对应 createNPC 中的 retain()
            npc->release();
        }
    }
    _npcMap.clear();
}

std::vector<NPCBase*> NPCManager::getAllNPCs() {
    std::vector<NPCBase*> list;
    for (auto& pair : _npcMap) {
        if (pair.second != nullptr) {
            list.push_back(pair.second);
        }
    }
    return list;
}

NPCBase* NPCManager::getNPCByName(const std::string& name)
{
    auto it = _npcMap.find(name);
    if (it != _npcMap.end()) {
        return it->second;
    }
    return nullptr;
}

NPCBase* NPCManager::createNPC(const std::string& name) {
    // 1. 检查是否已存在
    auto it = _npcMap.find(name);
    if (it != _npcMap.end()) {
        NPCBase* existingNPC = it->second;
        // 如果需要从旧父节点移除以重新添加到新场景
        if (existingNPC && existingNPC->getParent()) {
            existingNPC->removeFromParent();
        }
        return existingNPC;
    }

    // 2. 创建新实例
    NPCBase* npc = nullptr;

    if (name == "Haley") {
        npc = Haley::create();
    }
    else if (name == "Sam") {
        npc = Sam::create();
    }
    else if (name == "Evelyn") {
        npc = Evelyn::create();
    }

    // 3. 存入 Map 并持有引用
    if (npc != nullptr) {
        npc->setNPCName(name);

        // retain() 增加引用计数，防止被自动释放池回收
        npc->retain();

        _npcMap[name] = npc;
    }

    return npc;
}