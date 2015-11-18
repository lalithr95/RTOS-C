#include "includes.h"
#define TASK_STK_SIZE 512
UBYTE MTASK_STK[TASK_STK_SIZE];
UBYTE MTaskData;
UBYTE TaskStk[3][TASK_STK_SIZE];

char TaskData[3] = "ABC";
UWORD OldBP;
UWORD OldSS;
UWORD OldSP;

void interrupt(*OldTickISR)(void);
void far Task(void *data);
void far Task1(void *data);
void far Task2(void *data);
void DispChar(UBYTE x, UBYTE y, char c);
void DispStr(UBYTE x, UBYTE y, char *s);

void main(void) {
	clrscr();
	OldBP = _BP;
	OldSS = _SS;
	OldSP = _SP;
	OldTickISR = getvect(0x08);
	setvect(0x81, OldTickISR);
	setvect(uCOS, (void interrupt(*)(void))OSCtxSw);
	OSInit();
	OSTaskCreate(Task, (void *)&MTaskData, (void *)&MTASK_STK[TASK_STK_SIZE], 62);
	OSStart();
}

void far Task(void *data) {
	data = data;
	OS_ENTER_CRITICAL();
	setvect(0x08, (void interrupt(*)(void))OldTickISR);
	OS_EXIT_CRITICAL();
	DispStr(0, 22, "Creating 2 Task");
	OSTimeDly(10);
	OSTaskCreate(Task1, (void *)&TaskData[1], (void *)&TaskStk[1][TASK_STK_SIZE]);
	OSTaskCreate(Task2, (void *)&TaskData[2], (void *)&TaskStk[2][TASK_STK_SIZE]);
	while(1) {
		if (kbhit) {
			OS_EXIT_CRITICAL();
			setvect(0x08, OldTickISR);
			OS_EXIT_CRITICAL();
			clrscr();
			_BP = OldBP;
			_SS = OldSS;
			_SP = OldSP;
			exit(0);
		}
		OSTimeDly(10);
	}
}

void far Task1(void *data) {
	data = data;
	while(1) {
		DispStr(0, 21, "Creating second Task");
		DispChar(1, 3, 'V');
		OSTimeDly(3);
	}
}

void far Task2(void *data) {
	data = data;
	while(1) {
		DispStr(0, 21, "Creating first Task");
		DispChar(1, 3, '');
		OSTimeDly(10);
	}
}

void DispStr(UBYTE x, UBYTE y, char *s) {
	UWORD offset;
	UBYTE *pscr;
	offset  = (UWORD) y*160 + (UWORD)x*2;
	pscr = MK_FP(0xB800, offset);
	while(*s) {
		OS_ENTER_CRITICAL();
		*pscr++ = *s++;
		*pscr = 0x07;
		OS_EXIT_CRITICAL();
	}
}

void DispChar(UBYTE x, UBYTE y, char c) {
	UWORD offset;
	UBYTE far *pscr;
	offset = (UWORD)y*160 + (UWORD)x*2;
	pscr = MK_FP(0xB800, offset);
	OS_ENTER_CRITICAL();
	*pscr++ = c;
	*pscr = 0x07;
	OS_EXIT_CRITICAL();
}
