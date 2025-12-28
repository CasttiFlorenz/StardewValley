/****************************************************************
 * Project Name:  StardewValley
 * File Name:     BarnAnimal.cpp
 * File Function: BarnAnimal类的实现
 * Author:        郭芷烟
 * Update Date:   2025/12/28
 * License:       MIT License
 ****************************************************************/
#include "BarnAnimal.h"
#include <algorithm>

USING_NS_CC;

// 批量加载动画帧
std::vector<SpriteFrame*> BarnAnimal::framesFromFiles(const std::string& baseNoExt, int maxFrames)
{
    std::vector<SpriteFrame*> frames;
    // 预分配内存避免频繁扩容
    frames.reserve(static_cast<size_t>(std::max(0, maxFrames)));

    for (int i = 0; i < maxFrames; ++i) {
        const std::string path = baseNoExt + std::to_string(i) + ".png";

        // 尝试创建精灵以获取 SpriteFrame
        const auto spr = Sprite::create(path);
        if (!spr) {
            continue;
        }

        auto frame = spr->getSpriteFrame();
        if (frame) {
            frames.push_back(frame);
        }
    }
    return frames;
}
