#include "DialogueLayer.h"

USING_NS_CC;
using namespace ui;

DialogueLayer* DialogueLayer::create() {
    DialogueLayer* pRet = new(std::nothrow) DialogueLayer();
    if (pRet && pRet->init()) { pRet->autorelease(); return pRet; }
    delete pRet; return nullptr;
}
bool DialogueLayer::init() {
    if (!Layer::init()) return false;
    Size winSize = Director::getInstance()->getWinSize();

    // 1. 吞噬触摸
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch*, Event*) {
        if (_choiceNode->isVisible()) {
            return true; // 吞噬触摸，不作反应
        }

        // 如果是【普通对话模式】，点击屏幕 = Continue (下一句)
        showNextSentence();
        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 2. 背景框
    _bg = Scale9Sprite::create("CreateScene/textBox.png");
    _bg->setContentSize(Size(800, 200));
    _bg->setPosition(winSize.width / 2, 150);
    this->addChild(_bg);

    // 3. 名字与内容
    _nameLabel = Label::createWithTTF("", "fonts/arial.ttf", 28);
    _nameLabel->setPosition(100, 160);
    _nameLabel->setColor(Color3B(100, 50, 20));
    _bg->addChild(_nameLabel);

    _contentLabel = Label::createWithTTF("", "fonts/arial.ttf", 24);
    _contentLabel->setPosition(400, 80);
    _contentLabel->setDimensions(700, 100);
    _contentLabel->setColor(Color3B::BLACK);
    _bg->addChild(_contentLabel);

    // 4. 选择按钮节点
    _choiceNode = Node::create();
    _bg->addChild(_choiceNode);
    _choiceNode->setVisible(false);

    // 创建选择按钮
    createChoiceButtons();

    return true;
}

void DialogueLayer::createChoiceButtons() {
    // YES 按钮
    auto btnYes = Button::create();
    btnYes->setTitleText("YES");
    btnYes->setTitleFontSize(30);
    btnYes->setTitleColor(Color3B::WHITE);
    btnYes->ignoreContentAdaptWithSize(false);
    btnYes->setContentSize(Size(100, 50));

    // 添加背景色
    auto yesBg = LayerColor::create(Color4B(0, 128, 0, 200), 100, 50); // 绿色背景
    yesBg->setPosition(0, 0);
    btnYes->addChild(yesBg, -1);

    btnYes->setPosition(Vec2(600, 50));

    // 修改后的点击监听逻辑
    btnYes->addClickEventListener([this](Ref* sender) {
        CCLOG("YES button clicked");

        // 1. 禁用按钮，防止多次点击触发重复回调
        static_cast<ui::Button*>(sender)->setEnabled(false);

        // 2. 提升引用计数，防止回调过程中对象被析构
        this->retain();

        // 3. 执行回调逻辑
        this->onChoiceMade(true);

        // 4. 安全检查：如果对象还没被逻辑删除，则将其从父节点移除
        if (this->getParent()) {
            this->removeFromParent();
        }

        // 5. 对应上面的 retain，释放引用
        this->release();
        });

    _choiceNode->addChild(btnYes);

    // --- NO 按钮部分 ---
    auto btnNo = Button::create();
    btnNo->setTitleText("NO");
    btnNo->setTitleFontSize(30);
    btnNo->setTitleColor(Color3B::WHITE);
    btnNo->ignoreContentAdaptWithSize(false);
    btnNo->setContentSize(Size(100, 50));

    // 添加背景色
    auto noBg = LayerColor::create(Color4B(200, 0, 0, 200), 100, 50);
    noBg->setPosition(0,0);
    btnNo->addChild(noBg, -1);

    btnNo->setPosition(Vec2(700, 50));

    // 修改点击回调逻辑
    btnNo->addClickEventListener([this](Ref* sender) {
        CCLOG("NO button clicked");

        // 1. 禁用按钮，防止连点导致多次进入回调
        static_cast<ui::Button*>(sender)->setEnabled(false);

        // 2. 增加一次 retain，确保在回调执行期间 this 内存不会被意外回收
        this->retain();

        // 3. 执行逻辑回调
        this->onChoiceMade(false);

        // 4. 安全移除：检查 parent 是否还存在，防止二次移除
        if (this->getParent()) {
            this->removeFromParent();
        }

        // 5. 对应上面的 retain，释放引用计数
        this->release();
        });

    _choiceNode->addChild(btnNo);
}


// 处理选择结果
void DialogueLayer::onChoiceMade(bool choice) {
    if (_choiceCallback) {
        _choiceCallback(choice);  // 回调给调用者
    }
    else {
        CCLOG("Warning: No choice callback set!");
    }
}

// 显示选择对话框
void DialogueLayer::showChoice(const std::string& content, ChoiceCallback callback) {
    // 清空对话队列
    while (!_dialogueQueue.empty()) _dialogueQueue.pop();

    _nameLabel->setString("");
    _contentLabel->setString(content);

    // 保存回调函数
    _choiceCallback = callback;

    _choiceNode->setVisible(true);
}
void DialogueLayer::showText(const std::string& name, const std::vector<std::string>& contentList) {
    _nameLabel->setString(name);
    _choiceNode->setVisible(false);

    while (!_dialogueQueue.empty()) {
        _dialogueQueue.pop();
    }
    for (const auto& str : contentList) {
        _dialogueQueue.push(str);
    }
    showNextSentence();
}
    
void DialogueLayer::showNextSentence() {
    // 如果队列空了，说明话说完了 -> 关闭对话框                                               
    if (_dialogueQueue.empty()) {
        this->removeFromParent();
        return;
    }

    // 取出下一句
    std::string text = _dialogueQueue.front();
    _dialogueQueue.pop(); // 弹出

    // 更新文本显示
    _contentLabel->setString(text);
}