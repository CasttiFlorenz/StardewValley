/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SocialLevel.cpp
 * File Function: SocialLevel类的实现
 * Author:        于恩熙
 * Update Date:   2025/12/21
 * License:       MIT License
 ****************************************************************/
#include "SocialLevel.h"

USING_NS_CC;

// 获取NPC头像数据
void SocialLevel::getNPCStyle(const std::string& name, std::string& outIconPath, float& outScale)
{
    if (name == NPC_NAME_EVELYN) {
        outIconPath = PATH_SOCIAL_EVELYN;
        outScale = 6.0f;
    }
    else if (name == NPC_NAME_HALEY) {
        outIconPath = PATH_SOCIAL_HALEY;
        outScale = 5.2f;
    }
    else if (name == NPC_NAME_SAM) {
        outIconPath = PATH_SOCIAL_SAM;
        outScale = 5.2f;
    }
    else {
        // 默认头像
        outIconPath = PATH_SOCIAL_EVELYN;
        outScale = 5.0f;
    }
}

// 创建好感度爱心显示
Node* SocialLevel::createFriendshipHearts(int level)
{
    Node* heartContainer = Node::create();
    const float SPACING = 35.0f;

    float totalWidth = (TOTAL_HEARTS - 1) * SPACING;
    float startX = -totalWidth / 2;

    for (int i = 0; i < TOTAL_HEARTS; i++) {
        // 假设每 150 分一颗心
        bool isFilled = (i < level / 150);
        const std::string heartPath = isFilled ? HEART_FILLED : HEART_EMPTY;

        // 打印爱心
        auto heartSprite = Sprite::create(heartPath);
        if (heartSprite) {
            float currentX = startX + i * SPACING;
            heartSprite->setPosition(Vec2(currentX, 0));
            heartSprite->setScale(0.3f);
            heartContainer->addChild(heartSprite);
        }
    }
    return heartContainer;
}

// 创建单个社交人物项
Node* SocialLevel::createCharacterItem(NPCBase* npc, const Vec2& position)
{
    if (!npc) return nullptr;

    // 获取实时数据
    std::string name = npc->getNPCName(); 
    int currentFriendship = npc->getFriendship(); 

    // 获取 UI 样式
    std::string iconPath;
    float iconScale;
    getNPCStyle(name, iconPath, iconScale);

    // 创建容器
    Node* characterItem = Node::create();
    characterItem->setPosition(position);

    // ========== 人物头像 ==========
    auto characterIcon = Sprite::create(iconPath);
    if (characterIcon) {
        characterIcon->setScale(iconScale);
        characterIcon->setAnchorPoint(Vec2(0, 0.5f));
        characterIcon->setPosition(Vec2(-350, 0));
        characterItem->addChild(characterIcon);
    }

    // ========== 人物名字 ==========
    auto nameLabel = Label::createWithTTF(name, PATH_FONT_LOUIS, 28);
    if (nameLabel) {
        nameLabel->setTextColor(Color4B::BLACK);
        nameLabel->setAnchorPoint(Vec2(0, 0.5f));
        nameLabel->setPosition(Vec2(-260, 0));
        characterItem->addChild(nameLabel);
    }

    // ========== 竖线 ==========
    DrawNode* verticalLine = DrawNode::create();
    if (verticalLine) {
        verticalLine->drawSegment(Vec2(-150, -75), Vec2(-150, 75), 2.0f, Color4F(0.55f, 0.27f, 0.07f, 1.0f));
        characterItem->addChild(verticalLine);
    }

    // ========== 好感度爱心 ==========
    Node* heartContainer = createFriendshipHearts(currentFriendship);
    if (heartContainer) {
        heartContainer->setPosition(Vec2(100, 0));
        heartContainer->setScale(1.2f);
        characterItem->addChild(heartContainer);
    }

    return characterItem;
}

// 创建社交界面
Node* SocialLevel::createSocialInterface(Node* parent, Sprite* background)
{
    if (!parent || !background) return nullptr;

    // 创建主容器
    Node* container = Node::create();

    // 获取背景位置
    Vec2 bgPos = background->getPosition();
    float startY = bgPos.y + 120;
    float spacing = 120;

    const auto& npcList = NPCManager::getInstance()->getAllNPCs();

    // 遍历所有活着的 NPC
    for (size_t i = 0; i < npcList.size(); ++i) {
        NPCBase* npc = npcList[i];
        if (!npc) continue;

        // 计算位置
        Vec2 position = Vec2(bgPos.x, startY - i * spacing);

        // 创建条目
        Node* characterItem = createCharacterItem(npc, position);
        if (characterItem) {
            container->addChild(characterItem);

            if (i < npcList.size() - 1) {
                DrawNode* horizontalLine = DrawNode::create();
                if (horizontalLine) {
                    float lineY = startY - (i + 0.5f) * spacing;
                    horizontalLine->drawSegment(
                        Vec2(bgPos.x - 460, lineY), // 920的一半
                        Vec2(bgPos.x + 460, lineY),
                        2.0f,
                        Color4F(0.55f, 0.27f, 0.07f, 1.0f)
                    );
                    container->addChild(horizontalLine);
                }
            }
        }
    }


    parent->addChild(container, 100);
    return container;
}