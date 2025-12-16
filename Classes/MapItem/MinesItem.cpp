#include "MinesItem.h"

MinesItem* MinesItem::create(MinesItemType type, const Vec2& tileCoord) {
    auto p = new (std::nothrow) MinesItem();
    if (p && p->init(type, tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool MinesItem::init(MinesItemType type, const Vec2& tileCoord) {
    if (!Sprite::init()) {
        return false;
    }

    _type = type;
    _tileCoord = tileCoord;

    return true;
}
