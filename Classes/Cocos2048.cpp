#include "Cocos2048.h"

USING_NS_CC;

CCScene* Cocos2048::scene()
{
    CCScene *scene = CCScene::create();
    Cocos2048 *layer = Cocos2048::create();

    scene->addChild(layer);

    return scene;
}

bool Cocos2048::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize wndSize = CCDirector::sharedDirector()->getVisibleSize();

	m_blockSize = CCSize(wndSize.height/4,wndSize.height/4);

	CCPoint base(wndSize.height/8,wndSize.height/8);

    for(int i = 0 ; i < ROW ; i++)
		for(int j = 0 ;j<COL ; j++)
		{
			m_sprites[i][j] = 0;
			m_points[i][j]  = ccp(base.x+j*m_blockSize.height,base.y+(3-i)*m_blockSize.width);
		}

	CCSprite* pBg = CCSprite::create("bg.png");
	CCSize size = pBg->getContentSize();
	pBg->setScaleX(wndSize.width  / size.width);
	pBg->setScaleY(wndSize.height / size.height);
	pBg->setAnchorPoint(ccp(0.0f,0.0f));
	pBg->setPosition(ccp(0.0f,0.0f));
	this->addChild(pBg,ZORDER_BG);

	CCSprite* pFrame = CCSprite::create("frame.png");
	 size = pFrame->getContentSize();
	pFrame->setScaleX(wndSize.height  / size.width);
	pFrame->setScaleY(wndSize.height / size.height);
	pFrame->setAnchorPoint(ccp(0.0f,0.0f));
	pFrame->setPosition(ccp(0.0f,0.0f));
	this->addChild(pFrame,ZORDER_FRAME);

	m_gameServer.Restart();
	dealWithEvent();

    return true;
}


CCSprite* Cocos2048::getSprite(int num)
{
	char fn[10];
	sprintf(fn,"%d.png",num);

	return CCSprite::create(fn);
}

void Cocos2048::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
}

void Cocos2048::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool Cocos2048::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	m_touchBeginPos = pTouch->getLocation();
	return true;
}

void Cocos2048::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint endPos = pTouch->getLocation();

	if(endPos.x - m_touchBeginPos.x > 50)
	{
		m_gameServer.TouchMove(TMT_L2R);
		dealWithEvent();
	}
	else if(endPos.x - m_touchBeginPos.x < -50)
	{
		m_gameServer.TouchMove(TMT_R2L);
		dealWithEvent();
	}
	else if(endPos.y - m_touchBeginPos.y > 50)
	{
		m_gameServer.TouchMove(TMT_B2T);
		dealWithEvent();
	}
	else if(endPos.y - m_touchBeginPos.y < -50)
	{
		m_gameServer.TouchMove(TMT_T2B);
		dealWithEvent();
	}
}

void Cocos2048::dealWithEvent()
{
	Event_2048* events = m_gameServer.GetEvents();
	int num			   = m_gameServer.GetEventAmount();

	for(int i = 0 ; i<num ; i++)
	{
		switch(events[i].type)
		{
		case ET_APPEAR:
			appearEvent(events+i);
			break;
		case ET_MOVE:
			moveEvent(events+i);
			break;
		case ET_ADD:
			addEvent(events+i);
			break;
		default:
			break;
		}
	}
}

void Cocos2048::addMoveActEnd(CCNode* pNode, void* pSprite)
{
	this->removeChild((CCNode*)pSprite);
}

void Cocos2048::moveEvent(Event_2048* pEvent)
{
	int rowS = pEvent->coordSource0.row;
	int colS = pEvent->coordSource0.col;
	int rowT = pEvent->coordTarget.row;
	int colT = pEvent->coordTarget.col;

	CCPoint   pos   = m_points[rowT][colT];
	CCSprite* pS    = m_sprites[rowS][colS];
	CCAction* pMove = CCMoveTo::create(0.1f,pos);
	pS->runAction(pMove);

	m_sprites[rowS][colS] = 0;
	m_sprites[rowT][colT] = pS;
}

void Cocos2048::setMoveRemoveAct(const CCPoint& target, CCSprite* pS)
{
	CCFiniteTimeAction* pS0Move     = CCMoveTo::create(0.1f,target);

	CCFiniteTimeAction* pS0CallFunc = CCCallFuncND::create(this,
														   callfuncND_selector(Cocos2048::addMoveActEnd),
														   pS);

	pS->runAction(CCSequence::create(pS0Move,pS0CallFunc,NULL));
}

void Cocos2048::addEvent(Event_2048* pEvent)
{
	int rowS0 = pEvent->coordSource0.row;
	int colS0 = pEvent->coordSource0.col;
	int rowS1 = pEvent->coordSource1.row;
	int colS1 = pEvent->coordSource1.col;
	int rowT  = pEvent->coordTarget.row;
	int colT  = pEvent->coordTarget.col;

	setMoveRemoveAct(m_points[rowT][colT],m_sprites[rowS0][colS0]);
	m_sprites[rowS0][colS0] = 0;

	setMoveRemoveAct(m_points[rowT][colT],m_sprites[rowS1][colS1]);
	m_sprites[rowS1][colS1] = 0;

	CCSprite* pS = getSprite(m_gameServer.GetSave(rowT,colT));
	pS->setPosition(m_points[rowT][colT]);
	setSpriteSize(pS);

	m_sprites[rowT][colT] = pS;
	this->addChild(pS,ZORDER_NUMBER);
}

void Cocos2048::appearEvent(Event_2048* pEvent)
{
	int rowT = pEvent->coordTarget.row;
	int colT = pEvent->coordTarget.col;

	CCSprite* pS = getSprite(m_gameServer.GetSave(rowT,colT));
	pS->setPosition(m_points[rowT][colT]);
	setSpriteSize(pS);

	m_sprites[rowT][colT] = pS;
	pS->setVisible(false);
	CCFiniteTimeAction* pAct0 = CCDelayTime::create(0.1f);
	CCFiniteTimeAction* pAct1 = CCShow::create();
	CCFiniteTimeAction* pAct2 = CCFadeIn::create(0.2f);
			
	pS->runAction(CCSequence::create(pAct0,pAct1,pAct2,NULL));
	this->addChild(pS,ZORDER_NUMBER);
}

void Cocos2048::setSpriteSize(CCSprite* pS)
{
	CCSize  size = pS->getContentSize();
	pS->setScaleX(m_blockSize.width  / size.width);
	pS->setScaleY(m_blockSize.height / size.height);
}