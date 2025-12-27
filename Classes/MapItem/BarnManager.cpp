#include "BarnManager.h"


BarnManager* BarnManager::_instance = nullptr;


BarnManager* BarnManager::getInstance(GameMap* barn)
{
    // 获取或创建单例，首次需提供圈舍地图；后续忽略 barn 参数。
    if (!_instance) {
        if (!barn) {
            CCLOG("BarnManager::getInstance failed: barn map is null");
            return nullptr;
        }

        _instance = new (std::nothrow) BarnManager();
        if (_instance && _instance->init(barn)) {
            CC_SAFE_RETAIN(_instance);
        }
        else {
            CC_SAFE_DELETE(_instance);
        }
    }

    return _instance;
}


void BarnManager::destroyInstance()
{
    // 安全销毁单例并清理资源
    if (_instance) {
        _instance->clear();
        CC_SAFE_RELEASE_NULL(_instance);
    }
}


bool BarnManager::init(GameMap* barn)
{
    // 绑定地图与常用图层
    _gameMap = barn;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    _feedLayer = _tiledMap ? _tiledMap->getLayer("feeding") : nullptr;

    // 位置缓存与产物队列初始化
    _nestCenters.clear();
    _productionCenters.clear();

    _productions.clear();
    _productions.resize(5);

    _productionTileKeys.clear();
    _productionTileKeys.resize(5, 0);

    // 读取 1~5 号巢位与产物点位（依赖关卡对象）
    for (int i = 1; i <= 5; ++i) {
        std::string nestName = "nest" + std::to_string(i);
        std::string prodName = "production" + std::to_string(i);

        const Rect nr = _gameMap->getObjectRect(nestName);
        const Rect pr = _gameMap->getObjectRect(prodName);

        // 使用对象中心点作为落位
        _nestCenters.emplace_back(nr.getMidX(), nr.getMidY());
        _productionCenters.emplace_back(pr.getMidX(), pr.getMidY());

        Vec2 prodTile = _gameMap->calMapPos(_productionCenters.back());
        _productionTileKeys[i - 1] = keyFor(prodTile);
    }

    _animals.clear();
    _haySprites.clear();

    return _tiledMap != nullptr;
}


long long BarnManager::keyFor(const Vec2& tileCoord)
{
    // 将瓦片坐标打包为 64 位键以用于哈希表
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & 0xffffffffLL);
}


bool BarnManager::addHayAt(const Vec2& tileCoord)
{
    // 在可喂食图层有效瓦片上添加饲料
    if (!_feedLayer || !_tiledMap || !_gameMap) return false;

    Size s = _feedLayer->getLayerSize();
    if (s.width < 1 || s.height < 1) return false;
    if (tileCoord.x < 0 || tileCoord.x >= s.width ||
        tileCoord.y < 0 || tileCoord.y >= s.height) return false;

    unsigned int gid = _feedLayer->getTileGIDAt(tileCoord);
    if (gid == 0) return false;

    long long k = keyFor(tileCoord);
    if (_haySprites.find(k) != _haySprites.end()) return false;

    auto sp = Sprite::create("EnvironmentObjects/Hay.png");
    if (!sp) return false;

    Vec2 pos = _gameMap->calWorldPos(tileCoord);
    sp->setPosition(pos);
    _tiledMap->addChild(sp, 50);
    _gameMap->setCameraMask((unsigned short)CameraFlag::USER1, true);

    sp->retain();
    _haySprites[k] = sp;

    return true;
}


BarnAnimal* BarnManager::createAnimal(AnimalType type)
{
    // 创建指定类型的动物实例
    switch (type) {
    case AnimalType::CHICKEN:
        return Chicken::create();
    case AnimalType::COW:
    default:
        return Cow::create();
    }
}


bool BarnManager::addAnimal(AnimalType type)
{
    // 将动物放入第一个空闲巢位，并开始动画
    if (!_gameMap || !_tiledMap) return false;

    int idx = -1;
    for (int i = 0; i < 5; ++i) {
        if (i >= (int)_animals.size() || _animals[i] == nullptr) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return false;

    auto sp = createAnimal(type);
    if (!sp) return false;

    auto nestRect = _gameMap->getObjectRect("nest1");
    float offsetY = nestRect.size.height > 0 ? nestRect.size.height / 6 : 0.0f;
    Vec2 pos = _nestCenters[idx] + Vec2(0, offsetY);

    sp->setPosition(pos);
    _tiledMap->addChild(sp, 60);
    _gameMap->setCameraMask((unsigned short)CameraFlag::USER1, true);

    sp->startAnimation();
    sp->retain();

    if (idx < (int)_animals.size()) {
        _animals[idx] = sp;
    }
    else {
        _animals.resize(idx + 1);
        _animals[idx] = sp;
    }

    return true;
}


void BarnManager::onNewDay()
{
    // 新的一天：统计动物数与饲料数
    int m = 0;
    for (auto* a : _animals) {
        if (a) m++;
    }

    int n = (int)_haySprites.size();
    int consume = std::min(n, m);

    // 先消耗饲料
    if (consume > 0) {
        int consumed = 0;
        std::vector<long long> keys;
        keys.reserve(_haySprites.size());

        for (auto& kv : _haySprites) {
            keys.push_back(kv.first);
        }

        std::sort(keys.begin(), keys.end());

        for (auto k : keys) {
            if (consumed >= consume) break;

            auto sp = _haySprites[k];
            if (sp) {
                sp->removeFromParent();
                CC_SAFE_RELEASE(sp);
            }

            _haySprites.erase(k);
            consumed++;
        }
    }

    // 再生成产物（受动物数与饲料数共同限制）
    int produceCount = std::min(m, n);
    int produced = 0;

    for (int i = 0; i < 5 && produced < produceCount; ++i) {
        if (i >= (int)_animals.size() || !_animals[i]) continue;

        auto animal = _animals[i];
        auto path = animal->getProducePath();
        auto prod = Sprite::create(path);
        if (!prod) continue;

        prod->setPosition(_productionCenters[i]);
        _tiledMap->addChild(prod, 70);
        _gameMap->setCameraMask((unsigned short)CameraFlag::USER1, true);

        prod->retain();
        _productions[i].push_back({ prod, animal->getProduceObject() });
        produced++;
    }
}


void BarnManager::startAnimations()
{
    // 启动所有动物动画
    for (auto* a : _animals) {
        if (a) a->startAnimation();
    }
}


void BarnManager::stopAnimations()
{
    // 暂停所有动物动画
    for (auto* a : _animals) {
        if (a) a->stopAnimation();
    }
}


ItemType BarnManager::collectProductionAt(const Vec2& tileCoord)
{
    // 在指定产物点位收集一件产物
    if (!_gameMap || !_tiledMap) return ItemType::NONE;

    long long k = keyFor(tileCoord);

    for (int i = 0; i < 5; ++i) {
        if (_productionTileKeys[i] == k) {
            if (_productions[i].empty()) return ItemType::NONE;

            auto pair = _productions[i].front();
            _productions[i].erase(_productions[i].begin());

            if (pair.first) {
                pair.first->removeFromParent();
                CC_SAFE_RELEASE(pair.first);
            }

            return pair.second;
        }
    }

    return ItemType::NONE;
}


void BarnManager::clear()
{
    // 清理饲料精灵
    for (auto& kv : _haySprites) {
        if (kv.second) {
            kv.second->removeFromParent();
            CC_SAFE_RELEASE(kv.second);
        }
    }
    _haySprites.clear();

    // 清理动物
    for (auto* a : _animals) {
        if (a) {
            a->removeFromParent();
            CC_SAFE_RELEASE(a);
        }
    }
    _animals.clear();

    // 清理产物
    for (auto& list : _productions) {
        for (auto& pair : list) {
            if (pair.first) {
                pair.first->removeFromParent();
                CC_SAFE_RELEASE(pair.first);
            }
        }
        list.clear();
    }

    _productions.clear();
    _nestCenters.clear();
    _productionCenters.clear();
    _productionTileKeys.clear();
}
