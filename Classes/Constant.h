#ifndef __CONSTANT_H__
#define __CONSTANT_H__
#include <map>
#include <string>


USING_NS_CC;

// 地图缩放比例
constexpr float TILED_MAP_SCALE = 5.0f;
// 玩家默认速度
constexpr float DEFAULT_SPEED = 500.0f;

// 农舍地图宽高（瓦片数）
constexpr int FARM_HOUSE_WIDTH = 20;
constexpr int FARM_HOUSE_HEIGHT = 12;

// 定义方向枚举
enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

constexpr int CAMERA_POSZ = 200;

#endif // __CONSTANT_H__