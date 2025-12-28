/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Crop.cpp
 * File Function: Crop类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "Crop.h"

// 工厂创建方法
Crop* Crop::create(ItemType type) {
    Crop* p = nullptr;
    switch (type) {
    case ItemType::PARSNIP:
        p = Parsnip::create();
        break;
    case ItemType::POTATO:
        p = Potato::create();
        break;
    case ItemType::CAULIFLOWER:
        p = Cauliflower::create();
        break;
    default:
        return nullptr;
    }
    return p;
}

// 初始化基类
bool Crop::init(ItemType type) {
    if (!Sprite::init()) {
        return false;
    }
    _type = type;
    _status = CropStatus::SEEDS;
    _growthStage = INITIAL_GROWTH_STAGE;
    return true;
}

// 更新生长
void Crop::updateGrowth(bool isWatered) {
    if (_status == CropStatus::DEAD || _status == CropStatus::MATURE) {
        return;
    }

    if (!isWatered) {
        _status = CropStatus::DEAD;
        this->setTexture(CROP_DEAD_TEXTURE_PATH);
        return;
    }

    _growthStage++;

    if (_growthStage >= _maxGrowthStage) {
        _status = CropStatus::MATURE;
        _growthStage = _maxGrowthStage;
    }
    else {
        _status = CropStatus::GROWING;
    }

    updateTexture();
}

// 更新纹理
void Crop::updateTexture() {
    if (_status == CropStatus::DEAD) {
        this->setTexture(CROP_DEAD_TEXTURE_PATH);
        return;
    }

    std::string filename =
        CROP_TEXTURE_BASE_PATH_PREFIX + _texturePrefix + "_" +
        std::to_string(_growthStage) + CROP_TEXTURE_STAGE_SUFFIX;
    this->setTexture(filename);
}

// 设置生长阶段
void Crop::setGrowthStage(int stage) {
    _growthStage = std::max(0, std::min(stage, _maxGrowthStage));
    updateTexture();
    if (_growthStage >= _maxGrowthStage) {
        _status = CropStatus::MATURE;
    }
    else {
        _status = CropStatus::GROWING;
    }
}

// 设置阶段与状态
void Crop::setGrowthStage(int stage, CropStatus status) {
    _growthStage = std::max(0, std::min(stage, _maxGrowthStage));
    _status = status;

    if (_status != CropStatus::DEAD) {
        if (_growthStage >= _maxGrowthStage) {
            _status = CropStatus::MATURE;
        }
        else {
            _status = CropStatus::GROWING;
        }
    }

    updateTexture();
}

// --- 子类实现 ---

// 防风草
Parsnip* Parsnip::create() {
    auto p = new (std::nothrow) Parsnip();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool Parsnip::init() {
    if (!Crop::init(ItemType::PARSNIP)) return false;
    _texturePrefix = CROP_TEXTURE_PARSNIP_PREFIX;
    _maxGrowthStage = PARSNIP_MAX_GROWTH_STAGE;
    updateTexture();
    return true;
}

// 土豆
Potato* Potato::create() {
    auto p = new (std::nothrow) Potato();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool Potato::init() {
    if (!Crop::init(ItemType::POTATO)) return false;
    _texturePrefix = CROP_TEXTURE_POTATO_PREFIX;
    _maxGrowthStage = POTATO_MAX_GROWTH_STAGE;
    updateTexture();
    return true;
}

// 花椰菜
Cauliflower* Cauliflower::create() {
    auto p = new (std::nothrow) Cauliflower();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool Cauliflower::init() {
    if (!Crop::init(ItemType::CAULIFLOWER)) return false;
    _texturePrefix = CROP_TEXTURE_CAULIFLOWER_PREFIX;
    _maxGrowthStage = CAULIFLOWER_MAX_GROWTH_STAGE;
    updateTexture();
    return true;

}
