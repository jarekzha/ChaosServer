/*
 * PlayerOfflineHandler.h
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#pragma once

class PlayerOfflineHandler
{
public:
                        PlayerOfflineHandler():handleInProgress(false){}
    virtual             ~PlayerOfflineHandler(){}

    void                HandleOffline()
    {
        if (!handleInProgress)
        {
            OnOffline();
        }
    }

    bool                HandleInProgress()
    {
        return handleInProgress;
    }

protected:
    virtual void        OnOffline() = 0;

private:
    bool                handleInProgress;
};

