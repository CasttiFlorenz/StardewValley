#ifndef SAVEMANAGE_H
#define SAVEMANAGE_H

#include <string>
#include <vector>
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "Constant.h"
#include "../Inventory/InventoryScene.h"
#include "../NPC/SocialLevel.h"
#include "../Time/TimeManager.h"
#include "../Player/SkillLevel.h"

class SaveManage {
public:
    static SaveManage* getInstance();

    // 综合操作
    bool saveAllData();
    bool loadAllData();

private:
    SaveManage() = default;
    ~SaveManage() = default;
    SaveManage(const SaveManage&) = delete;
    SaveManage& operator=(const SaveManage&) = delete;

    // 工具函数
    std::string getFilePath(const std::string& filename);

    // 背包存档/读档
    bool saveInventory();
    bool loadInventory();

    // 好感度存档/读档
    bool saveFriendships();
    bool loadFriendships();

    // 游戏时间存档/读档
    bool saveGameTime();
    bool loadGameTime();

    // 技能存档/读档
    bool saveSkills();
    bool loadSkills();


    // 序列化函数
    rapidjson::Value serializeItem(const Item& item, rapidjson::Document::AllocatorType& alloc);
    rapidjson::Value serializeGameTime(const GameTime& time, rapidjson::Document::AllocatorType& alloc);
    rapidjson::Value serializeSkill(const SkillData& skill, rapidjson::Document::AllocatorType& alloc);
    bool deserializeGameTime(const rapidjson::Value& timeObj, GameTime& time);

};

#endif // SAVEMANAGE_H
