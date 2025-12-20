#include "Evelyn.h"

bool Evelyn::init()
{
    if (!NPCBase::init()) {
        return false;
    }

    // 加载Evelyn的纹理
    if (!loadTexture("NPC/Evelyn..png")) { 
        return false;
    }

    // 设置静态帧（不播放动画）
    setupStaticFrame();

    return true;
}

void Evelyn::setupStaticFrame()
{
    auto texture = this->getTexture();
    if (!texture) return;

    // Evelyn纹理是5行4列
    int totalRows = 5;
    int totalCols = 4;

    // 计算每帧尺寸
    float frameWidth = texture->getContentSize().width / totalCols;
    float frameHeight = texture->getContentSize().height / totalRows;

    // 使用第1行第1帧（索引0,0）作为静态显示
    float x = 0;  // 第1列
    float y = 0 * frameHeight;  // 第1行（索引0）

    // 设置纹理显示区域
    this->setTextureRect(Rect(x, y, frameWidth, frameHeight));

    CCLOG("Evelyn设置为静态显示：第1行第1帧");
}