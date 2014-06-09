/**
 *@file ��GameServer��������һЩ��������
 */
#ifndef __GAME_SERVER_H__
#define __GAME_SERVER_H__

#include <stdlib.h>

#define ROW 4
#define COL 4
#define AMOUNT ROW*COL

///���ӵ�����
struct Coord
{
	int row;
	int col;
};

///���ƻ����ķ���
enum TouchMoveType{TMT_L2R = 0,TMT_R2L,TMT_T2B,TMT_B2T};

///�ƶ�������ӻ��µ����ֳ���
enum Event_Type_2048{ET_MOVE,ET_ADD,ET_APPEAR};

///�������¼�
struct Event_2048
{
	Event_Type_2048 type;
	Coord coordSource0;
	Coord coordSource1;
	Coord coordTarget;
};

/**
 *@brief 2048���߼����֣���ɳ�����ʾ֮���һ��
 *�ö�ά���鱣�����ݣ��ṩ�����������Ƶ��߼�����
 */
class GameServer
{
public:
	void Restart();
	void TouchMove(TouchMoveType);

	int GetSave(int r,int c)   {return m_save[r][c];}

	///�õ������ϴ������¼��������¼�
	Event_2048*  GetEvents()   {return m_event;}
	int GetEventAmount()	   {return m_numEvent;}
private:
	int   m_score;
	int   m_save[ROW][COL];     ///<������������ϵ���ֵ
	Coord m_blank[AMOUNT];      ///<����û�����ֵĸ��ӣ����������������
	int   m_numBlank;		    ///<û�����ֵĸ��ӵ���Ŀ
	Event_2048 m_event[AMOUNT]; ///<���ƻ����������¼�
	int   m_numEvent;		    ///<�¼���Ŀ

	void randNum();
	void computeBlank();
	void fromR2L();
	void fromL2R();
	void fromT2B();
	void fromB2T();
	void addEvent(Event_Type_2048 type,int,int,int,int);
};

#endif