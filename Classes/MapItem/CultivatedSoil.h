#pragma once
#ifndef __CULTIVATED_SOIL_H__
#define __CULTIVATED_SOIL_H__

#include "EnvironmentItem.h"
#include "Crop.h"

class CultivatedSoil : public EnvironmentItem {
public:
    static CultivatedSoil* create(const cocos2d::Vec2& tileCoord);
    bool init(const cocos2d::Vec2& tileCoord);

    void water();
    void dry();
    bool plant(CropType type);
    CropType harvest();
    void updateDay();

    SoilStatus getStatus() const { return _status; }
    Crop* getCrop() const { return _crop; }
    bool hasCrop() const { return _crop != nullptr; }

private:
    SoilStatus _status;
    Crop* _crop;
};

#endif // __CULTIVATED_SOIL_H__
