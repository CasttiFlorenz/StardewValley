#pragma once
#ifndef __CROP_H__
#define __CROP_H__

#include "cocos2d.h"
#include "Constant.h"
#include "../Time/GameTime.h"

USING_NS_CC;

class Crop : public Sprite {
public:
    // 创建指定类型的作物（使用物品枚举中的作物类型）
    static Crop* create(ItemType type);
    // 初始化作物的基本状态（种子阶段、成长阶段等）
    virtual bool init(ItemType type);

    // 根据是否浇水更新作物成长状态（每天调用一次）
    void updateGrowth(bool isWatered);

    // 获取作物的物品类型（如 Parsnip/Potato 等）
    ItemType getCropType() const { return _type; }
    CropStatus getStatus() const { return _status; }
    int getGrowthStage() const { return _growthStage; }

protected:
    // 当前作物的物品类型
    ItemType _type;
    // 当前作物生长状态
    CropStatus _status;
    // 当前生长阶段
    int _growthStage;
    // 最大生长阶段（到达即成熟）
    int _maxGrowthStage;
    // 纹理前缀（用于拼接不同阶段的贴图）
    std::string _texturePrefix;

    void updateTexture();
};

class Parsnip : public Crop {
public:
    static Parsnip* create();
    virtual bool init() override;
};

class Potato : public Crop {
public:
    static Potato* create();
    virtual bool init() override;
};

class Cauliflower : public Crop {
public:
    static Cauliflower* create();
    virtual bool init() override;
};

#endif // __CROP_H__
