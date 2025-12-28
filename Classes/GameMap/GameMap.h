#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "cocos2d.h"
#include "Constant.h"

USING_NS_CC;

// 游戏地图基类
// 职责：地图加载、坐标转换、碰撞检测、地图切换
class GameMap : public Scene
{
public:
    // 初始化地图
    virtual bool init() override;

    // 帧更新
    virtual void update(float dt) noexcept override {}

    // 进入地图时的初始化
    virtual void IntoMap(MapType lastMap) = 0;

    // 获取当前地图名称
    MapType getMapName() const noexcept { return _mapName; }

    // 获取 Tiled 地图对象
    TMXTiledMap* getTiledMap()  const noexcept { return _map; }

    // 是否相机跟随
    virtual bool isCameraFollow() const noexcept { return false; }

    // 是否为户外地图
    virtual bool isOutdoor() noexcept { return false; }

    // 获取玩家初始位置
    virtual Vec2 getPlayerStartPosition(MapType lastMap) = 0;

    // 判断是否需要切换地图
    virtual MapType leaveMap(const Vec2& curPos,
        bool isStart,
        const Direction& direction) = 0;

    // 判断坐标是否可通行
    virtual bool isCollidable(Vec2 worldPos);

    // 世界坐标 -> 瓦片坐标
    Vec2 calMapPos(Vec2 worldPos);

    // 瓦片坐标 -> 世界坐标
    Vec2 calWorldPos(const Vec2& tileCoord);

    // 获取对象层矩形区域
    Rect getObjectRect(const std::string& objectName);

    // 左键点击处理
    virtual MouseEvent onLeftClick(const Vec2& playerPos,
        const Direction direction,
        ItemType objects) 
    {
        return MouseEvent::USE_TOOL;
    }

    // 右键点击处理
    virtual MouseEvent onRightClick(const Vec2& playerPos,
        const Direction direction) 
    {
        return MouseEvent::NONE;
    }

    // 应用方向偏移
    virtual void ApplyDirectionOffset(Vec2& basePos, Direction direction);

    // 判断属性值是否为真
    virtual bool IsTrueProperty(const Value& v);

protected:
    TMXTiledMap* _map = nullptr;   // Tiled 地图对象
    MapType _mapName;              // 地图名称
};

#endif // __GAME_MAP_H__