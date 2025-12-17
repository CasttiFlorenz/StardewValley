#include "StoneItem.h"

StoneItem* StoneItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) StoneItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool StoneItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::STONE, tileCoord)) {
        return false;
    }

    this->setTexture("EnvironmentObjects/Stone.png");

    return true;
}
