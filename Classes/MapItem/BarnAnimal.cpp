#include "BarnAnimal.h"

USING_NS_CC;

// 批量从文件中创建动画帧：基础路径 + 索引 + ".png"
std::vector<SpriteFrame*> BarnAnimal::framesFromFiles(const std::string& baseNoExt, int maxFrames)
{
    std::vector<SpriteFrame*> frames;
    frames.reserve(static_cast<size_t>(std::max(0, maxFrames)));

    for (int i = 0; i < maxFrames; ++i) {
        std::string path = baseNoExt + std::to_string(i) + ".png";
        auto spr = Sprite::create(path);
        if (!spr) {
            // 若遇到不存在的资源，继续尝试后续（保证在资源缺失时不崩溃）
            continue;
        }
        auto f = spr->getSpriteFrame();
        if (f) {
            frames.push_back(f);
        }
    }
    return frames;
}

