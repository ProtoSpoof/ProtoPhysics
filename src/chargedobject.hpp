#pragma once

#include "physicsobject.hpp"

class ChargedObject: public PhysicsObject {
    private:
    double mCharge;

    public:
    double getCharge();
    

};