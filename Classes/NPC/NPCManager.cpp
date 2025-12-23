#include "NPCManager.h"

NPCManager* NPCManager::_instance = nullptr;

NPCManager* NPCManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = new NPCManager();
    }
    return _instance;
}

std::vector<NPCBase*> NPCManager::getAllNPCs() {
    std::vector<NPCBase*> list;
    for (auto& pair : _npcMap) {
        if (pair.second) {
            list.push_back(pair.second);
        }
    }
    return list;
}

NPCBase* NPCManager::getNPCByName(const std::string& name)
{
    if (_npcMap.find(name) != _npcMap.end()) {
        return _npcMap[name];
    }
    return nullptr;
}

NPCBase* NPCManager::createNPC(const std::string& name) {

    if (_npcMap.find(name) != _npcMap.end()) {
        NPCBase* existingNPC = _npcMap[name];
        if (existingNPC->getParent()) {
            existingNPC->removeFromParent();
        }

        return existingNPC; 
    }

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

    if (npc) {
        npc->setNPCName(name);
        npc->retain();

        _npcMap[name] = npc;
    }

    return npc;
}
