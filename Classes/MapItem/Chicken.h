#pragma once
#ifndef __CHICKEN_H__
#define __CHICKEN_H__

#include "BarnAnimal.h"

class Chicken : public BarnAnimal {
public:
    static Chicken* create();
    std::string getProducePath() const override;
    Objects getProduceObject() const override;
    void startAnimation() override;
    void stopAnimation() override;
};

#endif // __CHICKEN_H__
