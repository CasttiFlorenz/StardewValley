#pragma once
#ifndef __CULTIVATED_SOIL_H__
#define __CULTIVATED_SOIL_H__

#include "EnvironmentItem.h"
#include "Crop.h"

// 耕地类（维护干燥/湿润状态，管理作物）
class CultivatedSoil : public EnvironmentItem {
public:
    // 静态创建函数
    static CultivatedSoil* create(const cocos2d::Vec2& tileCoord);

    // 初始化
    bool init(const cocos2d::Vec2& tileCoord);

    // 析构函数
    virtual ~CultivatedSoil();

    // 浇水（变湿）
    void water();

    // 变干
    void dry();

    // 种植作物
    bool plant(ItemType type);

    // 收获作物
    ItemType harvest();

    // 每日更新
    void updateDay();

    // 获取状态与作物
    SoilStatus getStatus() const noexcept { return _status; }
    Crop* getCrop() const noexcept { return _crop; }
    bool hasCrop() const noexcept { return _crop != nullptr; }

    // 设置土壤状态
    void setStatus(SoilStatus status);

private:
    SoilStatus _status; // 土壤状态
    Crop* _crop;        // 当前作物
};

#endif // __CULTIVATED_SOIL_H__