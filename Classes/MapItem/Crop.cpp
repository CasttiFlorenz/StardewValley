#include "Crop.h"

Crop* Crop::create(CropType type) {
    Crop* p = nullptr;
    switch (type) {
        case CropType::PARSNIP:
            p = Parsnip::create();
            break;
        case CropType::POTATO:
            p = Potato::create();
            break;
        case CropType::CAULIFLOWER:
            p = Cauliflower::create();
            break;
        default:
            return nullptr;
    }
    return p;
}

bool Crop::init(CropType type) {
    if (!Sprite::init()) {
        return false;
    }
    _type = type;
    _status = CropStatus::SEEDS;
    _growthStage = 0;
    return true;
}

void Crop::updateGrowth(bool isWatered) {
    if (_status == CropStatus::DEAD || _status == CropStatus::MATURE) {
        return;
    }

    if (!isWatered) {
        _status = CropStatus::DEAD;
        this->setTexture("EnvironmentObjects/dead.png");
        return;
    }

    _growthStage++;
    
    // Check if mature
    if (_growthStage >= _maxGrowthStage) {
        _status = CropStatus::MATURE;
        _growthStage = _maxGrowthStage; // Ensure we don't exceed max
    } else {
        _status = CropStatus::GROWING;
    }

    updateTexture();
}

void Crop::updateTexture() {
    if (_status == CropStatus::DEAD) {
        this->setTexture("EnvironmentObjects/dead.png");
        return;
    }

    // Texture name format: Prefix_Stage.png (e.g., Parsnip_0.png)
    // Assuming stage 0 is seeds, and max stage is mature
    // If we have stages 0, 1, 2, 3 and max is 3 (4 images)
    
    std::string filename = "EnvironmentObjects/" + _texturePrefix + "_" + std::to_string(_growthStage) + ".png";
    this->setTexture(filename);
}

// Parsnip Implementation
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
    if (!Crop::init(CropType::PARSNIP)) return false;
    _texturePrefix = "Parsnip";
    _maxGrowthStage = 3; // 0, 1, 2, 3 (4 stages)
    updateTexture();
    return true;
}

// Potato Implementation
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
    if (!Crop::init(CropType::POTATO)) return false;
    _texturePrefix = "Potato";
    _maxGrowthStage = 4; // 0, 1, 2, 3, 4 (5 stages)
    updateTexture();
    return true;
}

// Cauliflower Implementation
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
    if (!Crop::init(CropType::CAULIFLOWER)) return false;
    _texturePrefix = "Cauliflower";
    _maxGrowthStage = 4; // 0, 1, 2, 3, 4 (5 stages)
    updateTexture();
    return true;
}
