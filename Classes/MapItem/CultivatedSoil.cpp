#include "CultivatedSoil.h"

// 工厂方法：创建指定坐标的耕地实例
CultivatedSoil* CultivatedSoil::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) CultivatedSoil();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool CultivatedSoil::init(const cocos2d::Vec2& tileCoord) {
    // 初始化为“耕地”类型的环境物体
    if (!EnvironmentItem::init(EnvironmentItemType::CULTIVATED_SOIL, tileCoord)) {
        return false;
    }

    // 初始状态：干燥、无作物
    _status = SoilStatus::DRY;
    _crop = nullptr;

    // 设置初始贴图
    this->setTexture("EnvironmentObjects/Dry.png");

    return true;
}

// 防御性地确保作物节点被移除，避免悬空引用
CultivatedSoil::~CultivatedSoil() {
    if (_crop) {
        _crop->removeFromParent();
        _crop = nullptr;
    }
}

// 浇水：仅在干燥状态下生效
void CultivatedSoil::water() {
    if (_status != SoilStatus::DRY) {
        return;
    }

    _status = SoilStatus::WET;
    this->setTexture("EnvironmentObjects/Wet.png");
}

// 土壤干燥：仅在湿润状态下切换
void CultivatedSoil::dry() {
    if (_status != SoilStatus::WET) {
        return;
    }

    _status = SoilStatus::DRY;
    this->setTexture("EnvironmentObjects/Dry.png");
}

// 在土壤上种植作物
bool CultivatedSoil::plant(ItemType type) {
    // 已有作物，无法再次种植
    if (_crop) {
        return false;
    }

    _crop = Crop::create(type);
    if (!_crop) {
        return false;
    }

    // 将作物显示在土壤中央偏上的位置
    const cocos2d::Size soilSize = this->getContentSize();
    _crop->setPosition(cocos2d::Vec2(
        soilSize.width * 0.5f,
        soilSize.height * (2.0f / 3.0f)
    ));

    this->addChild(_crop);
    return true;
}

// 收获成熟作物
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

// 每日更新逻辑：
// 1. 根据昨天是否浇水推进作物生长
// 2. 新的一天开始时，土壤恢复为干燥状态
void CultivatedSoil::updateDay() {
    if (_crop) {
        _crop->updateGrowth(_status == SoilStatus::WET);
    }

    dry();
}
