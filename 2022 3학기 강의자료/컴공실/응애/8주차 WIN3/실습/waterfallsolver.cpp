#include "stdafx.h"
#include "mylist.h"
#include "waterfallsolver.h"

#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable:4996)


Line *mLine = NULL;
Point *mPoint = NULL;
int mLine_num;
int mPoint_num;

mylist* m_flow_point = NULL;

bool init_state = false;
bool draw_state = false;
bool sele_state = false;
int  curr_point = 0;

static void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void init_data(void){
	if(mLine!=NULL) free(mLine);
	if(mPoint!=NULL) free(mPoint);
	if(m_flow_point!=NULL) delete m_flow_point;
	mLine = NULL;
	mPoint = NULL;
	m_flow_point = NULL;

	mLine_num = 0;
	mPoint_num = 0;
	init_state = false;
	draw_state = false;
	sele_state = false;
	curr_point = 0;
}

void data_read(LPCTSTR fname){
	FILE* file;
	int i;

	file=fopen(fname,"r");

	/* line read */
	///////////////////////////////////////한줄씩 데이터를 읽어 mLine[i].xl mLine[i].yl mLine[i].xr mLine[i].yr에 값을 넣는다.
	fscanf(file, "%d", &mLine_num);
	mLine = (Line *)malloc(sizeof(Line)*mLine_num);
	for(i = 0; i < mLine_num; i++) {
		fscanf(file, "%d%d%d%d", &mLine[i].xl, &mLine[i].yl, &mLine[i].xr, &mLine[i].yr);
		// 항상 xl < xr이도록 유지한다
		if(mLine[i].xl > mLine[i].xr) {
			swap(&mLine[i].xl, &mLine[i].xr);
			swap(&mLine[i].yl, &mLine[i].yr);
		}
	}

	/* point read */
	//////////////////////////////////////한줄씩 데이터를 읽어 mPoint[i].x mPoint[i].y에 값을 저장한다.
	fscanf(file, "%d", &mPoint_num);
	mPoint = (Point *)malloc(sizeof(Point)*mPoint_num);
	for(i = 0; i < mPoint_num; i++)
		fscanf(file, "%f%f", &mPoint[i].x, &mPoint[i].y);

	fclose(file);
	init_state = true;
}

/* 문제는 물 그릴때 푼다. 아래의 drawWaterflow() 내에서 호출한다. */
void waterfall_Solver(void){
	Point S;
	Line *minP, *P;
	int i, j;
	float tslew, ty, cy, minlv, templv;

	if(init_state==false) return;

	if(m_flow_point!=NULL) delete m_flow_point;
	m_flow_point = new mylist();

	S.x = mPoint[curr_point].x;
	S.y = mPoint[curr_point].y;
	m_flow_point->add(&S);

	/// S에서 수직으로 내린 직선이 만나는 선분 없이 바닥까지 닿을 때까지 S를 이동시킨다.
	while(1) {
		/// 모든 선분에 대하여 S에서 수직으로 내린 직선과 교점이 있으면 찾는다.
		/// 그 교점 가운데 S보다 낮으면서 가장 높은 점의 y 좌표를 cy에 저장한다. 이 점을 포함하는 선분의 index를 j에 저장한다.
		cy = 0;
		for(i = 0; i < mLine_num; i++) {
			/// x값의 범위에 따라 교점의 유무를 판단한다
			if(!(mLine[i].xl <= S.x && S.x <= mLine[i].xr))
				continue;

			/// 점 (xl, yl)과 (xr, yr)를 잇는 직선의 방정식은 y - yr = ((yr - yl)/(xr - xl)) *(x - xr)
			/// 이 직선과 직선 x = S.x와의 교점은 ( S.x, ((yr - yl)/(xr - xl)) * (S.x - xr) + yr )
			ty = ((1.0f*mLine[i].yr - mLine[i].yl)/(mLine[i].xr - mLine[i].xl))
			     *(S.x - mLine[i].xr) + mLine[i].yr;

			/// 교점이 S보다 위에 있는 경우 무시한다
			if(ty >= S.y)
				continue;

			/// 이미 알고 있던 교점(cy)보다 이 교점(ty)이 더 높다면 cy를 갱신
			if(ty > cy) {
				cy = ty;
				j = i;
			}
		}

		/// 초기값이 변경되지 않음 = S에서 수직으로 내린 직선과 만나는 선분이 없음
		if(cy == 0)
			break;

		/// S를 교점으로 옮기고 선분을 연결
		S.y = cy;
		m_flow_point->add(&S);

		/// 해당 선분의 양 끝점 중 더 낮은 쪽까지 선분을 연결
		if(mLine[j].yl < mLine[j].yr) {
			S.x = (float)mLine[j].xl;
			S.y = (float)mLine[j].yl;
		}
		else {
			S.x = (float)mLine[j].xr;
			S.y = (float)mLine[j].yr;
		}
		m_flow_point->add(&S);
	}

	/// S를 바닥까지 떨어뜨리고 선분을 연결
	S.y = 0; m_flow_point->add(&S);
}

void drawBackground(CDC* pDC){
	int i; CPen MyPen;

	if(init_state==false) return;

	MyPen.CreatePen(PS_SOLID, 10, RGB(0,0,154));
	pDC->SelectObject(MyPen);
	pDC->MoveTo(gXmin, gYmin); pDC->LineTo(gXmax, gYmin);
	pDC->MoveTo(gXmin, gYmax); pDC->LineTo(gXmax, gYmax);

	MyPen.DeleteObject(); MyPen.CreatePen(PS_SOLID, 5, RGB(100,0,0));
	pDC->SelectObject(MyPen);
	for(i=0;i<mLine_num;i++){
		pDC->MoveTo(gXmin + mLine[i].xl*lineSize, gYmax - mLine[i].yl*lineSize);
		pDC->LineTo(gXmin + mLine[i].xr*lineSize, gYmax - mLine[i].yr*lineSize);
	}
	MyPen.DeleteObject(); MyPen.CreatePen(PS_SOLID, 10, RGB(0,0,0));
	pDC->SelectObject(MyPen);
	for(i=0;i<mPoint_num;i++){
		pDC->Ellipse(gXmin + (int)mPoint[i].x*lineSize - cirsize,
					 gYmax - (int)mPoint[i].y*lineSize - cirsize,
					 gXmin + (int)mPoint[i].x*lineSize + cirsize,
					 gYmax - (int)mPoint[i].y*lineSize + cirsize);
	}
}

void drawStartPoint(CDC* pDC){
	CPen MyPen;
	if(init_state==false) return;
	if(sele_state==false) return;

	MyPen.CreatePen(PS_SOLID, 10, RGB(255,0,0));
	pDC->SelectObject(MyPen);
	pDC->Ellipse(gXmin + (int)(mPoint[curr_point].x*lineSize) - cirsize,
				 gYmax - (int)(mPoint[curr_point].y*lineSize) - cirsize,
				 gXmin + (int)(mPoint[curr_point].x*lineSize) + cirsize,
				 gYmax - (int)(mPoint[curr_point].y*lineSize) + cirsize);
}

void drawWaterflow(CDC* pDC){
	node* temp;
	CPen MyPen;
	if(init_state==false) return;
	if(sele_state==false) return;
	if(draw_state==false) return;

	MyPen.CreatePen(PS_SOLID, 5, RGB(0,0,255));
	pDC->SelectObject(MyPen);
	temp = m_flow_point->move_first();
	while(temp->next!=NULL){
		pDC->MoveTo(gXmin + (int)(temp->point->x*lineSize),
					gYmax - (int)(temp->point->y*lineSize));
		pDC->LineTo(gXmin + (int)(temp->next->point->x*lineSize),
					gYmax - (int)(temp->next->point->y*lineSize));
		temp = m_flow_point->move_next();
	}
}

void change_start_point(void){
	if(init_state==false) return;
	if(sele_state==true) {
		curr_point++;
		if(curr_point==mPoint_num){
			curr_point=0;
			sele_state = false;
		}
	} else {
		sele_state = true;
		curr_point = 0;
	}
}

void enable_drawWaterflow(bool mEnable){
	if(init_state==false) return;
	draw_state = mEnable;
}
