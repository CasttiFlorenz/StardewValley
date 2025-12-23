// NPCManager.h
#ifndef __NPC_MANAGER_H__
#define __NPC_MANAGER_H__

#include "cocos2d.h"
#include "Haley.h"
#include "Sam.h"
#include "Evelyn.h"
USING_NS_CC;

class NPCManager
{
public:
    static NPCManager* getInstance();

    void createAndAddNPCs(Scene* scene);

    void playAllAnimations();

    std::vector<NPCBase*> NPCManager::getAllNPCs();

    NPCBase* createNPC(const std::string& name);

    NPCBase* getNPCByName(const std::string& name);
private:
    NPCManager() {}
    ~NPCManager() {}
    std::map<std::string, NPCBase*> _npcMap;
    static NPCManager* _instance;

};

#endif // __NPC_MANAGER_H__