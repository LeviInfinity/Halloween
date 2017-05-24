#pragma once
#include"cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
class WelcomeScene:public cocos2d::Layer
{
public:
	static cocos2d::Scene * createScene();
	virtual bool init();
	virtual void onStartItemCallback(Ref * pSender);
	virtual void onExitItemCallback(Ref * pSender);
	virtual void onSetItemCallback(Ref * pSender);
	CREATE_FUNC(WelcomeScene);

};