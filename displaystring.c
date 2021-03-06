#include "includes.h"
#define TASK_STK_SIZE 512
UBYTE TaskStk[TASK_STK_SIZE];
UBYTE TaskData;
UWORD OldBP;
UWORD OldSS;
UWORD OldSP;
void interrupt(*OldTickISR)(void);
void far Task(void *data);
void DispChar(UBYTE x, UBYTE y, char c);

void main(void) {
	clrscr();
	OldBP = _BP;
	OldSS = _SS;
	OldSP = _SP;
	OldTickISR = getvect(0x08);
	setvect(0x81, OldTickISR);
	setvect(uCOS, (void interrupt(*)(void))OSCtxSw);
	OSInit();
	OSTaskCreate(Task, (void *)&TaskData, (void *)&TaskStk[TASK_STK_SIZE], 62);
	OSStart();
}

void far Task(void *data) {
	data = data;
	while(1) {
		DispChar(1, 3, 'A');
		DispChar(3, 3, 'B');
		DispChar(5, 3, 'C');
	}
}

void DispChar(UBYTE x, UBYTE y, char c) {
	UBYTE far *pscr;
	UWORD offset;
	offset = (UWORD)y*160 + (UWORD)x*2;
	pscr = MK_FP(0xB800, offset);
	OS_ENTER_CRITICAL;
	*pscr++ = c;
	*pscr = 0x07;
	OS_EXIT_CRITICAL;
}
