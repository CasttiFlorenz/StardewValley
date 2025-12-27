#include "Crop.h"

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

bool Crop::init(ItemType type) {
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

    if (_growthStage >= _maxGrowthStage) {
        _status = CropStatus::MATURE;
        _growthStage = _maxGrowthStage; 
    }
    else {
        _status = CropStatus::GROWING;
    }

    updateTexture();
}

void Crop::updateTexture() {
    if (_status == CropStatus::DEAD) {
        this->setTexture("EnvironmentObjects/dead.png");
        return;
    }

    std::string filename =
        "EnvironmentObjects/" + _texturePrefix + "_" +
        std::to_string(_growthStage) + ".png";
    this->setTexture(filename);
}

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
    _texturePrefix = "Parsnip";
    _maxGrowthStage = 3; 
    updateTexture();
    return true;
}

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
    _texturePrefix = "Potato";
    _maxGrowthStage = 4;
    updateTexture();
    return true;
}

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
    _texturePrefix = "Cauliflower";
    _maxGrowthStage = 4; 
    updateTexture();
    return true;
}
