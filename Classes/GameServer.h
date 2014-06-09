/**
 *@file 放GameServer的声明和一些补助定义
 */
#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include <stdlib.h>

#define ROW 4
#define COL 4
#define AMOUNT ROW*COL

///格子的坐标
struct Coord
{
	int row;
	int col;
};

///手势滑动的方向
enum TouchMoveType{TMT_L2R = 0,TMT_R2L,TMT_T2B,TMT_B2T};

///移动或者相加或新的数字出现
enum Event_Type_2048{ET_MOVE,ET_ADD,ET_APPEAR};

///发生的事件
struct Event_2048
{
	Event_Type_2048 type;
	Coord coordSource0;
	Coord coordSource1;
	Coord coordTarget;
};

/**
 *@brief 2048的逻辑部分，完成除了显示之外的一切
 *用二维数组保存数据，提供上下左右手势的逻辑处理
 */
class GameServer
{
public:
	void Restart();
	void TouchMove(TouchMoveType);

	int GetSave(int r,int c)   {return m_save[r][c];}

	///得到的是上次手势事件发生的事件
	Event_2048*  GetEvents()   {return m_event;}
	int GetEventAmount()	   {return m_numEvent;}
private:
	int   m_score;
	int   m_save[ROW][COL];     ///<保存各个格子上的数值
	Coord m_blank[AMOUNT];      ///<保存没有数字的格子，用于随机出现数字
	int   m_numBlank;		    ///<没有数字的格子的数目
	Event_2048 m_event[AMOUNT]; ///<手势滑动后发生的事件
	int   m_numEvent;		    ///<事件数目

	void randNum();
	void computeBlank();
	void fromR2L();
	void fromL2R();
	void fromT2B();
	void fromB2T();
	void addEvent(Event_Type_2048 type,int,int,int,int);
};

#endif