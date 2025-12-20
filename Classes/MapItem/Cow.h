#pragma once
#ifndef __COW_H__
#define __COW_H__

#include "BarnAnimal.h"

class Cow : public BarnAnimal {
public:
    static Cow* create();
    std::string getProducePath() const override;
    Objects getProduceObject() const override;
    void startAnimation() override;
    void stopAnimation() override;
};

#endif // __COW_H__
