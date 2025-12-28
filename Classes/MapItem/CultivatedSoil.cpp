/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CultivatedSoil.cpp
 * File Function: CultivatedSoil类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/16
 * License:       MIT License
 ****************************************************************/
#include "CultivatedSoil.h"

// 创建实例
CultivatedSoil* CultivatedSoil::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) CultivatedSoil();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

// 初始化
bool CultivatedSoil::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::CULTIVATED_SOIL, tileCoord)) {
        return false;
    }

    _status = SoilStatus::DRY;
    _crop = nullptr;

    this->setTexture(SOIL_DRY_TEXTURE_PATH);

    return true;
}

// 析构函数
CultivatedSoil::~CultivatedSoil() {
    if (_crop) {
        _crop->removeFromParent();
        _crop = nullptr;
    }
}

// 浇水
void CultivatedSoil::water() {
    if (_status != SoilStatus::DRY) {
        return;
    }

    _status = SoilStatus::WET;
    this->setTexture(SOIL_WET_TEXTURE_PATH);
}

// 变干
void CultivatedSoil::dry() {
    if (_status != SoilStatus::WET) {
        return;
    }

    _status = SoilStatus::DRY;
    this->setTexture(SOIL_DRY_TEXTURE_PATH);
}

// 种植作物
bool CultivatedSoil::plant(ItemType type) {
    if (_crop) {
        return false;
    }

    _crop = Crop::create(type);
    if (!_crop) {
        return false;
    }

    const cocos2d::Size soilSize = this->getContentSize();
    _crop->setPosition(cocos2d::Vec2(
        soilSize.width * CROP_POSITION_X_SCALE,
        soilSize.height * (static_cast<float>(CROP_POSITION_Y_NUMERATOR) / CROP_POSITION_Y_DENOMINATOR)
    ));

    this->addChild(_crop);
    return true;
}

// 收获作物
ItemType CultivatedSoil::harvest() {
    if (!_crop) {
        return ItemType::NONE;
    }

    if (_crop->getStatus() != CropStatus::MATURE) {
        return ItemType::NONE;
    }

    ItemType type = _crop->getCropType();
    _crop->removeFromParent();
    _crop = nullptr;

    return type;
}

// 每日更新
void CultivatedSoil::updateDay() {
    if (_crop) {
        _crop->updateGrowth(_status == SoilStatus::WET);
    }

    dry();
}

// 设置状态
void CultivatedSoil::setStatus(SoilStatus status) {
    _status = status;
    if (_status == SoilStatus::WET) {
        setTexture(SOIL_WET_TEXTURE_PATH);
    }
    else {
        setTexture(SOIL_DRY_TEXTURE_PATH);
    }

}
