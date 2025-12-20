#include "DaffodilsItem.h"

DaffodilsItem* DaffodilsItem::create(const cocos2d::Vec2& tileCoord) {
    auto p = new (std::nothrow) DaffodilsItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool DaffodilsItem::init(const cocos2d::Vec2& tileCoord) {
    if (!EnvironmentItem::init(EnvironmentItemType::DAFFODILS, tileCoord)) {
        return false;
    }

    this->setTexture("EnvironmentObjects/Daffodils.png");

    return true;
}