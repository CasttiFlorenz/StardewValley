/****************************************************************
 * Project Name:  StardewValley
 * File Name:     InventoryScene.cpp
 * File Function: InventoryScene类的实现
 * Author:        于恩熙、赵睿妍
 * Update Date:   2025/12/17
 * License:       MIT License
 ****************************************************************/

#include "InventoryScene.h"

USING_NS_CC;

InventoryScene* InventoryScene::_instance = nullptr;

InventoryScene* InventoryScene::getInstance() {
    if (!_instance) {
        _instance = InventoryScene::create();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void InventoryScene::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}
// 初始化主场景 
bool InventoryScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();


    _previewFrame = Sprite::create("/Items/box.png"); // 预览框图片

    // 放在左下角
    float previewX = origin.x + 100;      // 左边距100
    float previewY = origin.y + 100;      // 下边距100
    _previewFrame->setPosition(cocos2d::Vec2(previewX, previewY));
    this->addChild(_previewFrame, 5);

    // 预览工具图片
    _previewTool = Sprite::create();
    _previewTool->setPosition(_previewFrame->getPosition());
    this->addChild(_previewTool, 6);

    // 创建背包界面
    _inventoryLayer = InventoryGridScene::create();
    if (_inventoryLayer) {
        _inventoryLayer->setVisible(false);
        _inventoryLayer->setGridSelectedCallback([this](int gridIndex) {
            // 当背包中格子被选中时，立即更新预览
            this->updatePreviewTool();
            });
        this->addChild(_inventoryLayer, 10);
    }
    _inventoryLayer->setGridsTouchEnabled(false);

    _inventoryVisible = false;
    _inventoryLayer->setGridsTouchEnabled(false);
    _inventoryVisible = false;

    // 监听 "INVENTORY_COUNT_CHANGED" 事件
    // 只要有地方发这个信号，主界面就刷新预览图和背包数据
    auto refreshListener = EventListenerCustom::create("INVENTORY_COUNT_CHANGED", [this](EventCustom* event) {

        // 1. 刷新全屏背包的数据（即使它现在是隐藏的，也要更新，防止下次打开闪烁）
        if (_inventoryLayer) {
            _inventoryLayer->placeTools(-1);
        }

        // 2. 刷新左下角预览图
        this->updatePreviewTool();
        });

    _eventDispatcher->addEventListenerWithSceneGraphPriority(refreshListener, this);

    return true;
}

// 工具特效接口 
void InventoryScene::ToolUseAnimation()
{
    if (_showToolUseEffect && ((_selectedGrid >= 0 && _selectedGrid <= 5)
        || (_selectedGrid >= 10 && _selectedGrid <= 12))) {

        const Item* item = _inventoryLayer->getItemAt(_selectedGrid);
        if (item->getCount() > 0)
            // 显示工具使用特效
            showToolUseEffect(_selectedGrid);
    }
}

// 显示工具特效 
void InventoryScene::showToolUseEffect(int selected, bool clearPrevious)
{
    // 先隐藏之前可能存在的特效
    if (clearPrevious)
        hideToolUseEffect();

    // 从背包获取当前选中的工具信息
    if (_inventoryLayer) {
        const Item* item = _inventoryLayer->getItemAt(selected);
        std::string toolImagePath = item->getPos();

        if (!toolImagePath.empty()) {
            // 创建工具特效精灵
            _toolUseEffect = Sprite::create(toolImagePath);
            if (_toolUseEffect) {

                setToolUse();

                _toolUseEffect->setScale(item->getScale());    // 设置大小

                float time = 0.3f;
                auto delay = DelayTime::create(time);

                //逆时针旋转30度
                auto rotate = RotateBy::create(time, -30.0f);
                _toolUseEffect->setFlippedX(true);   //水平镜像

                if (_player->getPlayerDirection() == Direction::RIGHT ||
                    _player->getPlayerDirection() == Direction::DOWN) {
                    _toolUseEffect->setFlippedX(false);
                    rotate = RotateBy::create(time, 30.0f);
                }
                auto remove = CallFunc::create([this]() {
                    hideToolUseEffect();
                    });

                cocos2d::Action* sequence;
                if (static_cast<int>(item->getTag()) < 5)
                    sequence = Sequence::create(rotate, remove, nullptr);   // 组合动画：旋转 -> 停留 -> 移除
                else 
                    sequence = Sequence::create(delay, remove, nullptr);   // 闪现
                _toolUseEffect->runAction(sequence);
            }
        }
    }
}

// 使用工具动画的属性
void InventoryScene::setToolUse()
{
    Node* playerScene = _player;
    while (playerScene && playerScene->getParent()) {
        playerScene = playerScene->getParent();
    }

    if (playerScene) {
        // 添加到玩家所在的场景
        playerScene->addChild(_toolUseEffect, 10);

        // 设置和玩家相同的摄像机
        _toolUseEffect->setCameraMask(_player->getCameraMask(), true);

        // 使用玩家的位置（相对坐标）
        Vec2 offset = Vec2::ZERO;
        switch (_player->getPlayerDirection()) {
            case Direction::DOWN: offset = Vec2(5, -2); break;
            case Direction::UP: offset = Vec2(5, 18); break;
            case Direction::LEFT: offset = Vec2(-3, 5); break;
            case Direction::RIGHT: offset = Vec2(13, 5); break;
        }
        Vec2 worldPos = _player->convertToWorldSpace(offset);
        _secondEffectPos = worldPos;
        _toolUseEffect->setPosition(worldPos);
    }
}

void InventoryScene::hideToolUseEffect()
{
    if (_toolUseEffect) {
        _toolUseEffect->stopAllActions();      // 停止所有动画
        _toolUseEffect->removeFromParent();    // 从父节点移除
        _toolUseEffect = nullptr;              // 指针置空
    }
}


void InventoryScene::toggleInventory()
{
    _inventoryVisible = !_inventoryVisible;

    if (_inventoryLayer) {
        _inventoryLayer->setVisible(_inventoryVisible);

        if (_inventoryVisible) {
           
            _inventoryLayer->placeTools(-1);

            // 顺便更新一下预览框
            updatePreviewTool();

            _inventoryLayer->setGridsTouchEnabled(true);

            _showToolUseEffect = false;
            hideToolUseEffect();

            _inventoryLayer->setScale(0.1f);
            auto scaleAction = ScaleTo::create(0.3f, 1.0f);
            _inventoryLayer->runAction(scaleAction);
        }
        else {
            _inventoryLayer->setGridsTouchEnabled(false);

            _showToolUseEffect = true;
            _selectedGrid = _inventoryLayer->getSelectedGrid();

            auto scaleAction = ScaleTo::create(0.2f, 0.1f);
            auto hideAction = CallFunc::create([this]() {
                _inventoryLayer->setVisible(false);
                _inventoryLayer->setScale(1.0f);
                });
            auto sequence = Sequence::create(scaleAction, hideAction, nullptr);
            _inventoryLayer->runAction(sequence);
        }
    }
}

// 更新预览的工具图片
void InventoryScene::updatePreviewTool()
{
    if (!_inventoryLayer) {
        return;
    }

    // 获取背包中当前高光的格子索引
    _selectedGrid = _inventoryLayer->getSelectedGrid();

    // 删除旧的标签
    this->removeChildByTag(999);

    if (_selectedGrid >= 0 && _selectedGrid < _inventoryLayer->INTEM_COUNT) { // 只显示前INTEM_COUNT个工具
        // 获取工具图片路径
        const Item* item = _inventoryLayer->getItemAt(_selectedGrid);
        std::string toolImage = item->getPos();

        if (!toolImage.empty()) {
            auto toolSprite = cocos2d::Sprite::create(toolImage);
            if (toolSprite) {
                _previewTool->setTexture(toolSprite->getTexture());
                _previewTool->setTextureRect(toolSprite->getTextureRect());
                _previewTool->setScale(item->getScale() + 0.1f); // 放大一点显示
                _previewTool->setVisible(true);

                // 显示数量
                if (item->getTag() > Objects::FISHINGROD) {
                    std::string countText = std::to_string(item->getCount());
                    auto label = Label::createWithTTF(countText, "/fonts/arial.ttf", 18);

                    if (label) {
                        label->setPosition(_previewTool->getPositionX() + 30,
                            _previewTool->getPositionY() - 30);
                        label->setColor(Color3B::WHITE);
                        label->enableOutline(Color4B::BLACK, 2);  // 黑色描边，更清晰
                        label->setTag(999);
                        this->addChild(label, 100);
                    }
                }
            }
        }
    }
    else
        _previewTool->setVisible(false);  // 保持可见，显示空框
}

//获取选中物品的标签
Objects InventoryScene::getTap() const
{
    if (_selectedGrid >= 0 && _selectedGrid < _inventoryLayer->INTEM_COUNT) {
        const Item* item = _inventoryLayer->getItemAt(_selectedGrid);
        return item->getTag();
    }
    return Objects::HOE;
}



// 增加指定物品的数量
void InventoryScene::addItemCount(Objects object, int amount)
{
    int itemIndex = static_cast<int>(object);

    if (object >= Objects::HOE && object < Objects::COUNT) {

        // 1. 获取 PlayerState 单例
        auto playerState = PlayerState::getInstance();
        auto& items = playerState->getInventoryItems();

        Item* foundItem = nullptr;

        // 2. 在背包中查找是否已有该物品 (用于堆叠)
        for (auto item : items) {
            if (item->getTag() == object) {
                foundItem = item;
                break;
            }
        }

        if (foundItem) {
        
            playerState->addItem(foundItem, amount);
        }
        else {
          
            Item* newItem = new Item(object, amount, 1.0f, 0.0f, (char*)"Items/unknown.png", 0, "New Item");
            playerState->addItem(newItem, amount);
            delete newItem; // PlayerState 会复制数据，删除临时的
        }

        if (_inventoryLayer) {
            _inventoryLayer->placeTools(-1); 
        }

    

        // 显示工具使用特效
        if (object != Objects::DAFFODILS && object != Objects::LEEK)
            ToolUseAnimation();

        // 0.2秒后显示第二个特效
        scheduleOnce([this, itemIndex, amount](float dt) {
            showToolUseEffect(itemIndex);
            this->removeChildByTag(3000);       // 删除旧的标签

            std::string countText = "+" + std::to_string(amount);
            auto label = Label::createWithTTF(countText, "/fonts/arial.ttf", 13);

            if (label) {
                label->setPosition(_secondEffectPos.x + 25, _secondEffectPos.y - 17);
                label->setColor(Color3B::WHITE);
                label->enableOutline(Color4B::BLACK, 2);  // 黑色描边，更清晰
                label->setTag(3000);

                // 确保和特效使用相同的CameraMask
                if (_toolUseEffect) {
                    label->setCameraMask(_toolUseEffect->getCameraMask());
                    this->addChild(label, _toolUseEffect->getLocalZOrder() + 10);
                }
                else {
                    this->addChild(label, 100);
                }

                // 0.3秒后直接删除
                scheduleOnce([label](float dt) {
                    if (label) {
                        label->removeFromParent();
                    }
                    }, 0.3f, "remove_label");
            }
            }, 0.2f, "show_second_effect");

        // 发送事件通知背包刷新
        EventCustom event("INVENTORY_COUNT_CHANGED");
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
        // 更新预览框图片（更新数量）
        updatePreviewTool();
    }
}

// 减少指定物品的数量
void InventoryScene::removeIetmCount(Objects object, int amount)
{
    if (object >= Objects::HOE && object < Objects::COUNT) {


        // 1. 获取 PlayerState
        auto playerState = PlayerState::getInstance();
        auto& items = playerState->getInventoryItems();

        // 2. 查找并扣除
      
        for (auto item : items) {
            if (item->getTag() == object) {
                // 调用 PlayerState 的移除接口
                playerState->removeItem(item, amount);
                break; // 找到一个就退出
            }
        }

        // 3. 刷新界面
        if (_inventoryLayer) {
            _inventoryLayer->placeTools(-1); // 或者 refreshDisplay()
        }
    }

    // 发送事件通知背包刷新
    EventCustom event("INVENTORY_COUNT_CHANGED");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    // 更新预览框图片（更新数量）
    updatePreviewTool();
}
