#pragma once
#ifndef __CULTIVATED_SOIL_H__
#define __CULTIVATED_SOIL_H__

#include "EnvironmentItem.h"
#include "Crop.h"

// 耕地（土壤）类：
// 负责维护土壤状态（干 / 湿）
// 并管理其上作物的创建、生长与销毁
class CultivatedSoil : public EnvironmentItem {
public:
    // 创建指定瓦片坐标上的耕地
    static CultivatedSoil* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);

    // 析构函数：确保作物节点被正确移除（防御式）
    virtual ~CultivatedSoil();

    // 浇水：将干燥土壤变为湿润
    void water();
    // 干燥：将湿润土壤恢复为干燥
    void dry();

    // 在该土壤上种植指定类型的作物（种子）
    bool plant(ItemType type);

    // 收获成熟作物，返回对应物品类型；若不可收获返回 NONE
    ItemType harvest();

    // 每日更新：推进作物生长，并重置土壤状态
    void updateDay();

    SoilStatus getStatus() const { return _status; }
    Crop* getCrop() const { return _crop; }
    bool hasCrop() const { return _crop != nullptr; }

private:
    SoilStatus _status; // 当前土壤状态（干 / 湿）
    Crop* _crop;        // 当前种植的作物（可能为空）
};

#endif // __CULTIVATED_SOIL_H__
