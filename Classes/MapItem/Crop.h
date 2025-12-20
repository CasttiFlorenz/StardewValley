#pragma once
#ifndef __CROP_H__
#define __CROP_H__

#include "cocos2d.h"
#include "Constant.h"
#include "../Time/GameTime.h"

USING_NS_CC;

class Crop : public Sprite {
public:
    static Crop* create(CropType type);
    virtual bool init(CropType type);

    // 更新作物状态（每天调用一次）
    void updateGrowth(bool isWatered);

    CropType getCropType() const { return _type; }
    CropStatus getStatus() const { return _status; }
    int getGrowthStage() const { return _growthStage; }

protected:
    CropType _type;
    CropStatus _status;
    int _growthStage;
    int _maxGrowthStage;
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
