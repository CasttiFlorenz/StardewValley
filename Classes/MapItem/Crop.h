/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Crop.h
 * File Function: Crop类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#pragma once
#ifndef __CROP_H__
#define __CROP_H__

#include "cocos2d.h"
#include "Constant.h"
#include "../Time/GameTime.h"

USING_NS_CC;

// 作物基类
class Crop : public Sprite {
public:
    // 静态创建函数
    static Crop* create(ItemType type);

    // 初始化
    virtual bool init(ItemType type);

    // 更新生长状态（每日调用）
    void updateGrowth(bool isWatered);

    // 获取作物信息
    ItemType getCropType() const noexcept { return _type; }
    CropStatus getStatus() const noexcept { return _status; }
    int getGrowthStage() const noexcept { return _growthStage; }

    // 设置生长阶段
    void setGrowthStage(int stage);

    // 设置生长阶段和状态
    void setGrowthStage(int stage, CropStatus status);

protected:
    // 更新纹理显示
    void updateTexture();

protected:
    ItemType _type;             // 作物类型
    CropStatus _status;         // 生长状态
    int _growthStage;           // 当前阶段
    int _maxGrowthStage;        // 最大阶段（成熟）
    std::string _texturePrefix; // 纹理前缀
};

// 防风草
class Parsnip : public Crop {
public:
    static Parsnip* create();
    virtual bool init() override;
};

// 土豆
class Potato : public Crop {
public:
    static Potato* create();
    virtual bool init() override;
};

// 花椰菜
class Cauliflower : public Crop {
public:
    static Cauliflower* create();
    virtual bool init() override;
};

#endif // __CROP_H__
