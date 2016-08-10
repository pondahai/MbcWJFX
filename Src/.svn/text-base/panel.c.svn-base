#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"

#include "vpl.h"


/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
extern BNP KeyboardObject;

extern UInt16 PanelPointerState;// = HANDBitmap; // panel : HAND , FINGER
extern struct hand HandPointer;
extern int prev_x,prev_y;
extern struct Point WireStart,WireStop;
//extern BNP LLP;
extern HOOKP SYSHOOK;	// 程式主串列的頭
extern  int penMoveAverage;
extern  Boolean CanvasState;//=true;
extern WNP WLLP;
extern  int RUN;//=false;
extern  int STOP;
extern Boolean RUNFOREVER;
extern  Boolean LAMP;
//extern  Boolean RunWho; //開關 0=執行線段 1=執行元件
//extern Boolean WaitFileLoad;
//extern Boolean WhoLoad; 

/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/
//extern HOOKP NEW(HOOKP sysHead);
extern HOOKP InsertHOOKNode(HOOKP sysHead);
extern InAreaInfType IsInArea(InAreaInfType ,BNP ,UInt16 now_x,UInt16 now_y,UInt16 ,UInt16 );
extern  IONODEP CheckOnNode(InAreaInfType InAreaInf,int now_x,int now_y,Boolean Draw);
extern CTRLNODEP CheckOnCTRLNode(InAreaInfType InAreaInf,int now_x,int now_y);
extern Boolean CheckBoundary(int x,int y,int XX,int YY,int X,int Y);
extern void CaculateNowXNowY(UInt16 *now_x,UInt16 *now_y,UInt16 screenX,UInt16 screenY);
extern Boolean DoRun(BNP BlockLLHeadP,WNP WireLLHeadP);

/***********************************************************************
 *
 *   Internal variables
 *
 ***********************************************************************/
int IncreseCtrlSpeed=0;

/***********************************************************************
*	Panel event process routine
*	
*	
*	
*	
*	
*	
************************************************************************/
/***********************************************************************
*	Panel event process routine
*	
*	
*	
*	
*	
*	
************************************************************************/

// 6-2 僅擦去數值
void ValueErase(BNP BL)
{
	//int i;
	
	char text[10];
	switch(BL->pap->BITMAPID)
	{
		case CTRLU8Bitmap:
			//MemMove(&i,(BL->bap->IONodeLLHead->P),2);
			StrPrintF(text,"%d",*((int *)BL->bap->IONodeLLHead->P));
			WinEraseChars(text,StrLen(text),BL->pap->XY.x+3,BL->pap->XY.y+3);
		break;
		case INDICATOR8Bitmap:
			//MemMove(&i,(BL->bap->IONodeLLHead->P),2);
			StrPrintF(text,"%d",*((int *)BL->bap->IONodeLLHead->P));
			WinEraseChars(text,StrLen(text),BL->pap->XY.x+3,BL->pap->XY.y+3);
		break;
		/*
		case LEDOFFBitmap:
		case LEDONBitmap:
			if(*(BL->bap->IONodeLLHead->P) == true)
				BL->pap->BITMAPID = LEDONBitmap;
			else
				BL->pap->BITMAPID = LEDOFFBitmap;
			DrawIcon(BL->pap->BITMAPID,BL->pap->XY.x,BL->pap->XY.y);
		*/
		break;
		default:
		break;
	}

}
// 6-2 僅顯示數值
void ValueDisplay(BNP BL)
{
	//int i;

	char text[10];
	switch(BL->pap->BITMAPID)
	{
		case CTRLU8Bitmap:
			//MemMove(&i,(BL->bap->IONodeLLHead->P),2);
			StrPrintF(text,"%d",*((int *)BL->bap->IONodeLLHead->P));
			WinDrawChars(text,StrLen(text),BL->pap->XY.x+3,BL->pap->XY.y+3);
		break;
		case INDICATOR8Bitmap:
			//MemMove(&i,(BL->bap->IONodeLLHead->P),2);
			StrPrintF(text,"%d",*((int *)BL->bap->IONodeLLHead->P));
			WinDrawChars(text,StrLen(text),BL->pap->XY.x+3,BL->pap->XY.y+3);
		break;
		case LEDOFFBitmap:
		case LEDONBitmap:
			if( *((int *)BL->bap->IONodeLLHead->P) == true)
				BL->pap->BITMAPID = LEDONBitmap;
			else
				BL->pap->BITMAPID = LEDOFFBitmap;
			DrawIcon(BL->pap->BITMAPID,BL->pap->XY.x,BL->pap->XY.y);
		break;
		default:
		break;
	}

}
// 6-2 執行RUN時的panel draw
static void DrawRUNPanel(BNP llp,Boolean show)
{
	//int i;
	//RectangleType Rtest;
	//char debugtext[20];
	//BNP llp;

	//if(Items[0].UID != NULL)
	//for(i=0;i<5;i++) // now have 5 items on diagram
	//llp = SYSHOOK->BlockLLHeadP;
	while(1)
	{
	 if(llp != NULL)
	 {
			switch(llp->bap->BITMAPID)
			{
		        case SWITCHCASEBitmap :
		        break;
		        case FORLOOPBitmap :
					if(show)
					{
						DrawRUNPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,true);
					}else{
						DrawRUNPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,false);
					}
		        break;
				case WHILELOOPBitmap :
					if(show)
					{
						DrawRUNPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,true);
					}else{
						DrawRUNPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,false);
					}
				break;
				default:
					if(llp->pap != NULL)
					{

					if(show)
					{
						/*
						StrPrintF(debugtext,"%d",llp->ID);
							WinDrawChars(debugtext,StrLen(debugtext),
				 			llp->pap->XY.x  ,
							llp->pap->XY.y - 11);
						*/
						//DrawIcon(llp->pap->BITMAPID,llp->pap->XY.x,llp->pap->XY.y);
						ValueDisplay(llp);
					}
					else
					{
						ValueErase(llp);
						/*
						StrPrintF(debugtext,"%d",llp->ID);
							WinEraseChars(debugtext,StrLen(debugtext),
				 			llp->pap->XY.x  ,
							llp->pap->XY.y - 11);
						Rtest.topLeft.x=llp->pap->XY.x;
						Rtest.topLeft.y=llp->pap->XY.y;
						Rtest.extent.x=llp->pap->SIZE.x;
						Rtest.extent.y=llp->pap->SIZE.y;
						WinEraseRectangle(&Rtest,0);
						*/
					}
					}
				break;
			}

		
	 }else{
	 	break;
	 }
	  llp = llp->NEXTNODE;
	}
	/*
	DrawIcon(PanelPointerState,142,0);
	//
	//
	if(LAMP)
		DrawIcon(LAMPLIGHTBitmap,100,0);
	else
		DrawIcon(LAMPBitmap,100,0);
		
	if(STOP==2)
		DrawIcon(STOPBitmap,80,0);
	else if(STOP==0)
		DrawIcon(STOPUNUSEBitmap,80,0);
	else if(STOP==1)
		DrawIcon(STOPWAITBitmap,80,0);
		
	if(RUN==2)
		DrawIcon(PAUSEBitmap,60,0);
	else if(RUN==1)
		DrawIcon(RUNBitmap,60,0);
	else if(RUN==0)
		DrawIcon(RUNWAITBitmap,60,0);
	*/	
}

// 5-17 panel 也要巢狀解析 不然迴圈內的 出入元件 出不來
static void DrawPanel(BNP llp,Boolean show)
{
	//int i;
	RectangleType Rtest;
	char debugtext[20];
	//BNP llp;

	//if(Items[0].UID != NULL)
	//for(i=0;i<5;i++) // now have 5 items on diagram
	//llp = SYSHOOK->BlockLLHeadP;
	while(1)
	{
	 if(llp != NULL)
	 {
			switch(llp->bap->BITMAPID)
			{
		        case SWITCHCASEBitmap :
		        break;
		        case FORLOOPBitmap :
					if(show)
					{
						DrawPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,true);
					}else{
						DrawPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,false);
					}
		        break;
				case WHILELOOPBitmap :
					if(show)
					{
						DrawPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,true);
					}else{
						DrawPanel(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,false);
					}
				break;
				default:
					if(llp->pap != NULL)
					{

					if(show)
					{
						StrPrintF(debugtext,"%d",llp->ID);
							WinDrawChars(debugtext,StrLen(debugtext),
				 			llp->pap->XY.x  ,
							llp->pap->XY.y - 11);

						DrawIcon(llp->pap->BITMAPID,llp->pap->XY.x,llp->pap->XY.y);
						ValueDisplay(llp);
					}
					else
					{
						StrPrintF(debugtext,"%d",llp->ID);
							WinEraseChars(debugtext,StrLen(debugtext),
				 			llp->pap->XY.x  ,
							llp->pap->XY.y - 11);

						Rtest.topLeft.x=llp->pap->XY.x;
						Rtest.topLeft.y=llp->pap->XY.y;
						Rtest.extent.x=llp->pap->SIZE.x;
						Rtest.extent.y=llp->pap->SIZE.y;
						WinEraseRectangle(&Rtest,0);
					}
					}
				break;
			}

		
	 }else{
	 	break;
	 }
	  llp = llp->NEXTNODE;
	}
	
	
	if(show)
	{
		if(KeyboardObject)
		{
			if((KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y+60)>160)
			{
				DrawDecimalIntKeyboard(KeyboardObject->pap->XY.x,KeyboardObject->pap->XY.y-60);
			}
			else
			{
				DrawDecimalIntKeyboard(KeyboardObject->pap->XY.x,KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y);
			}
		}
	}else
	{
		if(KeyboardObject)
		{
			if((KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y+60)>160)
			{
				EraseDecimalIntKeyboard(KeyboardObject->pap->XY.x,KeyboardObject->pap->XY.y-60);
			}
			else
			{
				EraseDecimalIntKeyboard(KeyboardObject->pap->XY.x,KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y);
			}
		}
	}

	DrawIcon(PanelPointerState,142,0);
	//
	//
	if(LAMP)
		DrawIcon(LAMPLIGHTBitmap,100,0);
	else
		DrawIcon(LAMPBitmap,100,0);
		
	if(STOP==2)
		DrawIcon(STOPBitmap,80,0);
	else if(STOP==0)
		DrawIcon(STOPUNUSEBitmap,80,0);
	else if(STOP==1)
		DrawIcon(STOPWAITBitmap,80,0);
		
	if(RUN==2)
		DrawIcon(PAUSEBitmap,60,0);
	else if(RUN==1)
		DrawIcon(RUNBitmap,60,0);
	else if(RUN==0)
		DrawIcon(RUNWAITBitmap,60,0);
		
	if(RUNFOREVER)
		DrawIcon(RUNRUNBitmap,40,0);
	else
		DrawIcon(RUNRUNWAITBitmap,40,0);
}
void ChangeControl(BNP BL,CTRLNODEP i)
{
	//int ivalue;
	
	if(i != NULL)
	switch(BL->pap->BITMAPID)
	{
		case CTRLU8Bitmap:
			//MemMove(&ivalue,BL->bap->IONodeLLHead->P,2);
			if(i->ID==CONTROLUP)
			{
				//ivalue++;
				//MemMove(BL->bap->IONodeLLHead->P,&ivalue,2);
				//*BL->bap->IONodeLLHead->P = (int)((int)(*BL->bap->IONodeLLHead->P) + (int)1);
				(*(int *) BL->bap->IONodeLLHead->P)++;
			}
			if(i->ID==CONTROLDOWN)
			{
				//ivalue--;
				//MemMove(BL->bap->IONodeLLHead->P,&ivalue,2);
				//*BL->bap->IONodeLLHead->P = (int)((int)(*BL->bap->IONodeLLHead->P) - (int)1);
				(*(int *) BL->bap->IONodeLLHead->P)--;
			}
		
		break;
		case SWOFFBitmap:
		
			(*((int *)BL->bap->IONodeLLHead->P)) = true;
			BL->pap->BITMAPID = SWONBitmap;
		
		break;
		case SWONBitmap:
		
			(*((int *)BL->bap->IONodeLLHead->P)) = false;
			BL->pap->BITMAPID = SWOFFBitmap;
		
		break;
		default:
		break;
	}
}
Boolean PenDownInPanelToolArea(EventPtr eventP)
{
	Boolean In=false;

	if(CheckBoundary(140,0,18,18,eventP->screenX,eventP->screenY))
	{
			In=true;
		FrmPopupForm(PanelToolsForm);
	}else if(CheckBoundary(120,0,16,16,eventP->screenX,eventP->screenY)){
			In=true;
	// 是否落在
		//FrmPopupForm(FunctionsForm);
	}else if(CheckBoundary(100,0,16,16,eventP->screenX,eventP->screenY)){
			In=true;
		if(LAMP)
		{
			LAMP=false;
		}else{
			LAMP=true;
		}
		DrawPanel(SYSHOOK->BlockLLHeadP,1);
	}else if(CheckBoundary(80,0,16,16,eventP->screenX,eventP->screenY)){
		if(STOP==1)
		{
			In=true;
			STOP=0;
			RUN=0;
			RUNFOREVER=false;
			DrawIcon(RUNWAITBitmap,60,0);
			DrawIcon(STOPUNUSEBitmap,80,0);
			DrawIcon(RUNRUNWAITBitmap,40,0);
		}
	}else if(CheckBoundary(60,0,16,16,eventP->screenX,eventP->screenY)){
		if(RUN==0)
		{
			In=true;
		// 作一次線清除的動作
	//DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,0,0,0,true);
			STOP=1;
			RUN=1;
			// 清除元件旗標
			CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
			// 線段 狀態重置
			ResetWireStatus(SYSHOOK->BlockLLHeadP->NEXTNODE,
			SYSHOOK->WireLLHeadP);

			//RunWho=false;
			if(LAMP)	// 如果燈打開
			{
				FrmGotoForm(BlockDiagramForm);
			}else
			{
				FrmGotoForm(FrontPanelForm);
			}
		}else if(RUN==1){
			RUN=2;
			DrawIcon(PAUSEBitmap,60,0);
		}else if(RUN==2){
			RUN=1;
			DrawIcon(RUNBitmap,60,0);
		}
		//DrawPanel(1);
	}else if(CheckBoundary(40,0,16,16,eventP->screenX,eventP->screenY)){
		In =true;
		if(RUNFOREVER)
		{
			RUNFOREVER=false;
			DrawIcon(RUNRUNWAITBitmap,40,0);
		}else{
			RUNFOREVER=true;
			STOP=1;
			RUN=1;
			// 清除元件旗標
			CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
			// 線段 狀態重置
			ResetWireStatus(SYSHOOK->BlockLLHeadP->NEXTNODE,
			SYSHOOK->WireLLHeadP);
			if(LAMP)	// 如果燈打開
			{
				FrmGotoForm(BlockDiagramForm);
			}else
			{
				FrmGotoForm(FrontPanelForm);
			}
		}
	}else if(CheckBoundary(0,0,33,15,eventP->screenX,eventP->screenY)){
		In =true;
	// 是否落在標題，若是則轉換 form
		FrmGotoForm(BlockDiagramForm);
	}
	// 6-2 如果筆在非作用區 則不要執行動作
	if(CheckBoundary(0,0,160,16,eventP->screenX,eventP->screenY))
	{
		In=true;
	}
	return In;
}
 void PanelpenDownProcess(EventPtr eventP)
{
	//Handle resH;
	//BitmapType *bitmap;
	UInt16 now_x,now_y;
	//Boolean InArea=false;
	//int keyboard_x,keyboard_y;
	CTRLNODEP i;
	InAreaInfType InAreaInf;
	InAreaInf.InArea=false;
	//BNP 
	
	CaculateNowXNowY(&now_x,&now_y,eventP->screenX,eventP->screenY);
/*
		if(eventP->screenX<10)now_x=10;else now_x = eventP->screenX;
		if(eventP->screenY<10+15)now_y=10+15;else now_y = eventP->screenY;
*/	
	InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
	
	// tool = hand
	if(PenDownInPanelToolArea(eventP)){
		if(KeyboardObject) // 6-8
		{
			DrawPanel(SYSHOOK->BlockLLHeadP,0); // clear diagram
			KeyboardObject=NULL;
			DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
		}
	}else{
	switch(PanelPointerState){
		case HANDBitmap:
			// 6-2 執行中不可編輯
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}

			if(InAreaInf.InArea)
			{
				// in boundary
				DrawIcon(CLENCHBitmap,now_x+offset_x,now_y+offset_y);
				
				HandPointer.HandState = CLENCHBitmap;
				HandPointer.CatchWhat = InAreaInf.llp; // point now pap->
				HandPointer.RelativePoint.x = now_x - InAreaInf.llp->pap->XY.x;
				HandPointer.RelativePoint.y = now_y - InAreaInf.llp->pap->XY.y;

			}
			else
			{
				DrawIcon(HANDBitmap,now_x+offset_x,now_y+offset_y);
				
				HandPointer.HandState = HANDBitmap;
			}
		break;
		case FINGERBitmap:
			if(InAreaInf.InArea)
			{
				// 6-2 在輸入時 不用平滑處理 加快速度
				penMoveAverage = 0;
				IncreseCtrlSpeed=5;
				// check on node?
				// LLP 隱含傳遞
				DrawIcon(PRESSFINGERBitmap,now_x+offset_x,now_y+offset_y);
				//if((i=CheckOnNode(InAreaInf,now_x,now_y,true)) != -1)
				//{
				DrawRUNPanel(SYSHOOK->BlockLLHeadP,false); // 6-2 just erase value

				i=CheckOnCTRLNode(InAreaInf,now_x,now_y);
					ChangeControl(InAreaInf.llp,i);

				DrawRUNPanel(SYSHOOK->BlockLLHeadP,true); // 6-2 just display value
				//}
			}
			else
			{
				DrawIcon(PRESSFINGERBitmap,now_x+offset_x,now_y+offset_y);
		
			}
		
		
		break;
		case PENCILBitmap:
			if(KeyboardObject)
			{
				//DrawRUNPanel(SYSHOOK->BlockLLHeadP,false); // 6-2 just erase value
				DrawPanel(SYSHOOK->BlockLLHeadP,0); // clear diagram
				//keyboard_x=KeyboardObject->pap->XY.x;
				//keyboard_y=KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y;
				if(
				((KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y+60)>160)?
	KeyboardPendownProcess(KeyboardObject->pap->XY.x,KeyboardObject->pap->XY.y-60,KeyboardObject,now_x,now_y):
	KeyboardPendownProcess(KeyboardObject->pap->XY.x,KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y,KeyboardObject,now_x,now_y)
				)
				{
					//DrawRUNPanel(SYSHOOK->BlockLLHeadP,true); // 6-2 just display value
					//DrawDecimalIntKeyboard(keyboard_x,keyboard_y);
					DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
				}else{
					//DrawRUNPanel(SYSHOOK->BlockLLHeadP,true); // 6-2 just display value
					//EraseDecimalIntKeyboard(keyboard_x,keyboard_y);

					KeyboardObject=NULL;
					DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
				}
			}else
			{
				//InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
				if(InAreaInf.InArea)
				{
					switch(InAreaInf.llp->pap->BITMAPID)
					{
						case CTRLU8Bitmap:
							KeyboardObject=InAreaInf.llp;
							//DrawDecimalIntKeyboard(KeyboardObject->pap->XY.x,KeyboardObject->pap->XY.y+KeyboardObject->pap->SIZE.y);
							DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
						break;
					}
				}
			}
		break;

		default:
		break;
	}// switch
	}// else if
		//
			prev_x = now_x;
			prev_y = now_y;
			//penMove Average = 0;
	
}
 void PanelpenMoveProcess(EventPtr eventP)
{

	//Handle resH;
	//BitmapType *bitmap;
	UInt16 now_x,now_y;
	//int i;
	CTRLNODEP i;
	//Boolean InArea=false;
	InAreaInfType InAreaInf;
	InAreaInf.InArea=false;

	CaculateNowXNowY(&now_x,&now_y,eventP->screenX,eventP->screenY);
/*	
		if(eventP->screenX<10)now_x=10;else now_x = eventP->screenX;
		if(eventP->screenY<10+15)now_y=10+15;else now_y = eventP->screenY;
*/		
		//DrawPanel(SYSHOOK->BlockLLHeadP,0); // clear diagram
		
		// tool = hand
	if(CheckBoundary(0,0,160,16,eventP->screenX,eventP->screenY))
	{}
	else{
	switch(PanelPointerState){
		case HANDBitmap:
			DrawPanel(SYSHOOK->BlockLLHeadP,0); // clear diagram
			// clear prev. pointer icon
			DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
			
			if(HandPointer.HandState == CLENCHBitmap)
			{
				// in boundary
				setNewObjXY(now_x-HandPointer.RelativePoint.x,now_y-HandPointer.RelativePoint.y);
				DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
				DrawIcon(CLENCHBitmap,now_x+offset_x,now_y+offset_y);
			}
			else if(HandPointer.HandState == HANDBitmap)
			{
				DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
				DrawIcon(HANDBitmap,now_x+offset_x,now_y+offset_y);
			}

		//
		break;
		case FINGERBitmap:
		
			//DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
			// clear prev. pointer icon
			DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
			
			
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0);
			// LLP is set
			//CheckOnNode(prev_x,prev_y,false);

			if(InAreaInf.InArea)
			{
				// check on node?
				DrawIcon(PRESSFINGERBitmap,now_x+offset_x,now_y+offset_y);
				CheckOnNode(InAreaInf,now_x,now_y,true);
				if(IncreseCtrlSpeed==0)
				{
					DrawRUNPanel(SYSHOOK->BlockLLHeadP,false); // 6-2 just erase value
				
					i=CheckOnCTRLNode(InAreaInf,now_x,now_y);
						ChangeControl(InAreaInf.llp,i);

					DrawRUNPanel(SYSHOOK->BlockLLHeadP,true); // 6-2 just display value

				}else{
					IncreseCtrlSpeed--;
				}

			}
			else
			{
			
				//DrawIcon(,,);
				DrawIcon(PRESSFINGERBitmap,now_x+offset_x,now_y+offset_y);
			}
		
		
		break;
		default:
		break;
		}
	}
			prev_x = now_x;
			prev_y = now_y;
	
}
 void PanelpenUpProcess(EventPtr eventP)
{
//	Handle resH;
//	BitmapType *bitmap;
    FormPtr frmP;

	//CaculateNowXNowY(&now_x,&now_y,eventP->screenX,eventP->screenY);

		// tool = hand
	if(CheckBoundary(0,0,160,16,eventP->screenX,eventP->screenY))
	{}
	else{
	switch(PanelPointerState){
		case HANDBitmap:
		
			//if(eventP->screenY > 15 && HandPointer.HandState != NULL)
			//{
			if(HandPointer.HandState != NULL)
			{
				//if((prev_x) < 10)prev_x=10;
				//if((prev_y) < 10+15)prev_y=10+15;
				// clear prev. icon image
				DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				
				frmP = FrmGetActiveForm(); //redraw form
				//MainFormInit( frmP);
				FrmDrawForm ( frmP);
				
				DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
				DrawMainRUN();
			}
			HandPointer.CatchWhat = NULL;
			HandPointer.HandState = NULL;
		break;
		case FINGERBitmap:
		
			//if(eventP->screenY > 15 )
			//{
		
				//if((prev_x) < 10)prev_x=10;
				//if((prev_y) < 10+15)prev_y=10+15;
				
				// clear prev. icon image
				
				// 6-2 在輸入時 不用平滑處理 加快速度
				penMoveAverage =_PenMovAver;

				DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				
				frmP = FrmGetActiveForm(); //redraw form
				//MainFormInit( frmP);
				FrmDrawForm ( frmP);
				
				DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
				DrawMainRUN();
			//}
		
		break;
		case PENCILBitmap:
		/*
			if(KeyboardObject)
			{
				DrawDecimalIntKeyboard(KeyboardObject->bap->XY.x,KeyboardObject->bap->XY.y+KeyboardObject->bap->SIZE.y);
			}
			else
			{
				EraseDecimalIntKeyboard(KeyboardObject->bap->XY.x,KeyboardObject->bap->XY.y+KeyboardObject->bap->SIZE.y);
			}
		*/
				DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
		break;
		default:
		break;
	}
	}
}
/***********************************************************************
*	FrontPanel DoCommand routine
*	
*	
*	
*	
*	
*	
************************************************************************/

 Boolean FrontPanelFormDoCommand(Word command)
{
	Boolean handled = false;
//	FormPtr frmP;

	switch (command)
		{
		case AboutMenu:
			MenuEraseStatus (0);
			//AbtShowAbout (appFileCreator);
			//frmP = FrmInitForm (AboutForm);
			//FrmDoDialog (frmP);					// Display the About Box.
			//FrmDeleteForm (frmP);
			FrmPopupForm (AboutForm);
			handled = true;
			break;
		case FrontPanelWorksBlockDiagram:
			MenuEraseStatus (0);
			FrmGotoForm(BlockDiagramForm);
			handled = true;
			break;
		case FrontPanelWorksFrontPanel:
			MenuEraseStatus (0);
			FrmGotoForm(FrontPanelForm);
			handled = true;
			break;
		case FrontPanelWorksUIs:
			MenuEraseStatus (0);
			FrmGotoForm(UIsForm);
			handled = true;
			break;
		case RunRun:
		/*
			MenuEraseStatus (0);
			RUN = 1;
			DrawMainRUN();
			handled = true;
		*/
			break;
		case RunStop:
		/*
			MenuEraseStatus (0);
			RUN = 0;
			DrawMainRUN();
			handled = true;
		*/
			break;
			/*
		case FileLoadTestData:
			MenuEraseStatus (0);
			DrawCanvas(0);
			// prepare the test data
			SetTestData();
			DrawCanvas(1);
			handled = true;
			break;
			*/
		case FileNEW:
			MenuEraseStatus (0);
			NEW();
			FrmGotoForm(FrontPanelForm);
			handled = true;
			break;
		case FileSAVE:
			MenuEraseStatus (0);
			//FrmGotoForm(FileNameInputForm);
		/*
			FrmPopupForm(FileNameInputForm);
		*/
			handled = true;
			break;
		case FileLOAD:
		
			MenuEraseStatus (0);
		/*
			//FrmGotoForm(FilesForm);
			//WhoLoad = menuLoad;
			FrmPopupForm(FilesForm);
		*/
			handled = true;
			break;
		}
	return handled;
}

 Boolean FrontPanelFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;
	//char str[10];
	//Handle resH;
	//BitmapType *bitmap;
	//RectangleType Rtest;
	//int i;
	
	switch (eventP->eType) 
		{
		case menuEvent:
			PanelPointerState=HANDBitmap;
			return FrontPanelFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			//MainFormInit( frmP);
			FrmDrawForm ( frmP);
			
			// Do draw diagram
			DrawPanel(SYSHOOK->BlockLLHeadP,1);
			//DrawCanvas(1);
			// draw main run button
			DrawMainRUN();

			HandPointer.CatchWhat = NULL;
			HandPointer.HandState = NULL;
			handled = true;
			break;
		case penDownEvent:
			//frmP = FrmGetActiveForm();
			//StrPrintF(str,"%d",FrmGetActiveFormID());
			//WinDrawChars(str,StrLen(str),eventP->screenX,eventP->screenY);
			
			//if(eventP->screenY > 15)
			//{
				// Do check diagram
				PanelpenDownProcess(eventP);
			//}
			
			handled = true;
			break;
		case penMoveEvent:
				
			//penMove Average++;
			if(penMoveAverage ==0)
			{
				PanelpenMoveProcess(eventP);
				penMoveAverage =_PenMovAver;
			}else{
			 	penMoveAverage --;
			}
			
			handled = true;
			break;
		case penUpEvent:
		
			PanelpenUpProcess(eventP);
			
			handled = true;
			break;
		case keyDownEvent:
			switch(KeyCurrentState())
			{
				case keyBitPageUp:
					if(PanelPointerState == HANDBitmap)
					{
						PanelPointerState = FINGERBitmap;
					}
					else if(PanelPointerState == FINGERBitmap)
					{
						PanelPointerState = HANDBitmap;

					}
					DrawPanel(SYSHOOK->BlockLLHeadP,true);
				break;
				case keyBitPageDown:
					if(CanvasState)
					{
						CanvasState = false;
						FrmGotoForm(FrontPanelForm);
						
					}
					else
					{
						CanvasState = true;
						FrmGotoForm(BlockDiagramForm);
						
					}
					handled = true;
				break;
			}
			
		break;
		case nilEvent:
		if(RUN==1)
		{
			DrawIcon(RUNBitmap,60,0);
			// 5-30
			//DrawPanel(SYSHOOK->BlockLLHeadP,true);
			DrawRUNPanel(SYSHOOK->BlockLLHeadP,false);
			
			if(DoRun(SYSHOOK->BlockLLHeadP->NEXTNODE,SYSHOOK->WireLLHeadP))
			{
				DrawRUNPanel(SYSHOOK->BlockLLHeadP,true);
			}else{
				if(RUNFOREVER)
				{
					DrawRUNPanel(SYSHOOK->BlockLLHeadP,true);
					// 清除元件旗標
					CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
					// 線段 狀態重置
					ResetWireStatus(SYSHOOK->BlockLLHeadP->NEXTNODE,
					SYSHOOK->WireLLHeadP);
	

				}else{
					RUN=0;
					STOP=0;
					DrawIcon(RUNWAITBitmap,60,0);
					DrawPanel(SYSHOOK->BlockLLHeadP,1); // redraw diagram
				}
			}
		}
		break;

		default:
			break;
		
		}
	
	return handled;
}

