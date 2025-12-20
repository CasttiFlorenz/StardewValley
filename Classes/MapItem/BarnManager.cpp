#include "BarnManager.h"
#include <algorithm>

BarnManager* BarnManager::_instance = nullptr;

BarnManager* BarnManager::getInstance() {
    if (!_instance) {
        _instance = new BarnManager();
        _instance->autorelease();
        CC_SAFE_RETAIN(_instance);
    }
    return _instance;
}

void BarnManager::destroyInstance() {
    CC_SAFE_RELEASE_NULL(_instance);
}

bool BarnManager::init(GameMap* barn) {
    _gameMap = barn;
    _tiledMap = _gameMap ? _gameMap->getTiledMap() : nullptr;
    _feedLayer = _tiledMap ? _tiledMap->getLayer("feed") : nullptr;
    _nestCenters.clear();
    _productionCenters.clear();
    _productions.clear();
    _productions.resize(5);
    _productionTileKeys.clear();
    _productionTileKeys.resize(5, 0);
    for (int i = 1; i <= 5; ++i) {
        std::string nestName = "nest" + std::to_string(i);
        std::string prodName = "production" + std::to_string(i);
        Rect nr = _gameMap->getObjectRect(nestName);
        Rect pr = _gameMap->getObjectRect(prodName);
        _nestCenters.push_back(Vec2(nr.getMidX(), nr.getMidY()));
        _productionCenters.push_back(Vec2(pr.getMidX(), pr.getMidY()));
        Vec2 prodTile = _gameMap->calMapPos(_productionCenters.back());
        _productionTileKeys[i - 1] = keyFor(prodTile);
    }
    _animals.clear();
    _haySprites.clear();
    auto tm = TimeManager::getInstance();
    if (tm) {
        auto prev = tm->onDayStartCallback;
        tm->onDayStartCallback = [this, prev]() { if (prev) prev(); this->onNewDay(); };
    }
    return _tiledMap != nullptr;
}

long long BarnManager::keyFor(const Vec2& tileCoord) {
    long long x = static_cast<long long>(tileCoord.x);
    long long y = static_cast<long long>(tileCoord.y);
    return (x << 32) | (y & 0xffffffffLL);
}

bool BarnManager::addHayAt(const Vec2& tileCoord) {
    if (!_feedLayer || !_tiledMap || !_gameMap) return false;
    Size s = _feedLayer->getLayerSize();
    if (s.width < 1 || s.height < 1) return false;
    if (tileCoord.x < 0 || tileCoord.x >= s.width || tileCoord.y < 0 || tileCoord.y >= s.height) return false;
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

BarnAnimal* BarnManager::createAnimal(BarnAnimalType type) {
    if (type == BarnAnimalType::CHICKEN) return Chicken::create();
    return Cow::create();
}

bool BarnManager::addAnimal(BarnAnimalType type) {
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
    Vec2 pos = _nestCenters[idx] + Vec2(0, _gameMap->getObjectRect("nest1").size.height / 6);
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

void BarnManager::onNewDay() {
    int m = 0;
    for (auto* a : _animals) {
        if (a) m++;
    }
    int n = (int)_haySprites.size();
    int consume = std::min(n, m);
    if (consume > 0) {
        int consumed = 0;
        std::vector<long long> keys;
        keys.reserve(_haySprites.size());
        for (auto& kv : _haySprites) keys.push_back(kv.first);
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

void BarnManager::startAnimations() {
    for (auto* a : _animals) {
        if (a) a->startAnimation();
    }
}

void BarnManager::stopAnimations() {
    for (auto* a : _animals) {
        if (a) a->stopAnimation();
    }
}

Objects BarnManager::collectProductionAt(const Vec2& tileCoord) {
    if (!_gameMap || !_tiledMap) return Objects::NONE;
    long long k = keyFor(tileCoord);
    for (int i = 0; i < 5; ++i) {
        if (_productionTileKeys[i] == k) {
            if (_productions[i].empty()) return Objects::NONE;
            auto pair = _productions[i].front();
            _productions[i].erase(_productions[i].begin());
            if (pair.first) {
                pair.first->removeFromParent();
                CC_SAFE_RELEASE(pair.first);
            }
            return pair.second;
        }
    }
    return Objects::NONE;
}

