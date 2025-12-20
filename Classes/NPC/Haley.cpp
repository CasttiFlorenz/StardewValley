#include "Haley.h"

bool Haley::init()
{
    if (!NPCBase::init()) {
        return false;
    }

    // 加载纹理
    if (!loadTexture("NPC/Haley..png")) {
        return false;
    }

    // 创建并播放动画：12行4列，使用第11行（索引10）
    createAnimation(12, 4, 10, 0.15f);

    return true;
}

void Haley::playAnimation()
{

}