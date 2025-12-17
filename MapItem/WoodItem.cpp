#include "WoodItem.h"

WoodItem* WoodItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) WoodItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool WoodItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::WOOD, tileCoord)) {
        return false;
    }

    this->setTexture("EnvironmentObjects/Wood.png");

    return true;
}
