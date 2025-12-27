#include "GrassItem.h"

GrassItem* GrassItem::create(const cocos2d::Vec2& tileCoord) {
    // 工厂方法：安全创建并自动管理生命周期
    auto p = new (std::nothrow) GrassItem();
    if (p && p->init(tileCoord)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool GrassItem::init(const cocos2d::Vec2& tileCoord) {
    // 先初始化基类，绑定类型与瓦片坐标
    if (!EnvironmentItem::init(EnvironmentItemType::GRASS, tileCoord)) {
        return false;
    }

    // 使用精灵的文件名设置纹理（保持与项目其他环境物体一致）
    this->setTexture("EnvironmentObjects/Grass.png");


    return true;
}