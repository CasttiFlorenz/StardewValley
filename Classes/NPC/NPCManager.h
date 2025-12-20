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

    const std::vector<NPCBase*>& getAllNPCs() const { return _npcs; }

    NPCBase* createNPC(const std::string& name);
private:
    NPCManager() {}
    ~NPCManager() {}

    static NPCManager* _instance;
    std::vector<NPCBase*> _npcs;
};

#endif // __NPC_MANAGER_H__