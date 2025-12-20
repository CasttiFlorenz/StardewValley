#include "NPCManager.h"

NPCManager* NPCManager::_instance = nullptr;

NPCManager* NPCManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = new NPCManager();
    }
    return _instance;
}

void NPCManager::createAndAddNPCs(Scene* scene)
{
    if (!scene) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    Haley* haley = Haley::create();
    if (haley) {
        haley->setNPCPosition(Vec2(visibleSize.width * 0.25f, visibleSize.height * 0.5f));
        haley->setNPCScale(2.0f);
        scene->addChild(haley);
        _npcs.push_back(haley);
    }

    Sam* sam = Sam::create();
    if (sam) {
        sam->setNPCPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f));
        sam->setNPCScale(2.0f);
        scene->addChild(sam);
        _npcs.push_back(sam);
    }

    Evelyn* evelyn = Evelyn::create();
    if (evelyn) {
        evelyn->setNPCPosition(Vec2(visibleSize.width * 0.75f, visibleSize.height * 0.5f));
        evelyn->setNPCScale(2.0f);
        scene->addChild(evelyn);
        _npcs.push_back(evelyn);
    }
}

NPCBase* NPCManager::createNPC(const std::string& name) {
    NPCBase* npc = nullptr;
    if (name == "Haley") {
        npc = Haley::create();
        npc->setNPCName("Haley");
    }
    else if (name == "Sam") {
        npc = Sam::create();
        npc->setNPCName("Sam");
    }
    else if (name == "Evelyn") {
        npc = Evelyn::create();
        npc->setNPCName("Evelyn");
    }
    return npc;
}