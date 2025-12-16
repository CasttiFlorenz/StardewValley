#include "LeekItem.h"

LeekItem* LeekItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) LeekItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool LeekItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::LEEK, tileCoord)) {
        return false;
    }

    this->setTexture("EnvironmentObjects/Leek.png");

    return true;
}
