#include "FarmItem.h"

FarmItem* FarmItem::create(FarmItemType type, const Vec2& tileCoord) {
    auto p = new (std::nothrow) FarmItem();
    if (p && p->init(type, tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool FarmItem::init(FarmItemType type, const Vec2& tileCoord) {
    if (!Sprite::init()) {
        return false;
    }
    _type = type;
    _tileCoord = tileCoord;
    return true;
}
