#include"cocos2d.h"
#include"SpriteShape.h"
#include"Define.h"
USING_NS_CC;
SpriteShape::SpriteShape() :m_isNeedRemove(false)
{

}
SpriteShape* SpriteShape::create(int row, int col)
{
	SpriteShape *spr = new SpriteShape();
	int randNum = rand() % 20;
	spr->m_row = row;
	spr->m_col = col;
	if (randNum == 0)//20��֮1�ĸ��ʳ�����ֻ��
	{
		spr->m_imgIndex = 8;
		spr->initWithSpriteFrameName(spriteNormal[8]);
	}
	else
	{
		spr->m_imgIndex = rand() % TOTAL_SPRITE;//10
		spr->initWithSpriteFrameName(spriteNormal[spr->m_imgIndex]);	//ͨ��index����ͼƬ
	}
	spr->m_isNeedRemove = false;
	spr->setRow(row);
	spr->setCol(col);
	spr->autorelease();
	return spr;
}

