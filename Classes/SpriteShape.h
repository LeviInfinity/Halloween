#pragma once
#include"cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
class SpriteShape :public cocos2d::Sprite
{
public:
	SpriteShape();
	static SpriteShape * create(int row, int col);
	CC_SYNTHESIZE(int, m_row, Row);
	CC_SYNTHESIZE(int, m_col, Col);
	CC_SYNTHESIZE(int, m_imgIndex, ImgIndex);
	CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);
	CREATE_FUNC(SpriteShape);

};
