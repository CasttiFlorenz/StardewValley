#include "CultivatedSoil.h"

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
    if (!EnvironmentItem::init(EnvironmentItemType::CULTIVATED_SOIL, tileCoord)) {
        return false;
    }

    _status = SoilStatus::DRY;
    _crop = nullptr;
    this->setTexture("EnvironmentObjects/Dry.png");
    // Explicitly set the anchor point to (0, 0) to match tile map coordinate system
    // if other items use a different anchor point.
    // However, usually sprites are (0.5, 0.5). TiledMap coordinates are bottom-left.
    // If we rely on FarmItemManager's positioning, it sets position to center of tile.
    
    // Ensure content size is set correctly if not set by setTexture
    if (this->getContentSize().width == 0) {
        // Fallback size if texture fails load, though setTexture usually handles it.
        // Assuming 16x16 tiles scaled by 5.0f elsewhere, but sprite should be the raw image size.
    }

    return true;
}

void CultivatedSoil::water() {
    if (_status == SoilStatus::DRY) {
        _status = SoilStatus::WET;
        this->setTexture("EnvironmentObjects/Wet.png");
    }
}

void CultivatedSoil::dry() {
    if (_status == SoilStatus::WET) {
        _status = SoilStatus::DRY;
        this->setTexture("EnvironmentObjects/Dry.png");
    }
}

bool CultivatedSoil::plant(CropType type) {
    if (_crop != nullptr) {
        return false; // Already has a crop
    }

    _crop = Crop::create(type);
    if (_crop) {
        // Center the crop on the soil
        Size soilSize = this->getContentSize();
        _crop->setPosition(Vec2(soilSize.width / 2, soilSize.height / 3 * 2));
        this->addChild(_crop);
        return true;
    }
    return false;
}

CropType CultivatedSoil::harvest() {
    if (_crop && _crop->getStatus() == CropStatus::MATURE) {
        CropType type = _crop->getCropType();
        
        _crop->removeFromParent();
        _crop = nullptr;
        
        return type;
    }
    return CropType::NONE;
}

void CultivatedSoil::updateDay() {
    if (_crop) {
        // Pass whether it was watered yesterday (current status is yesterday's status before reset)
        _crop->updateGrowth(_status == SoilStatus::WET);
    }

    // Reset soil to dry for the new day
    dry();
}
