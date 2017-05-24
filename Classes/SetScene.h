#pragma once
#include"cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
class SetScene :public cocos2d::Layer
{
public:
	
	static cocos2d::Scene * createScene();
	virtual bool init();
	void onOpenMusicCallback(Ref * pSender);
	void onCloseMusicCallback(Ref * pSender);
	void onOpenEffectCallback(Ref * pSender);
	void onCloseEffectCallback(Ref * pSender);
	void onBackItemCallback(Ref * pSender);
	static bool isEffectPlay;
	static bool isMusicPlay;
	CREATE_FUNC(SetScene);

};
