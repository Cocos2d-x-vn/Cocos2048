#include "GameServer.h"

void GameServer::Restart()
{
	m_score    = 0;
	m_numEvent = 0;
	
	for(int i = 0 ; i<ROW ; i++)
		for(int j = 0 ;j<COL ; j++)
			m_save[i][j] = 0;

	//随机出两个数字
	int r0 = rand() % ROW;
	int c0 = rand() % COL;
	m_save[r0][c0] = 2 + rand() % 2 * 2;
	addEvent(ET_APPEAR,0,0,r0,c0);

	int r1 = rand() % ROW;
	int c1 = rand() % COL;
	if(r1 == r0 && c1==c0)
		r1 = (r0+1)%ROW;

	m_save[r1][c1] = 2 + rand() % 2 * 2;
	addEvent(ET_APPEAR,0,0,r1,c1);

	computeBlank();
}

void GameServer::computeBlank()
{
	m_numBlank = 0;

	for(int r = 0 ; r<ROW ; r++)
		for(int c = 0 ; c<COL ; c++)
			if(m_save[r][c]==0)
			{
				m_blank[m_numBlank].col = c;
				m_blank[m_numBlank].row = r;
				m_numBlank++;
			}

}

void GameServer::randNum()
{
	int r = rand() % m_numBlank;

	m_save[m_blank[r].row][m_blank[r].col] = 2 + rand()%2 * 2;

	addEvent(ET_APPEAR,0,0,m_blank[r].row,m_blank[r].col);
}

void GameServer::TouchMove(TouchMoveType tmt)
{
	m_numEvent = 0;

	switch(tmt)
	{
	case TMT_L2R:
		fromL2R();
		break;

	case TMT_R2L:
		fromR2L();
		break;

	case TMT_T2B:
		fromT2B();
		break;

	case TMT_B2T:
		fromB2T();
		break;
	}

	if(m_numEvent>0)
		computeBlank();

	if(m_numBlank>0 && m_numEvent>0)
		randNum();
}

void GameServer::fromR2L()
{
	for(int r = 0 ; r<ROW ; r++)
		for(int c = 0 ; c<COL-1 ; c++)
			for(int index = c+1 ; index<COL ; index++)
				if(m_save[r][index]!=0)
				{
					if(m_save[r][c]==0)
					{
						m_save[r][c] = m_save[r][index];
						m_save[r][index] = 0;
						addEvent(ET_MOVE,r,index,r,c);
					}
					else if(m_save[r][index] == m_save[r][c])
					{
						m_save[r][c] <<= 1;
						m_save[r][index] = 0;
						addEvent(ET_ADD,r,index,r,c);
						break;
					}
					else
						break;
				}
}

void GameServer::fromL2R()
{
	for(int r = 0 ; r<ROW ; r++)
		for(int c = COL-1 ; c>0 ; c--)
			for(int index = c-1 ; index>=0 ; index--)
				if(m_save[r][index]!=0)
				{
					if(m_save[r][c]==0)
					{
						m_save[r][c] = m_save[r][index];
						m_save[r][index] = 0;
						addEvent(ET_MOVE,r,index,r,c);
					}
					else if(m_save[r][index] == m_save[r][c])
					{
						m_save[r][c] <<= 1;
						m_save[r][index] = 0;
						addEvent(ET_ADD,r,index,r,c);
						break;
					}
					else
						break;
				}
}

void GameServer::fromT2B()
{
	for(int c = 0 ; c<COL ; c++)
		for(int r = ROW-1 ; r>0 ; r--)
			for(int index = r-1 ; index>=0 ; index--)
				if(m_save[index][c]!=0)
				{
					if(m_save[r][c]==0)
					{
						m_save[r][c] = m_save[index][c];
						m_save[index][c] = 0;
						addEvent(ET_MOVE,index,c,r,c);
					}
					else if(m_save[index][c] == m_save[r][c])
					{
						m_save[r][c] <<= 1;
						m_save[index][c] = 0;
						addEvent(ET_ADD,index,c,r,c);
						break;
					}
					else
						break;
				}
}

void GameServer::fromB2T()
{
	for(int c = 0 ; c<COL ; c++)
		for(int r = 0 ; r<ROW-1 ; r++)
			for(int index = r+1 ; index<ROW ; index++)
				if(m_save[index][c]!=0)
				{
					if(m_save[r][c]==0)
					{
						m_save[r][c] = m_save[index][c];
						m_save[index][c] = 0;
						addEvent(ET_MOVE,index,c,r,c);
					}
					else if(m_save[index][c] == m_save[r][c])
					{
						m_save[r][c] <<= 1;
						m_save[index][c] = 0;
						addEvent(ET_ADD,index,c,r,c);
						break;
					}
					else
						break;
				}
}

void GameServer::addEvent(Event_Type_2048 type,int sourceR,int sourceC,int targetR,int targetC)
{
	if(type==ET_ADD)
	{
	   //如果之前格子空白，将一个数字移到空白处
	   //现在再与另一个数字相加，就产生了一个多余的移动事件，去除它
	   if(m_numEvent-1>=0 &&
		   m_event[m_numEvent-1].coordTarget.row == targetR &&
		   m_event[m_numEvent-1].coordTarget.col == targetC)
	   {
		    m_event[m_numEvent-1].type = type;

			m_event[m_numEvent-1].coordSource1.row = sourceR;
			m_event[m_numEvent-1].coordSource1.col = sourceC;

			m_event[m_numEvent-1].coordTarget.row = targetR;
			m_event[m_numEvent-1].coordTarget.col = targetC;
	   }
	   else
	   {
		    m_event[m_numEvent].type = type;

			m_event[m_numEvent].coordSource0.row = sourceR;
			m_event[m_numEvent].coordSource0.col = sourceC;

			m_event[m_numEvent].coordSource1.row = targetR;
			m_event[m_numEvent].coordSource1.col = targetC;

			m_event[m_numEvent].coordTarget.row = targetR;
			m_event[m_numEvent].coordTarget.col = targetC;

			m_numEvent++;
	   }
	}
		
	else
	{
		m_event[m_numEvent].type = type;
		m_event[m_numEvent].coordSource0.row = sourceR;
		m_event[m_numEvent].coordSource0.col = sourceC;

		m_event[m_numEvent].coordTarget.row = targetR;
		m_event[m_numEvent].coordTarget.col = targetC;

		m_numEvent++;
	}
}