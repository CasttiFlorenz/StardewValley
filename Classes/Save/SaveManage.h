/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SaveManage.h
 * File Function: SaveManage类的实现
 * Author:        于恩熙、郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
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
#include "../MapItem/BarnManager.h"
#include "../MapItem/CultivationManager.h"
#include "../MapItem/FarmItemManager.h"
#include "../MapItem/MinesItemManager.h"

// 存档管理器（单例）
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

    // 获取文件完整路径
    std::string getFilePath(const std::string& filename);

    // 物品栏数据
    bool saveInventory();
    bool loadInventory();

    // 好感度数据
    bool saveFriendships();
    bool loadFriendships();

    // 游戏时间、天气、金钱
    bool saveGameConditions();
    bool loadGameConditions();

    // 技能数据
    bool saveSkills();
    bool loadSkills();

    // 畜棚数据
    bool saveBarnData();
    bool loadBarnData();

    // 农场数据
    bool saveFarmData();
    bool loadFarmData();

    // 矿洞数据
    bool saveMinesData();
    bool loadMinesData();

    // 序列化辅助
    rapidjson::Value serializeItem(const Item& item, rapidjson::Document::AllocatorType& alloc);
    rapidjson::Value serializeGameTime(const GameTime& time, rapidjson::Document::AllocatorType& alloc);
    rapidjson::Value serializeSkill(const SkillData& skill, rapidjson::Document::AllocatorType& alloc);

    // 反序列化辅助
    bool deserializeGameTime(const rapidjson::Value& timeObj, GameTime& time);
};


#endif // SAVEMANAGE_H
