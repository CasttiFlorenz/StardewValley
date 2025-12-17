#include "GrassItem.h"

GrassItem* GrassItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) GrassItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool GrassItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::GRASS, tileCoord)) {
        return false;
    }

    this->setTexture("EnvironmentObjects/Grass.png");


    return true;
}
