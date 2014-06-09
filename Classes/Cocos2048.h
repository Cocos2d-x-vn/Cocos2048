#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__


#include "cocos2d.h"
#include "GameServer.h"

#define ZORDER_BG     0
#define ZORDER_NUMBER 1
#define ZORDER_FRAME  2

USING_NS_CC;

class Cocos2048 : public cocos2d::CCLayer
{
public:
    virtual bool init();  

    static cocos2d::CCScene* scene();
    void onEnter();
	void onExit();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	void addMoveActEnd(CCNode* pNode, void* pSprite);

    CREATE_FUNC(Cocos2048);

private:
	CCPoint    m_points[ROW][COL];
	GameServer m_gameServer;
	CCPoint    m_touchBeginPos;
	CCSize     m_blockSize;
	CCSprite*  m_sprites[ROW][COL];
	CCSprite*  getSprite(int num);

	void dealWithEvent();
	void setMoveRemoveAct(const CCPoint& target, CCSprite* pS);
	void setSpriteSize(CCSprite* pS);
	void moveEvent(Event_2048* pEvent);
	void addEvent(Event_2048* pEvent);
	void appearEvent(Event_2048* pEvent);
};

#endif // __HELLOWORLD_SCENE_H__
