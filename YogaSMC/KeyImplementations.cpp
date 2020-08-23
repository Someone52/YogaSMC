//
//  KeyImplementations.cpp
//  YogaSMC
//
//  Created by Zhen on 7/28/20.
//  Copyright © 2020 Zhen. All rights reserved.
//

#include "KeyImplementations.hpp"
#include "YogaSMC.hpp"

SMC_RESULT BDVT::readAccess() {
    bool *ptr = reinterpret_cast<bool *>(data);
    YogaSMC *drv = OSDynamicCast(YogaSMC, dst);
    if (drv) {
        drv->dispatchMessage(kSMC_getConservation, ptr);
        DBGLOG("vpckey", "BDVT read %d -> %d", status, *ptr);
        status = *ptr;
    } else {
        *ptr = status;
        DBGLOG("vpckey", "BDVT read %d (no drv)", status);
    }
    return SmcSuccess;
}

SMC_RESULT BDVT::writeAccess() {
    bool *ptr = reinterpret_cast<bool *>(data);
    YogaSMC *drv = OSDynamicCast(YogaSMC, dst);
    // update internal status first
    if (drv) {
        drv->dispatchMessage(kSMC_getConservation, &status);
        DBGLOG("vpckey", "BDVT update %d", status);
    }
    DBGLOG("vpckey", "BDVT write %d -> %d", *ptr, status);
    if (status != *ptr) {
        if (drv) {
            drv->dispatchMessage(kSMC_setConservation, ptr);
            drv->dispatchMessage(kSMC_getConservation, &status);
            DBGLOG("vpckey", "BDVT update %d", status);
            *ptr = status;
        } else {
            status = *ptr;
        }
    }
    return SmcSuccess;
}

SMC_RESULT CH0B::readAccess() {
    data[0] = value;
    DBGLOG("vpckey", "CH0B read %d", value);
    return SmcSuccess;
}

SMC_RESULT CH0B::writeAccess() {
    value = data[0];
    DBGLOG("vpckey", "CH0B write %d", data[0]);
    return SmcSuccess;
}
