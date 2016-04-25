/*
 * xTypeID.cpp
 *
 *  Created on: 2012-11-14
 *      Author: Peter_ZHA
 */
#include "Language/TypeID.h"


UINT GetNewID()
{
    static UINT ID = 0;
    ++ID;
    return ID;
}

