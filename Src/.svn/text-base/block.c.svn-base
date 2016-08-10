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
Boolean SHOW_ID=true;
Boolean SHOW_VALUE=true;
Boolean SHOW_NOTREADYBIT=false;
Boolean SHOW_IONODENUM=false;
Boolean SHOW_WIREDIRTY=false;

extern BNP KeyboardObject;
extern UInt16 BlockPointerState;// = HANDBitmap; // block : HAND , THREAD
extern struct hand HandPointer;
extern int prev_x,prev_y;
extern EndPoint WireStart,WireStop;
//extern BNP LLP;
extern HOOKP SYSHOOK;	// 程式主串列的頭
extern  int penMoveAverage;
extern  Boolean CanvasState;//=true;
extern WNP WLLP;
extern  int RUN;//=false;
extern  int STOP;
extern Boolean RUNFOREVER;
extern  Boolean LAMP;
extern Boolean CUSTOMLOAD;//extern Boolean WaitFileLoad;
//extern Boolean WhoLoad; 
struct{
	//Boolean Hit;
	BNP llp;
}DragStatus;

// 元件脫離原本所屬串列
BNP ESCout;
// 動畫
extern Boolean RunLock;
//extern Boolean RunWho;
//extern Boolean Animation;
extern Word TicksPerSecend;
/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/
extern BNP allocate_a_new_node();
extern IONODEP allocate_a_new_IOnode();
extern HOOKP AddWireLinkListNode(HOOKP hookp,EndPoint Start,EndPoint Stop);
extern BNP RemoveBlockLinkList(BNP ,BNP  blockptr);
extern IONODEP RemoveIONodeLinkList(IONODEP IONodeHead,IONODEP  ionode);
extern WNP RemoveWireLinkList(WNP WireLLHead,WNP  wireptr);
extern BNP InsertIntoBlockLLwithUID(BNP bLLP,BNP node);
extern HOOKP InsertHOOKNode(HOOKP sysHead);
extern IONODEP InsertIntoIONodesLL(IONODEP IONodeH,IONODEP node );

extern InAreaInfType IsInArea(InAreaInfType ,BNP ,UInt16 now_x,UInt16 now_y,UInt16,UInt16);
//extern BNP FindItemByID(BNP ,UInt16 ID);
//extern HOOKP NEW(HOOKP sysHead);
extern void setNewObjXY(UInt16 newx,UInt16 newy);
extern void DrawMainRUN();
extern UInt16 CountIONODEs(IONODEP IO);
extern void DrawIcon(UInt16 BitMapID,UInt16 x,UInt16 y);
extern CTRLNODEP CheckOnCTRLNode(InAreaInfType InAreaInf,int now_x,int now_y);
extern BNP ItemMoveToLastPosition(BNP BLLHead,BNP bnp);
extern IONODEP CheckOnNode(InAreaInfType InAreaInf,int now_x,int now_y,Boolean Draw);
extern void NestReDraw(BNP llp);
extern void NestItemMoveToLastPosition(BNP llp);
extern void CaculateNowXNowY(UInt16 *now_x,UInt16 *now_y,UInt16 screenX,UInt16 screenY);
extern Boolean DoRun(BNP BlockLLHeadP,WNP WireLLHeadP);
extern void SAVE(char *filename );

/***********************************************************************
*	Block event process routine
*	
*	
*	
*	
*	
*	
************************************************************************/

// **********************************************
// * 4-24 加入處理結構功能
// *
// *
// **********************************************
void CaculateTurnLine(struct Point *Start,struct Point *Turn1,
			struct Point *Turn2,struct Point *End)
{
	if((Start->x == End->x)||(Start->y == End->y))
	{
		Turn1->x=Start->x;
		Turn1->y=Start->y;
		Turn2->x=Start->x;
		Turn2->y=Start->y;
		return;
	}
	Turn1->x = (Start->x+End->x)/2;
	Turn2->x = Turn1->x;
	Turn1->y = Start->y;
	Turn2->y = End->y;
}
void DrawWires(WNP wllp,Boolean show,Boolean ClearBit)
{
	struct Point start,end;
	struct Point turn1,turn2;
	BNP startblock,endblock;
	IONODEP startnode,endnode;
	BNP bllp;
	struct Point StartWindow;
	struct Point EndWindow;
	char debugtxt[20];
	
	//int i;
	//ULong MYtimetick;
	
	// draw wire process
	//WNP wllp;
	//WLLP = SYSHOOK->WireLLHeadP;
		while(wllp != NULL)
		{
		
		// 跳過暗線段
  	  if((wllp->StartBlockP != SYSHOOK->BlockLLHeadP) && (wllp->EndBlockP != SYSHOOK->BlockLLHeadP))
		{
			/*
			if(wllp->wire.StartBlockID != NULL)
			{
				startblock = (BNP)wllp->wire.StartBlockP;
				//startblock = FindItemByID(SYSHOOK->BlockLLHeadP->NEXTNODE,WLLP->wire.StartBlockID);
			}
			if(wllp->wire.EndBlockID != NULL)
			{
				endblock = (BNP)wllp->wire.EndBlockP;
				//endblock = FindItemByID(SYSHOOK->BlockLLHeadP->NEXTNODE,WLLP->wire.EndBlockID);
			}
			*/
			// 5-15
			// 不在動畫時 而且 不在 run 則 清除變數
			if(ClearBit)
			{
				wllp->DIRTY=false;
				wllp->STARTRUN=false;
				wllp->RUNSEG=0;
			}
			
			startblock = wllp->StartBlockP;
			endblock = wllp->EndBlockP;
			startnode = wllp->StartNodeP;
			endnode = wllp->EndNodeP;
			
			StartWindow.x=0;
			StartWindow.y=0;
			EndWindow.x=0;
			EndWindow.y=0;
			// 為處理結構內元件接線
			// 計算絕對座標
			if(startblock->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
			{
				StartWindow.x=0;StartWindow.y=0;
				bllp = startblock->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 走一遍串列，加出絕對座標
					StartWindow.x += bllp->bap->XY.x;
					StartWindow.y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
			
			}
			if(endblock->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
			{
				EndWindow.x=0;EndWindow.y=0;
				bllp = endblock->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 走一遍串列，加出絕對座標
					EndWindow.x += bllp->bap->XY.x;
					EndWindow.y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
			
			}
			
		start.x = StartWindow.x + startblock->bap->XY.x + startnode->TL.x + (startnode->EXT.x / 2);
		start.y = StartWindow.y + startblock->bap->XY.y + startnode->TL.y + (startnode->EXT.y / 2);
		
		end.x = EndWindow.x + endblock->bap->XY.x + endnode->TL.x + (endnode->EXT.x / 2);
		end.y = EndWindow.y + endblock->bap->XY.y + endnode->TL.y + (endnode->EXT.y / 2);
		
		CaculateTurnLine(&start,&turn1,&turn2,&end);

		if(show)
		{
			WinDrawLine(start.x,start.y,turn1.x,turn1.y);
			WinDrawLine(turn1.x,turn1.y,turn2.x,turn2.y);
		if(SHOW_WIREDIRTY)
		{
			StrPrintF(debugtxt,"%c",(wllp->DIRTY)?'T':'F');
				WinDrawChars(debugtxt,
					StrLen(debugtxt),
			 			turn1.x+2 ,
						turn1.y);
		}
			WinDrawLine(turn2.x,turn2.y,end.x,end.y);

		if(SHOW_NOTREADYBIT)
		{
			// debug 秀出節點狀態
			StrPrintF(debugtxt,"%c",(startnode->NotReady)?'T':'F');
				WinDrawChars(debugtxt,
					StrLen(debugtxt),
			 			start.x ,
						start.y -16);
			// debug 秀出節點狀態
			StrPrintF(debugtxt,"%c",(endnode->NotReady)?'T':'F');
				WinDrawChars(debugtxt,
					StrLen(debugtxt),
			 			end.x ,
						end.y -16);
		}
		}
		else
		{
			WinEraseLine(start.x,start.y,turn1.x,turn1.y);
			WinEraseLine(turn1.x,turn1.y,turn2.x,turn2.y);
		if(SHOW_WIREDIRTY)
		{
			StrPrintF(debugtxt,"%c",(wllp->DIRTY)?'T':'F');
				WinEraseChars(debugtxt,
					StrLen(debugtxt),
			 			turn1.x+2 ,
						turn1.y);
		}

			WinEraseLine(turn2.x,turn2.y,end.x,end.y);

		if(SHOW_NOTREADYBIT)
		{
			// debug 秀出節點狀態
			StrPrintF(debugtxt,"%c",(startnode->NotReady)?'T':'F');
				WinEraseChars(debugtxt,
					StrLen(debugtxt),
			 			start.x ,
						start.y -16);
			// debug 秀出節點狀態
			StrPrintF(debugtxt,"%c",(endnode->NotReady)?'T':'F');
				WinEraseChars(debugtxt,
					StrLen(debugtxt),
			 			end.x ,
						end.y -16);
		}
		}

/*
		if(startx == endx || starty == endy)
		{
			if(show)
			{
				WinDrawLine(startx,starty,endx,endy);
			}
			else
				WinEraseLine(startx,starty,endx,endy);
		}
		else
		{
			//compare x1,x2
			if(startx > endx)
				turn2x = turn1x = endx + (startx - endx)/2;
			if(startx < endx)
				turn2x = turn1x = startx + (endx - startx)/2;
				
			turn1y = starty;
			turn2y = endy;

			if(show)
			{
				WinDrawLine(startx,starty,turn1x,turn1y);
				WinDrawLine(turn1x,turn1y,turn2x,turn2y);
				WinDrawLine(turn2x,turn2y,endx,endy);
			}
			else
			{
				WinEraseLine(startx,starty,turn1x,turn1y);
				WinEraseLine(turn1x,turn1y,turn2x,turn2y);
				WinEraseLine(turn2x,turn2y,endx,endy);
			}

		}
		*/
		}// if 暗線段
			wllp = wllp->NEXTNODE;
		}
}
static void DrawCUSTOMBLOCKLattice(BNP llp,int window_x,int window_y)
{
	int i;
	int i_nodeN=0,o_nodeN=0;
	struct Point tl,bl,br,tr;
	int Hy;
	IONODEP ionodep;
	RectangleType Rtest;

	Rtest.topLeft.x=llp->bap->XY.x + window_x;
	Rtest.topLeft.y=llp->bap->XY.y + window_y;
	Rtest.extent.x=llp->bap->SIZE.x;
	Rtest.extent.y=llp->bap->SIZE.y;
		
	WinEraseRectangle(&Rtest,0);


	tl.x = llp->bap->XY.x + window_x;
	tl.y = llp->bap->XY.y + window_y;
	bl.x = llp->bap->XY.x + window_x;
	bl.y = llp->bap->XY.y+llp->bap->SIZE.y-1 + window_y;
	br.x = llp->bap->XY.x + llp->bap->SIZE.x-1 + window_x;
	br.y = llp->bap->XY.y+llp->bap->SIZE.y-1 + window_y;
	tr.x = llp->bap->XY.x + llp->bap->SIZE.x-1 + window_x;
	tr.y = llp->bap->XY.y + window_y;
	//畫框框
	WinDrawLine(tl.x,tl.y,bl.x,bl.y);
	WinDrawLine(bl.x,bl.y,br.x,br.y);
	WinDrawLine(br.x,br.y,tr.x,tr.y);
	WinDrawLine(tr.x,tr.y,tl.x,tl.y);
	WinDrawLine(tl.x+(llp->bap->SIZE.x/2),tl.y,tl.x+(llp->bap->SIZE.x/2),bl.y);
	//畫格子 走一遍IOnode鏈結串列
	ionodep = llp->bap->IONodeLLHead;
	while(ionodep != NULL)
	{
		if(ionodep->TYPE == 1)
			i_nodeN++;
		else
			o_nodeN++;
			
		ionodep = ionodep->NEXT;
	}
	/*
	for(i=0;i<IONodeNUM;i++)
	{
		if(llp->bap->IONodes[i].IO == true)
			i_nodeN++;
		else
			o_nodeN++;
	}
	*/
	for(i=1;i<=(i_nodeN-1);i++)
	{
		Hy = (llp->bap->SIZE.y / i_nodeN)*i + llp->bap->XY.y + window_y;
		WinDrawLine(tl.x,Hy,tl.x+(llp->bap->SIZE.x/2),Hy);
	}
	for(i=1;i<=(o_nodeN-1);i++)
	{
		Hy = (llp->bap->SIZE.y / o_nodeN)*i + llp->bap->XY.y + window_y;
		WinDrawLine(tl.x+(llp->bap->SIZE.x/2),Hy,tr.x,Hy);
	}
}
Boolean DrawRunPointOnWire(WNP wnp,Boolean not_show_result)
{
	Boolean not_finish=true;
	Boolean No_Any_Data_Move=true;// 6-2
	
	RectangleType Rtest;
	//struct Point MYwindow;
	struct Point StartNode;
	struct Point Turn1;
	struct Point Turn2;
	struct Point EndNode;
	BNP 	TempBNP;
	IONODEP	TempIONODEP;
	//char text[32];
	char debugtxt[20];
	Boolean ItIsNotDarkLine;
	FontID fontid;
	
	BNP bllp;
	//ULong MYTimeTicks;
/*
	Rtest.topLeft.x=llp->bap->XY.x + window_x;
	Rtest.topLeft.y=llp->bap->XY.y + window_y;
	Rtest.extent.x=llp->bap->SIZE.x + window_x;
	Rtest.extent.y=llp->bap->SIZE.y + window_y;
		
	WinEraseRectangle(&Rtest,0);
*/

	while(wnp != NULL)
	{
		// 跳過暗線段
		ItIsNotDarkLine=true;
		
	if((wnp->StartBlockP->TYPE==HOOKBLOCK)&&(wnp->StartBlockP == SYSHOOK->BlockLLHeadP))
		ItIsNotDarkLine=false;
		
	if((wnp->EndBlockP->TYPE==HOOKBLOCK)&&(wnp->EndBlockP == SYSHOOK->BlockLLHeadP))
		ItIsNotDarkLine=false;
	

	// 決定動畫方向
	if(wnp->EndNodeP->TYPE==0)
	{
		TempBNP=wnp->EndBlockP;
		TempIONODEP=wnp->EndNodeP;
		wnp->EndBlockP=wnp->StartBlockP;
		wnp->EndNodeP=wnp->StartNodeP;
		wnp->StartBlockP=TempBNP;
		wnp->StartNodeP=TempIONODEP;
	}

	if(not_show_result)
	{
	
  	  if(ItIsNotDarkLine &&(wnp->DIRTY == false))
  	  {
		//if(wnp->DIRTY)// dirty 表示該線段已經執行過
		// 6-2 該線段準備執行 還沒到終點
		if( ( (wnp->StartNodeP->NotReady == false) && (wnp->EndNodeP->NotReady == true) 
		&& ( ((wnp->RUNPOINT.x == -1)&&(wnp->RUNPOINT.y == -1))?false:true) ) )
		{
	// 6-2
	No_Any_Data_Move=false;
	// 計算絕對座標
	StartNode.x=wnp->StartNodeP->TL.x +(wnp->StartNodeP->EXT.x/2)+ wnp->StartBlockP->bap->XY.x;
	StartNode.y=wnp->StartNodeP->TL.y +(wnp->StartNodeP->EXT.y/2)+ wnp->StartBlockP->bap->XY.y;
	if(wnp->StartBlockP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
	{
		bllp = wnp->StartBlockP->SelfBlockLLHead;
		while(bllp != SYSHOOK->BlockLLHeadP)
		{//4-26 走一遍串列，加出絕對座標
			StartNode.x += bllp->bap->XY.x;
			StartNode.y += bllp->bap->XY.y;
			bllp = bllp->SelfBlockLLHead;
		}
	}
	EndNode.x=wnp->EndNodeP->TL.x +(wnp->EndNodeP->EXT.x/2)+ wnp->EndBlockP->bap->XY.x;
	EndNode.y=wnp->EndNodeP->TL.y +(wnp->EndNodeP->EXT.y/2)+ wnp->EndBlockP->bap->XY.y;
	if(wnp->EndBlockP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
	{
		bllp = wnp->EndBlockP->SelfBlockLLHead;
		while(bllp != SYSHOOK->BlockLLHeadP)
		{//4-26 走一遍串列，加出絕對座標
			EndNode.x += bllp->bap->XY.x;
			EndNode.y += bllp->bap->XY.y;
			bllp = bllp->SelfBlockLLHead;
		}
	}


			if(wnp->STARTRUN == false)
			{
				wnp->STARTRUN = true;
				wnp->RUNPOINT.x=StartNode.x;
				wnp->RUNPOINT.y=StartNode.y;
			}


				Rtest.topLeft.x=wnp->RUNPOINT.x-2;
				Rtest.topLeft.y=wnp->RUNPOINT.y-2;
				Rtest.extent.x=5;
				Rtest.extent.y=5;
		
				WinEraseRectangle(&Rtest,3);

				//for(;wnp->RUNPOINT.x != EndNode.x;wnp->RUNPOINT.x++);
				//for(;wnp->RUNPOINT.y != EndNode.y;wnp->RUNPOINT.y++);
				// 輸入兩點 取得四點
				CaculateTurnLine(&StartNode,&Turn1,&Turn2,&EndNode);
				
				switch(wnp->RUNSEG)
				{
					case 0:
						if((wnp->RUNPOINT.x==Turn1.x)&&(wnp->RUNPOINT.y==Turn1.y))
						{
							wnp->RUNSEG=1;
						}else{
							if(Turn1.x>wnp->RUNPOINT.x)
								wnp->RUNPOINT.x++;
							else
								wnp->RUNPOINT.x--;
						}
					break;
					case 1:
						if((wnp->RUNPOINT.x==Turn2.x)&&(wnp->RUNPOINT.y==Turn2.y))
						{
							wnp->RUNSEG=2;
						}else{
							if(Turn2.y>wnp->RUNPOINT.y)
								wnp->RUNPOINT.y++;
							else
								wnp->RUNPOINT.y--;
						}
					break;
					case 2:
						if((wnp->RUNPOINT.x==EndNode.x)&&(wnp->RUNPOINT.y==EndNode.y))
						{
							wnp->RUNSEG=3;
						}else{
							if(EndNode.x>wnp->RUNPOINT.x)
								wnp->RUNPOINT.x++;
							else
								wnp->RUNPOINT.x--;
						}
					break;
				}
				/*
				if(wnp->RUNPOINT.x != EndNode.x)
				{
					if(EndNode.x>wnp->RUNPOINT.x)
						wnp->RUNPOINT.x++;
					else
						wnp->RUNPOINT.x--;
				}
				if(wnp->RUNPOINT.y != EndNode.y)
				{
					if(EndNode.y>wnp->RUNPOINT.y)
						wnp->RUNPOINT.y++;
					else
						wnp->RUNPOINT.y--;
				}
				*/
				//wnp->RUNPOINT.x=
				//wnp->RUNPOINT.y=
				
				Rtest.topLeft.x=wnp->RUNPOINT.x-2;
				Rtest.topLeft.y=wnp->RUNPOINT.y-2;
				Rtest.extent.x=5;
				Rtest.extent.y=5;
		
				WinDrawRectangle(&Rtest,3);
				
				// 到終點
				if((wnp->RUNPOINT.x == EndNode.x)&&
				(wnp->RUNPOINT.y == EndNode.y))
				{
					wnp->RUNPOINT.x = -1;
					wnp->RUNPOINT.y = -1;
					wnp->STARTRUN = false;
					//wnp->DIRTY = false;
					not_finish=false;
				}else{
					//not_finish=true; // 還有 未完成的線 
				}
				

				// 起點
		if(SHOW_VALUE)
		{
			//6-7 設定字型實驗
//stdFont = 0x00, // Small font used for the user's writing.  Shows a good amount
//boldFont, 	  // Small font.  Bold for easier reading.  Used often for ui.
//largeFont,	  // Larger font for easier reading.  Shows a lot less.
//symbolFont,	  // Various ui images like check boxes and arrows
//symbol11Font,   // Larger various ui images
//symbol7Font,	  // Smaller various ui images
//ledFont,	  // Calculator specific font
//largeBoldFont,  // A thicker version of the large font.  More readable.
			
			fontid=FntGetFont();
			FntSetFont(boldFont);
			StrPrintF(debugtxt,"%d",GetNodeData(wnp->StartNodeP));//*((int *)wnp->StartNodeP->P));
			WinDrawChars(debugtxt,
				StrLen(debugtxt),
 			StartNode.x,
			StartNode.y-8);
			FntSetFont(fontid);
		}
	/*		
			// debug 秀出節點狀態
			StrPrintF(debugtxt,"%d",wnp->StartNodeP->NotReady);
				WinDrawChars(debugtxt,
					StrLen(debugtxt),
			 			StartNode.x ,
						StartNode.y -16);
	*/					


				//MYTimeTicks=TimGetTicks();
				//while((TimGetTicks()-MYTimeTicks)<(TicksPerSecend/100)){}
			
			

			}// if 線段未完成
		}// if not dark line
	} // not show result
	else
	{	
				// 5-30 加入顯示數值
				//if((wnp->RUNPOINT.x == StartNode.x)&&
				//(wnp->RUNPOINT.y == StartNode.y))
				//{
				//}
				/*
	// 計算絕對座標
	StartNode.x=wnp->StartNodeP->TL.x +(wnp->StartNodeP->EXT.x/2)+ wnp->StartBlockP->bap->XY.x;
	StartNode.y=wnp->StartNodeP->TL.y +(wnp->StartNodeP->EXT.y/2)+ wnp->StartBlockP->bap->XY.y;
	if(wnp->StartBlockP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
	{
		bllp = wnp->StartBlockP->SelfBlockLLHead;
		while(bllp != SYSHOOK->BlockLLHeadP)
		{//4-26 走一遍串列，加出絕對座標
			StartNode.x += bllp->bap->XY.x;
			StartNode.y += bllp->bap->XY.y;
			bllp = bllp->SelfBlockLLHead;
		}
	}
	EndNode.x=wnp->EndNodeP->TL.x +(wnp->EndNodeP->EXT.x/2)+ wnp->EndBlockP->bap->XY.x;
	EndNode.y=wnp->EndNodeP->TL.y +(wnp->EndNodeP->EXT.y/2)+ wnp->EndBlockP->bap->XY.y;
	if(wnp->EndBlockP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
	{
		bllp = wnp->EndBlockP->SelfBlockLLHead;
		while(bllp != SYSHOOK->BlockLLHeadP)
		{//4-26 走一遍串列，加出絕對座標
			EndNode.x += bllp->bap->XY.x;
			EndNode.y += bllp->bap->XY.y;
			bllp = bllp->SelfBlockLLHead;
		}
	}
	
	
	*/
				// 到終點
				// 6-2 修正
	if(wnp->DIRTY)// dirty 表示該線段已經執行過
	{
		
		//if((wnp->RUNPOINT.x == -1)&&(wnp->RUNPOINT.y == -1))
		//{
			//wnp->DIRTY = false;
		if(SHOW_VALUE)
		{
			EndNode.x=wnp->EndNodeP->TL.x +(wnp->EndNodeP->EXT.x/2)+ wnp->EndBlockP->bap->XY.x;
			EndNode.y=wnp->EndNodeP->TL.y +(wnp->EndNodeP->EXT.y/2)+ wnp->EndBlockP->bap->XY.y;
			if(wnp->EndBlockP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
			{
				bllp = wnp->EndBlockP->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 走一遍串列，加出絕對座標
					EndNode.x += bllp->bap->XY.x;
					EndNode.y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
			}
			
			//6-7 設定字型實驗
			fontid=FntGetFont();
			FntSetFont(boldFont);
			StrPrintF(debugtxt,"%d",GetNodeData(wnp->EndNodeP));//*((int *)wnp->EndNodeP->P));
			WinDrawChars(debugtxt,
				StrLen(debugtxt),
 			EndNode.x,
			EndNode.y-8);
			FntSetFont(fontid);
		}
		
			/*
			// debug 秀出節點狀態
			StrPrintF(debugtxt,"%d",wnp->EndNodeP->NotReady);
				WinDrawChars(debugtxt,
					StrLen(debugtxt),
			 			EndNode.x ,
						EndNode.y -16);
			*/
		//}
	}
	
	}// else if
	
		wnp = wnp->NEXTNODE;
	}// while
	
	if(No_Any_Data_Move)
		return false;
	else
		return not_finish;
}
//
// 要改為可遞迴形式
// 如遇到『結構』node，則遞迴呼叫本函式畫出。/
// 5-12 結構加入ReDraw 變數 ，避免機器上秀圖的閃爍
// 也就是說該畫再畫 因為機器實在太慢
// 5-12 新增引數 ReDrawAll 強制重畫
void DrawDiagram(BNP bllpH,WNP wllpH,Boolean Animation,Boolean show,int window_x,int window_y,Boolean ReDrawAll)
{
	//Handle resH;
	//BitmapType *bitmap;
	//int i;
	RectangleType Rtest;
	//BNP llp;
	struct Point s1,e1,s2,e2;
	char *teststr;
	int teststrlen;
	IONODEP ionode;
	WNP wirep;
	HOOKP thook;
	Boolean InSideHaveNoWire,OutSideHaveNoWire;
	char debugtext[20];
	
	DrawWires(wllpH,show,((RUN == 0)?true:false));
	
	while(bllpH != NULL)
	{

		if(show)
		{//顯示
		   if(bllpH->bap->ReDraw || ReDrawAll)
		   {
			switch(bllpH->TYPE)
			{
				case HOOKBLOCK :
						DrawCUSTOMBLOCKLattice(bllpH,window_x,window_y);
						
						WinDrawChars(bllpH->filename,StrLen(bllpH->filename),
								bllpH->bap->XY.x + window_x,bllpH->bap->XY.y+33 + window_y);

				break;
				case LOOPBLOCK :
						// 清除底色
				Rtest.topLeft.x=bllpH->bap->XY.x + window_x;
				Rtest.topLeft.y=bllpH->bap->XY.y + window_y;
				Rtest.extent.x=bllpH->bap->SIZE.x;
				Rtest.extent.y=bllpH->bap->SIZE.y;
				WinEraseRectangle(&Rtest,0);
						
						DrawIcon(LOOP2Bitmap,bllpH->bap->XY.x + window_x,bllpH->bap->XY.y + window_y);
						DrawIcon(LOOP3Bitmap,bllpH->bap->XY.x + bllpH->bap->SIZE.x-5 + window_x,bllpH->bap->XY.y + window_y);
						DrawIcon(LOOP4Bitmap,bllpH->bap->XY.x + bllpH->bap->SIZE.x-5 + window_x,bllpH->bap->XY.y + bllpH->bap->SIZE.y-5 + window_y);
						DrawIcon(LOOP5Bitmap,bllpH->bap->XY.x + window_x,bllpH->bap->XY.y + bllpH->bap->SIZE.y-5 + window_y);
						s1.x=bllpH->bap->XY.x+7 + window_x;
						s1.y=bllpH->bap->XY.y + window_y;
						e1.x=bllpH->bap->XY.x+bllpH->bap->SIZE.x-6 + window_x;
						e1.y=bllpH->bap->XY.y + window_y;
						WinDrawLine(s1.x,s1.y,e1.x,e1.y);
						WinDrawLine(s1.x,s1.y+2,e1.x,e1.y+2);
						WinDrawLine(s1.x,s1.y+bllpH->bap->SIZE.y-1,e1.x,e1.y+bllpH->bap->SIZE.y-1);
						WinDrawLine(s1.x,s1.y+bllpH->bap->SIZE.y-3,e1.x,e1.y+bllpH->bap->SIZE.y-3);
						s2.x=bllpH->bap->XY.x+2 + window_x;
						s2.y=bllpH->bap->XY.y+5 + window_y;
						e2.x=bllpH->bap->XY.x+2 + window_x;
						e2.y=bllpH->bap->XY.y + bllpH->bap->SIZE.y - 6 + window_y;
						WinDrawLine(s2.x,s2.y,e2.x,e2.y);
						WinDrawLine(s2.x+2,s2.y,e2.x+2,e2.y);
						WinDrawLine(s2.x+bllpH->bap->SIZE.x-5,s2.y,e2.x+bllpH->bap->SIZE.x-5,e2.y);
						WinDrawLine(s2.x+bllpH->bap->SIZE.x-3,s2.y,e2.x+bllpH->bap->SIZE.x-3,e2.y);
						
					DrawIcon(LOOP1Bitmap,bllpH->bap->XY.x + window_x,bllpH->bap->XY.y + (bllpH->bap->SIZE.y/2)-3 + window_y);
					//WHILELOOPLOOP
					if(bllpH->bap->BITMAPID==WHILELOOPBitmap)
					{// 改變預設元件的位置 相對位置
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.x=bllpH->bap->SIZE.x-15;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.y=bllpH->bap->SIZE.y-18 ;
//	DrawIcon(WHILELOOPLOOPBitmap,bllpH->bap->XY.x +bllpH->bap->SIZE.x+ window_x-10,
//	bllpH->bap->XY.y + bllpH->bap->SIZE.y-18 + window_y);

					}
					if(bllpH->bap->BITMAPID==FORLOOPBitmap)
					{// 5-30 加入 for 迴圈的預設元件處理
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.x=8 ;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.y=13 ;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->XY.x=8 ;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->XY.y=bllpH->bap->SIZE.y-17 ;
					}
					// 5-8 處理空連接的IO節點
					ionode = 	bllpH->bap->IONodeLLHead;
					InSideHaveNoWire = true;
					OutSideHaveNoWire = true;
					while(ionode != NULL)
					{
					    // 找出所屬串列的虎克
					    switch(bllpH->SelfBlockLLHead->TYPE)
					    {
							case HOOKBLOCK :
								thook = (HOOKP)bllpH->SelfBlockLLHead->hookP;
							break;
							case LOOPBLOCK :
								thook = (HOOKP)bllpH->SelfBlockLLHead->LoopBlockHOOKP;
							break;
							case CASEBLOCK :
								thook = (HOOKP)bllpH->SelfBlockLLHead->CurrentHOOKP;
							break;
					    }
					    wirep = thook->WireLLHeadP;
						// 外線段串列
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								OutSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								OutSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// 內 注意 虎克對象
						wirep = ((HOOKP)bllpH->LoopBlockHOOKP)->WireLLHeadP;
						// 內線段串列
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								InSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								InSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// 若該 IO 節點沒有接線則移除它
						if((InSideHaveNoWire == true) && (OutSideHaveNoWire == true))
						{// 移除後 線段變數重設
							bllpH->bap->IONodeLLHead = RemoveIONodeLinkList(
									bllpH->bap->IONodeLLHead,ionode);
							ionode = bllpH->bap->IONodeLLHead;
						}else{
							ionode = ionode->NEXT;
						}
					}
					
					// 畫出IO 節點
					ionode = 	bllpH->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						Rtest.topLeft.x=bllpH->bap->XY.x + window_x + ionode->TL.x;
						Rtest.topLeft.y=bllpH->bap->XY.y + window_y + ionode->TL.y;
						Rtest.extent.x=ionode->EXT.x;
						Rtest.extent.y=ionode->EXT.y;
						WinDrawRectangle(&Rtest,0);
						
						ionode = ionode->NEXT;
					}
					//接著處理內部元件(遞迴) // 避免空串列
							DrawDiagram(((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP,
									((HOOKP)bllpH->LoopBlockHOOKP)->WireLLHeadP,
									false,
									true,
									bllpH->bap->XY.x + window_x,
									bllpH->bap->XY.y + window_y,ReDrawAll);
						
				break;
		        case CASEBLOCK :
						// 清除底色
				Rtest.topLeft.x=bllpH->bap->XY.x + window_x;
				Rtest.topLeft.y=bllpH->bap->XY.y + window_y;
				Rtest.extent.x=bllpH->bap->SIZE.x;
				Rtest.extent.y=bllpH->bap->SIZE.y;
				WinEraseRectangle(&Rtest,0);

						DrawIcon(SC1Bitmap,
							bllpH->bap->XY.x + window_x + bllpH->bap->SIZE.x - 6,
							bllpH->bap->XY.y + window_y + 4);
						DrawIcon(SC2Bitmap,
							bllpH->bap->XY.x + window_x + 2,
							bllpH->bap->XY.y + window_y + 4);
						WinDrawLine(
							bllpH->bap->XY.x + window_x,
							bllpH->bap->XY.y + window_y + 13,
							bllpH->bap->XY.x + window_x + bllpH->bap->SIZE.x-1,
							bllpH->bap->XY.y + window_y + 13);
							
			        	WinDrawLine(bllpH->bap->XY.x + window_x,bllpH->bap->XY.y + window_y,bllpH->bap->XY.x + bllpH->bap->SIZE.x-1 + window_x,bllpH->bap->XY.y + window_y);
			        	WinDrawLine(bllpH->bap->XY.x + window_x,bllpH->bap->XY.y+1 + window_y,bllpH->bap->XY.x + bllpH->bap->SIZE.x-1 + window_x,bllpH->bap->XY.y+1 + window_y);
			        	WinDrawLine(bllpH->bap->XY.x + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-2 + window_y,bllpH->bap->XY.x + bllpH->bap->SIZE.x-1 + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-2 + window_y);
			        	WinDrawLine(bllpH->bap->XY.x + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-1 + window_y,bllpH->bap->XY.x + bllpH->bap->SIZE.x-1 + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-1 + window_y);
			        	WinDrawLine(bllpH->bap->XY.x + window_x,bllpH->bap->XY.y + window_y,bllpH->bap->XY.x + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-1 + window_y);
			        	WinDrawLine(bllpH->bap->XY.x+1 + window_x,bllpH->bap->XY.y + window_y,bllpH->bap->XY.x+1 + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-1 + window_y);
			        	WinDrawLine(bllpH->bap->XY.x + bllpH->bap->SIZE.x-1 + window_x,bllpH->bap->XY.y + window_y,bllpH->bap->XY.x + bllpH->bap->SIZE.x-1 + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-1 + window_y);
			        	WinDrawLine(bllpH->bap->XY.x + bllpH->bap->SIZE.x-2 + window_x,bllpH->bap->XY.y + window_y,bllpH->bap->XY.x + bllpH->bap->SIZE.x-2 + window_x,bllpH->bap->XY.y+bllpH->bap->SIZE.y-1 + window_y);
						
						teststr = ((HOOKP)bllpH->CurrentHOOKP)->name;
						teststrlen = StrLen(teststr);
						WinDrawChars(teststr,
							teststrlen,
								bllpH->bap->XY.x + window_x + (bllpH->bap->SIZE.x/2) - 
								(teststrlen*5)/2,
								bllpH->bap->XY.y+2 + window_y);
						

					// 5-8 處理空連接的IO節點
					ionode = 	bllpH->bap->IONodeLLHead;
					InSideHaveNoWire = true;
					OutSideHaveNoWire = true;
					while(ionode != NULL)
					{
					    // 找出所屬串列的虎克
					    switch(bllpH->SelfBlockLLHead->TYPE)
					    {
							case HOOKBLOCK :
								thook = (HOOKP)bllpH->SelfBlockLLHead->hookP;
							break;
							case LOOPBLOCK :
								thook = (HOOKP)bllpH->SelfBlockLLHead->LoopBlockHOOKP;
							break;
							case CASEBLOCK :
								thook = (HOOKP)bllpH->SelfBlockLLHead->CurrentHOOKP;
							break;
					    }
					    wirep = thook->WireLLHeadP;
						// 外線段串列
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								OutSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								OutSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// 內 注意 虎克對象
						wirep = ((HOOKP)bllpH->CurrentHOOKP)->WireLLHeadP;
						// 內線段串列
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								InSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								InSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// 若該 IO 節點沒有接線則移除它
						if((InSideHaveNoWire == true) && (OutSideHaveNoWire == true))
						{// 移除後 線段變數重設
							bllpH->bap->IONodeLLHead = RemoveIONodeLinkList(
									bllpH->bap->IONodeLLHead,ionode);
							ionode = bllpH->bap->IONodeLLHead;
						}else{
							ionode = ionode->NEXT;
						}
					}
					
					
						// 畫出IO 節點
					ionode = 	bllpH->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						Rtest.topLeft.x=bllpH->bap->XY.x + window_x + ionode->TL.x;
						Rtest.topLeft.y=bllpH->bap->XY.y + window_y + ionode->TL.y;
						Rtest.extent.x=ionode->EXT.x;
						Rtest.extent.y=ionode->EXT.y;
						WinDrawRectangle(&Rtest,0);
						ionode = ionode->NEXT;
					}
						//接著處理內部元件(遞迴)
							DrawDiagram(((HOOKP)bllpH->CurrentHOOKP)->BlockLLHeadP,
									((HOOKP)bllpH->CurrentHOOKP)->WireLLHeadP,
									false,
									true,
									bllpH->bap->XY.x + window_x,
									bllpH->bap->XY.y + window_y,ReDrawAll);
						
				break;					
				default :
						DrawIcon(bllpH->bap->BITMAPID,bllpH->bap->XY.x + window_x,bllpH->bap->XY.y + window_y);
				break;
					
			}// switch
				/*
				DrawIcon(llp->bap->BITMAPID,llp->bap->XY.x,llp->bap->XY.y);
				
				if(llp->bap->BITMAPID == CUSTOMBLOCKBitmap)
				{
					DrawCUSTOMBLOCKLattice(llp);
					WinDrawChars(llp->FileName,StrLen(llp->FileName),
						llp->bap->XY.x,llp->bap->XY.y+33
					);
				}
				*/
				// 畫外框
				if(bllpH->bap->DrawBorder)
				{
	        		WinDrawLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
        		    bllpH->bap->XY.y-1 + window_y);
	        		WinDrawLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y,
        		    bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
        		    bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y);

	        		WinDrawLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    bllpH->bap->XY.x-1 + window_x,
        		    bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y);
	        		WinDrawLine(bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
        		    bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y);
        		    
        		    bllpH->bap->DrawBorder=false;
				}else{
// 清除外框線	
		WinEraseLine(bllpH->bap->XY.x-1 + window_x,
            bllpH->bap->XY.y-1 + window_y,
  		    	bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
	    			bllpH->bap->XY.y-1 + window_y);
		WinEraseLine(bllpH->bap->XY.x-1 + window_x,
            bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y,
	    		bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
        			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y);

		WinEraseLine(bllpH->bap->XY.x-1 + window_x,
            bllpH->bap->XY.y-1 + window_y,
  		    	bllpH->bap->XY.x-1 + window_x,
	    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y);
		WinEraseLine(bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
            bllpH->bap->XY.y-1 + window_y,
  		    	bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + 1 + window_x,
	    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + 1 + window_y);
				}
				
			// DEBUG 秀出self 以及 background blockllhead
			if(SHOW_ID)
			{
			StrPrintF(debugtext,"%d",bllpH->ID);
			WinDrawChars(debugtext,
				StrLen(debugtext),
 			bllpH->bap->XY.x + window_x ,
			bllpH->bap->XY.y + window_y -11);
			}
			if(SHOW_IONODENUM)
			{
			StrPrintF(debugtext,"%d",bllpH->bap->INodeNUM);
			WinDrawChars(debugtext,
				StrLen(debugtext),
 			bllpH->bap->XY.x + window_x +10,
			bllpH->bap->XY.y + window_y -11);
			StrPrintF(debugtext,"%d",bllpH->bap->ONodeNUM);
			WinDrawChars(debugtext,
				StrLen(debugtext),
 			bllpH->bap->XY.x + window_x +20,
			bllpH->bap->XY.y + window_y -11);
			}
			
			
			bllpH->bap->ReDraw=false;
		   }// ReDraw
		   /*
		   if(KeyboardObject)
		   {
				DrawDecimalIntKeyboard(KeyboardObject->bap->XY.x,KeyboardObject->bap->XY.y+KeyboardObject->bap->SIZE.y);

		   }
		   */
		}
		else
		{//清除
			
			if(bllpH->bap->Clean || ReDrawAll)
			{
				// 清除主要外觀
				Rtest.topLeft.x=bllpH->bap->XY.x + window_x;
				Rtest.topLeft.y=bllpH->bap->XY.y + window_y;
				Rtest.extent.x=bllpH->bap->SIZE.x;
				Rtest.extent.y=bllpH->bap->SIZE.y;
		
				WinEraseRectangle(&Rtest,0);
			
			if(SHOW_ID)
			{
			StrPrintF(debugtext,"%d",bllpH->ID);
			WinEraseChars(debugtext,
				StrLen(debugtext),
 			bllpH->bap->XY.x + window_x ,
			bllpH->bap->XY.y + window_y -11);
			}	
			if(SHOW_IONODENUM)
			{
			StrPrintF(debugtext,"%d",bllpH->bap->INodeNUM);
			WinEraseChars(debugtext,
				StrLen(debugtext),
 			bllpH->bap->XY.x + window_x +10,
			bllpH->bap->XY.y + window_y -11);
			StrPrintF(debugtext,"%d",bllpH->bap->ONodeNUM);
			WinEraseChars(debugtext,
				StrLen(debugtext),
 			bllpH->bap->XY.x + window_x +20,
			bllpH->bap->XY.y + window_y -11);
			}

				switch(bllpH->TYPE)
				{
    			    case LOOPBLOCK :
						// 清除外框線
						/*
	        			WinEraseLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    	bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
        		    			bllpH->bap->XY.y-1 + window_y);
	        			WinEraseLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y,
        		    		bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
        		    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y);

	        			WinEraseLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    	bllpH->bap->XY.x-1 + window_x,
        		    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y);
	        			WinEraseLine(bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    	bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
        		    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y);
       		    		*/
        		    	bllpH->bap->DrawBorder = false;

							DrawDiagram(((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP,
									((HOOKP)bllpH->LoopBlockHOOKP)->WireLLHeadP,
									false,
									false,
									bllpH->bap->XY.x + window_x,
									bllpH->bap->XY.y + window_y,ReDrawAll);
        		    	
    			    break;
			        case CASEBLOCK :
						// 清除外框線
						/*
	        			WinEraseLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    	bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
        		    			bllpH->bap->XY.y-1 + window_y);
	        			WinEraseLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y,
        		    		bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
        		    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y);

	        			WinEraseLine(bllpH->bap->XY.x-1 + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    	bllpH->bap->XY.x-1 + window_x,
        		    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y);
	        			WinEraseLine(bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
       		             bllpH->bap->XY.y-1 + window_y,
	        		    	bllpH->bap->XY.x-1+bllpH->bap->SIZE.x + window_x,
        		    			bllpH->bap->XY.y-1+bllpH->bap->SIZE.y + window_y);
       		    		*/
        		    	bllpH->bap->DrawBorder = false;
        		    	// 然後遞迴
							DrawDiagram(((HOOKP)bllpH->CurrentHOOKP)->BlockLLHeadP,
									((HOOKP)bllpH->CurrentHOOKP)->WireLLHeadP,
									false,
									false,
									bllpH->bap->XY.x + window_x,
									bllpH->bap->XY.y + window_y,ReDrawAll);
        		    	
					break;
					case HOOKBLOCK :
						WinEraseChars(bllpH->filename,StrLen(bllpH->filename),
							bllpH->bap->XY.x,bllpH->bap->XY.y+33);
        		    	bllpH->bap->DrawBorder = false;
					break;
					default :
					break;
					
				}
			bllpH->bap->Clean=false;
		/*
		   if(KeyboardObject)
		   {
				EraseDecimalIntKeyboard(KeyboardObject->bap->XY.x,KeyboardObject->bap->XY.y+KeyboardObject->bap->SIZE.y);
		   }
		*/
			}// Clean
			
		}// show==false


	  bllpH = bllpH->NEXTNODE;
	}
	
	// 畫出目前工具圖示
	DrawIcon(BlockPointerState,140,0);
	// 畫出方塊
	DrawIcon(ADDBitmap,120,0);
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
/*
 針對起 終點背景不同
 而需要處理
 有幾種情況
 1. 一點在SYSHOOK 一點在結構中
 2. 一點在結構中 一點在結構中
 不管怎樣都需要處理巢狀結構的問題
 
 5-31 不使用公共變數 SYSHOOK 改以判斷 TYPE != HOOKBLOCK
*/
void CrossWire( )
{
	//WireStart
	//WireStop
	BNP StructBlockP;
	BNP tBNP;
	EndPoint tEP;
	IONODEP ionode;
	Boolean SIDE;
	int MaxID;
	UInt16 true_x_start,true_x_stop;
	//WNP wireLLH;
	UInt16 DeepthStart=0 , DeepthEnd=0;
	
	// 5-18 調整頭尾順序 尾始終為輸入
	if(WireStop.NodeP->TYPE==0)
	{
		tEP = WireStop;
		WireStop = WireStart;
		WireStart = tEP;
	}
	// 5-7 計算頭尾分別巢狀深度
	tBNP = WireStart.BlockP->SelfBlockLLHead;
//	while(tBNP != SYSHOOK->BlockLLHeadP)
	while(tBNP->TYPE != HOOKBLOCK)
	{
		DeepthStart++;
		tBNP = tBNP->SelfBlockLLHead;
	}
	tBNP = WireStop.BlockP->SelfBlockLLHead;
//	while(tBNP != SYSHOOK->BlockLLHeadP)
	while(tBNP->TYPE != HOOKBLOCK)
	{
		DeepthEnd++;
		tBNP = tBNP->SelfBlockLLHead;
	}
	// 5-7 維持　起點的深度　為　最小
	// 但是還是無法解決  跨結構 的問題
	// 5-18 停用
	/*
	if(DeepthStart > DeepthEnd)
	{
		tEP = WireStop;
		WireStop = WireStart;
		WireStart = tEP;
	}
	*/
	// 5-8 判斷跨結構
	//if((DeepthStart != 0) && (DeepthEnd != 0))
	// if(WireStart.BlockP->SelfBlockLLHead)
	
	
	// 5-8 決定新節點的位置
	// 先算出真實位置
	true_x_start=0;
	tBNP = WireStart.BlockP->SelfBlockLLHead;
//	while(tBNP != SYSHOOK->BlockLLHeadP)
	while(tBNP->TYPE != HOOKBLOCK)
	{//4-26 走一遍串列，加出絕對座標
		true_x_start += tBNP->bap->XY.x;
		tBNP = tBNP->SelfBlockLLHead;
	}
	true_x_start += WireStart.BlockP->bap->XY.x;
	true_x_stop=0;
	tBNP = WireStop.BlockP->SelfBlockLLHead;
//	while(tBNP != SYSHOOK->BlockLLHeadP)
	while(tBNP->TYPE != HOOKBLOCK)
	{//4-26 走一遍串列，加出絕對座標
		true_x_stop += tBNP->bap->XY.x;
		tBNP = tBNP->SelfBlockLLHead;
	}
	true_x_stop += WireStop.BlockP->bap->XY.x;
	
	// 5-8 目前是以最對位置來判斷左右邊
	// 將來要以輸出入來判斷左右邊
	// 5-18 以巢狀深度判斷左右邊 這時的start一定是資料起點
	if(DeepthStart > DeepthEnd)
	{
		SIDE=false;
	}
	else
	{
		SIDE=true;
	}
	/*
	if(true_x_start < true_x_stop)
		SIDE = true;
	else
		SIDE = false;
	*/	
	
	//
	// WireStart 是否與 WireStop 同屬一串列，是則跳開
	
	// 5-18 分為兩條路 
	// 1.如果起點最深 則從起點走出來
	// 2.如果終點最深 則從終點倒走出來到起點
	while(WireStart.BlockP->SelfBlockLLHead != WireStop.BlockP->SelfBlockLLHead)
	{
	// 找出WireStart所屬結構
	// 5-18 由深度決定
		if(DeepthStart > DeepthEnd)
		{
			StructBlockP=WireStart.BlockP->SelfBlockLLHead;
		}
		else
		{
			StructBlockP=WireStop.BlockP->SelfBlockLLHead;
		}
		
	// 6-1 找出StructBlock 的 IO 節點最大ID值
	MaxID=0;
	ionode=StructBlockP->bap->IONodeLLHead;
	
	while(ionode!=NULL)
	{
		if((ionode->ID) > MaxID)
			MaxID=ionode->ID;
			
		ionode=ionode->NEXT;
	}
	MaxID++;
	
	// 新增IO節點
	//if( == 1)
	//{//　WireStop的節點 = 輸入 加入節點
	
		ionode = allocate_a_new_IOnode();
		ionode->ID = MaxID;
		// 如果資料起點比較深 表示他在裡面 則新IO點屬性是...
		if(DeepthStart > DeepthEnd)
		{
			ionode->TYPE = 0;
		}
		else
		{
			ionode->TYPE = 1;
		}
		//ionode->TYPE = WireStop.NodeP->TYPE; // 該節點屬性
		ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
		ionode->BYTES=sizeof(int);
		ionode->NotReady=true;
		ionode->NUMS=1;
		// 在左邊還是右邊
		if(SIDE)
		{// 左邊
			ionode->TL.x=0;
		}else{ // 右邊
			ionode->TL.x=StructBlockP->bap->SIZE.x-6;
		}
		//5-18
		if(DeepthStart > DeepthEnd)
		{
			ionode->TL.y=WireStart.BlockP->bap->XY.y;
		}
		else
		{
			ionode->TL.y=WireStop.BlockP->bap->XY.y;
		}
		//ionode->TL.y=WireStart.BlockP->bap->XY.y;
		ionode->EXT.x=5;
		ionode->EXT.y=5;
		
		StructBlockP->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			StructBlockP->bap->IONodeLLHead,
				ionode);
	// 新增線段
	tEP.BlockP=StructBlockP;
	tEP.NodeP=ionode;
	// 5-7 決定使用哪一個 線段串列頭
	// 5-7 以便將線段串列頭更新更新
	//if(IsTheSameWireLLHead(hookp,WireStop.BlockP))
	//{
		//                                      v start  v stop
	//	hookp = AddWireLinkListNode(hookp,WireStop,tEP);
	//}else{
		// 從 WireStop 的元件中找出串列頭型別
		//5-18
	if(DeepthStart > DeepthEnd)
	{
		switch(WireStart.BlockP->SelfBlockLLHead->TYPE)
		{// 以便指定更新哪一個線段串列頭
			case HOOKBLOCK :
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->hookP) = 
				AddWireLinkListNode(
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->hookP),
				WireStart,tEP);
			break;
			case LOOPBLOCK :
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->LoopBlockHOOKP) = 
				AddWireLinkListNode(
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->LoopBlockHOOKP),
				WireStart,tEP);
			break;
			case CASEBLOCK :
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->CurrentHOOKP) = 
				AddWireLinkListNode(
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->CurrentHOOKP),
				WireStart,tEP);
			break;
		}
	}
	else
	{
		switch(WireStop.BlockP->SelfBlockLLHead->TYPE)
		{// 以便指定更新哪一個線段串列頭
			case HOOKBLOCK :
				((HOOKP)WireStop.BlockP->SelfBlockLLHead->hookP) = 
				AddWireLinkListNode(
				((HOOKP)WireStop.BlockP->SelfBlockLLHead->hookP),
				tEP,WireStop);
			break;
			case LOOPBLOCK :
				((HOOKP)WireStop.BlockP->SelfBlockLLHead->LoopBlockHOOKP) = 
				AddWireLinkListNode(
				((HOOKP)WireStop.BlockP->SelfBlockLLHead->LoopBlockHOOKP),
				tEP,WireStop);
			break;
			case CASEBLOCK :
				((HOOKP)WireStop.BlockP->SelfBlockLLHead->CurrentHOOKP) = 
				AddWireLinkListNode(
				((HOOKP)WireStop.BlockP->SelfBlockLLHead->CurrentHOOKP),
				tEP,WireStop);
			break;
		}
	}
	//}
	
	
	// 將之取代WireStop，並重複第1步驟
	//5-18
	if(DeepthStart > DeepthEnd)
	{
		WireStart.BlockP = StructBlockP;
		WireStart.NodeP = ionode;
	}
	else
	{
		WireStop.BlockP = StructBlockP;
		WireStop.NodeP = ionode;
	}
	//StructBlockP->bap->IONodeN++;
	}
	
	
	// 新增線段連接WireStart WireStop 這裡是最後一段 線段
	// 5-18
	//if(DeepthStart > DeepthEnd)
	//{
		switch(WireStart.BlockP->SelfBlockLLHead->TYPE)
		{// 以便指定更新哪一個線段串列頭
			case HOOKBLOCK :
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->hookP) = 
				AddWireLinkListNode(
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->hookP),
				WireStart,WireStop);
			break;
			case LOOPBLOCK :
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->LoopBlockHOOKP) = 
				AddWireLinkListNode(
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->LoopBlockHOOKP),
				WireStart,WireStop);
			break;
			case CASEBLOCK :
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->CurrentHOOKP) = 
				AddWireLinkListNode(
				((HOOKP)WireStart.BlockP->SelfBlockLLHead->CurrentHOOKP),
				WireStart,WireStop);
			break;
		}
	//}
	//else
	//{
	//}
	//hookp = AddWireLinkListNode(hookp,WireStart,WireStop);
	
	//return hookp;
}
/////////////////////////////////////////////
// 5-31 
// 從 CheckWireLink 中分出來
//
void StartStopAddIntoWLL(void )
{
	EndPoint tPoint;

						if((WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP->SelfBlockLLHead)
						|| (WireStart.BlockP == WireStop.BlockP->SelfBlockLLHead) ||
						(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP)) 
						// 5-8 加上判斷結構內元件與結構 IOnode 連接
						{// 當起、終點之所屬串列頭相同時
						    // 找出所屬串列的虎克
						    // 5-8 必須有更新動作，否則內部虎克的wirellhead會失連
						    // 5-8 這裡要決定使用誰的虎克
						    // 如果是相同所屬則是同一個虎克
						    // 如果不同 則要選擇正確的那個
						    if(WireStart.BlockP == WireStop.BlockP->SelfBlockLLHead)
						    {
						   		tPoint = WireStop;
						   	}else if(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP){
						   		tPoint = WireStart;
						   	}else{
						   		tPoint = WireStart;
						   	}
						    switch(tPoint.BlockP->SelfBlockLLHead->TYPE)
						    {
								case HOOKBLOCK :
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->hookP = 
									AddWireLinkListNode(
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->hookP,
									WireStart,WireStop);
								break;
								case LOOPBLOCK :
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->LoopBlockHOOKP = 
									AddWireLinkListNode(
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->LoopBlockHOOKP,
									WireStart,WireStop);
								break;
								case CASEBLOCK :
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->CurrentHOOKP = 
									AddWireLinkListNode(
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->CurrentHOOKP,
									WireStart,WireStop);
								break;
						    }
							//thook = AddWireLinkListNode(thook,WireStart,WireStop);

							//hook = thook; //????
							//hook.BlockLLHeadP->WireLLHeadP = hook.WireLLHeadP;
						}else{
							// 進入跨結構接線處理函式
							// 如果線段頭與線段尾分屬不同串列 則
							//   要需要分段
							// 這些需要分段的線段
							// 會因為在不同的結構中 而需要插入不同的串段
							// 5-8 這裡還有一個意義
							// 結構內的元件 與 結構 IOnode 連接時也會進來這裡
							// 要跳開當一般連接處理(上面判斷式修改)
							// 5-8 CrossWire() 改為由內部決定
							// 需要更新的wirellhead為誰
							CrossWire();
						}
}
// ******************************************
// * 檢查是否可以建立連線
// * 如果是第1次進來，會記住開頭
// * 
// *
// *
// * 2003-4-22 來自CheckOnNode內部獨立出來
// *
// ******************************************
// i就是第幾接點 以0為首(已經減1了)
void CheckWireLink(InAreaInfType InAreaInf,IONODEP NodeP)
{
	//WNP WLLP;
//	EndPoint tPoint;
//	HOOKP thook;
				// wire link
			   //if(CheckWireLink)
			   //{
				if(WireStart.NodeP == NULL)
				{
					WireStart.BlockP = InAreaInf.llp;
					WireStart.NodeP = NodeP;
					//WireStop.x = NULL;
				}
				else if((WireStop.NodeP == NULL) && (WireStart.BlockP != InAreaInf.llp))
				{ //不是同一點
					WireStop.BlockP = InAreaInf.llp;
					WireStop.NodeP = NodeP;
				}
				
				if((WireStart.NodeP != NULL)&&(WireStop.NodeP != NULL))
				{// 當接線建立，檢查接線
					if((WireStart.NodeP->TYPE == false) &&
				 	(WireStop.NodeP->TYPE == false)
				  	)
					{// 輸出不可接在一起
						WireStart.BlockP = NULL;
						WireStop.BlockP = NULL;
					}
				}
				
				if((WireStop.NodeP != NULL) 
					&& (WireStart.NodeP != NULL)
					&& (WireStart.BlockP != WireStop.BlockP)
					)
				{// 連線建立 處理
					// THREAD or SCISSOR ?
					if(BlockPointerState == THREADBitmap)
					{
						StartStopAddIntoWLL();
						// 5-31 獨立出去
						/*
						if((WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP->SelfBlockLLHead)
						|| (WireStart.BlockP == WireStop.BlockP->SelfBlockLLHead) ||
						(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP)) 
						// 5-8 加上判斷結構內元件與結構 IOnode 連接
						{// 當起、終點之所屬串列頭相同時
						    // 找出所屬串列的虎克
						    // 5-8 必須有更新動作，否則內部虎克的wirellhead會失連
						    // 5-8 這裡要決定使用誰的虎克
						    // 如果是相同所屬則是同一個虎克
						    // 如果不同 則要選擇正確的那個
						    if(WireStart.BlockP == WireStop.BlockP->SelfBlockLLHead)
						    {
						   		tPoint = WireStop;
						   	}else if(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP){
						   		tPoint = WireStart;
						   	}else{
						   		tPoint = WireStart;
						   	}
						    switch(tPoint.BlockP->SelfBlockLLHead->TYPE)
						    {
								case HOOKBLOCK :
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->hookP = 
									AddWireLinkListNode(
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->hookP,
									WireStart,WireStop);
								break;
								case LOOPBLOCK :
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->LoopBlockHOOKP = 
									AddWireLinkListNode(
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->LoopBlockHOOKP,
									WireStart,WireStop);
								break;
								case CASEBLOCK :
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->CurrentHOOKP = 
									AddWireLinkListNode(
									(HOOKP)tPoint.BlockP->SelfBlockLLHead->CurrentHOOKP,
									WireStart,WireStop);
								break;
						    }
							//thook = AddWireLinkListNode(thook,WireStart,WireStop);

							//hook = thook; //????
							//hook.BlockLLHeadP->WireLLHeadP = hook.WireLLHeadP;
						}else{
							// 進入跨結構接線處理函式
							// 如果線段頭與線段尾分屬不同串列 則
							//   要需要分段
							// 這些需要分段的線段
							// 會因為在不同的結構中 而需要插入不同的串段
							// 5-8 這裡還有一個意義
							// 結構內的元件 與 結構 IOnode 連接時也會進來這裡
							// 要跳開當一般連接處理(上面判斷式修改)
							// 5-8 CrossWire() 改為由內部決定
							// 需要更新的wirellhead為誰
							CrossWire();
						}
						
						*/
					}
					else if(BlockPointerState == SCISSORBitmap)
					{
					/*
					  if(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP->SelfBlockLLHead)
					  {// 當起、終點之所屬串列頭相同時
					   if(hook->WireLLHeadP != NULL)
						if((WLLP = InWireLinkList(hook,WireStart,WireStop)) != NULL)
						{
							hook->WireLLHeadP = RemoveWireLinkList(hook->WireLLHeadP,WLLP); // 找出LL內的線，移除它
						}
					  }else{
					  
					  }
					 */
					}
				}
			   //}
				//
	//return hook;
}
//
// remove the wire connection when block has been kill
//
// 為了『結構』，要改為可遞迴結構
//
//
// 可遞迴 4/13
//
// blockptr 是要移除的元件
//
// 5-8 移除線段
// 改為由 blockptr 內部的 selfblockllhead 找出 HOOK 然後 以該wirellhead為頭
void BreakWireConnection(BNP  blockptr)
{
	Boolean exist=false;
	WNP WireLLHead,wnp;
	HOOKP thook;
	
	// 5-8 找出 hook
    // 找出所屬串列的虎克
    switch(blockptr->SelfBlockLLHead->TYPE)
    {
		case HOOKBLOCK :
			thook = (HOOKP)blockptr->SelfBlockLLHead->hookP;
		break;
		case LOOPBLOCK :
			thook = (HOOKP)blockptr->SelfBlockLLHead->LoopBlockHOOKP;
		break;
		case CASEBLOCK :
			thook = (HOOKP)blockptr->SelfBlockLLHead->CurrentHOOKP;
		break;
    }
    // 讀出
    WireLLHead = thook->WireLLHeadP;
    wnp = WireLLHead;
	while(wnp != NULL)
	{
		if((wnp->StartBlockP == blockptr) ||
		(wnp->EndBlockP == blockptr)		)
		{
			//tempLink = WLLP->PREVNODE;
			WireLLHead = RemoveWireLinkList(WireLLHead,wnp);
			//WLLP = tempLink;
			if(WireLLHead == NULL)
			 break;
			//break;
			wnp = WireLLHead;
			//continue;
		}
		else
		{
			wnp = wnp->NEXTNODE;
		}
	}
	
	thook->WireLLHeadP = WireLLHead;
	// 更新
    // 找出所屬串列的虎克
    switch(blockptr->SelfBlockLLHead->TYPE)
    {
		case HOOKBLOCK :
			 (HOOKP)blockptr->SelfBlockLLHead->hookP = thook;
		break;
		case LOOPBLOCK :
			 (HOOKP)blockptr->SelfBlockLLHead->LoopBlockHOOKP = thook;
		break;
		case CASEBLOCK :
			 (HOOKP)blockptr->SelfBlockLLHead->CurrentHOOKP = thook;
		break;
    }
}


/*
	功能 轉移元件
	5-7 轉移元件改為修改前後節點
	原本是新增->加入 改為
	只是從這個鏈結 改到 另一個鏈結 而原本的鏈結的地方補起來
	如果原本的鏈結只剩一個 就要注意 原本的鏈結頭要更新
	
	因為外部設計只要抓到的元件就將其推到鏈結的最後面
	所以這裡如果不會有移出鏈結第一個的情形
	所以不需要更新鏈結頭 
	只有在元件剩一個的時候才需要。
*/
static void ChangeLinkList(UInt16 now_x,UInt16 now_y)
{
	//InAreaInfType InAreaInf;
	BNP bllpH;//,insertBlock;
	BNP bllp;
	UInt16 x,y;
	//
	// 4-24 新方法，查詢指標目前落點之串列頭
	//
		if(HandPointer.CatchWhat->BackgroundBlockLLHead != 
				HandPointer.CatchWhat->SelfBlockLLHead)
		{// 終點與起點分屬不同串列，需要搬移
				  // 搬移前，解除連線
			BreakWireConnection(HandPointer.CatchWhat);
				  				
			// 這裡要重寫，針對巢狀迴圈需要
			if(HandPointer.CatchWhat->SelfBlockLLHead == SYSHOOK->BlockLLHeadP)
			{// ************從 主串列 轉移到 結構串列
				
				
					// 要找出真正的相對位置
				x=0;y=0;
				bllp = HandPointer.CatchWhat->BackgroundBlockLLHead;
				
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 走一遍串列，加出絕對座標
					x += bllp->bap->XY.x;
					y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
				// 5-7 改
				HandPointer.CatchWhat->bap->XY.x = now_x - HandPointer.RelativePoint.x - x;
				HandPointer.CatchWhat->bap->XY.y = now_y - HandPointer.RelativePoint.y - y;
				HandPointer.CatchWhat->SelfBlockLLHead=HandPointer.CatchWhat->BackgroundBlockLLHead;
				HandPointer.CatchWhat->NEXTNODE=NULL;
				// 5-7 將原串列修補起來
				if(HandPointer.CatchWhat->PREVNODE != NULL)// 這個串列節點沒有頭
					HandPointer.CatchWhat->PREVNODE->NEXTNODE = HandPointer.CatchWhat->NEXTNODE;
				if(HandPointer.CatchWhat->NEXTNODE != NULL)// 這個串列節點沒有尾
					HandPointer.CatchWhat->NEXTNODE->PREVNODE = HandPointer.CatchWhat->PREVNODE;
				/*
				// 系統串列至少還有一個元件存在，不會成為空串列
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// 這是最後一個鏈結節點
					HandPointer.CatchWhat->SelfBlockLLHead->
				}
				*/


					// 加入『結構』bllpH 是目前結構
				bllpH = HandPointer.CatchWhat->BackgroundBlockLLHead;
				switch(bllpH->TYPE)
				{
				case LOOPBLOCK :
					((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP = 
						InsertIntoBlockLLwithUID(
							((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP,
								HandPointer.CatchWhat);
				break;
				case CASEBLOCK :
					((HOOKP)bllpH->CurrentHOOKP)->BlockLLHeadP = 
						InsertIntoBlockLLwithUID(
							((HOOKP)bllpH->CurrentHOOKP)->BlockLLHeadP,
								HandPointer.CatchWhat);
				break;
				}
				
				
			}else{// 從 結構串列 轉移到 主串列 或是 結構串列 間 轉移
				if(HandPointer.CatchWhat->BackgroundBlockLLHead == SYSHOOK->BlockLLHeadP)
				{// ********* 從 結構串列 轉移到 主串列
					
				// 5-7 將原串列修補起來
				if(HandPointer.CatchWhat->PREVNODE != NULL)// 這個串列節點沒有頭
					HandPointer.CatchWhat->PREVNODE->NEXTNODE = HandPointer.CatchWhat->NEXTNODE;
				if(HandPointer.CatchWhat->NEXTNODE != NULL)// 這個串列節點沒有尾
					HandPointer.CatchWhat->NEXTNODE->PREVNODE = HandPointer.CatchWhat->PREVNODE;
				bllpH = HandPointer.CatchWhat->SelfBlockLLHead;
				switch(bllpH->TYPE)
				{
				case LOOPBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// 這是最後一個鏈結節點
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->LoopBlockHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				case CASEBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// 這是最後一個鏈結節點
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->CurrentHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				}
				// 5-7 改變位置 及 所屬串列
				HandPointer.CatchWhat->bap->XY.x = now_x - HandPointer.RelativePoint.x;
				HandPointer.CatchWhat->bap->XY.y = now_y - HandPointer.RelativePoint.y;
				HandPointer.CatchWhat->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
				HandPointer.CatchWhat->NEXTNODE=NULL;

					// 回到主串列
				SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,HandPointer.CatchWhat);
					// 從原所屬串列中移除
			//		bllpH->BlockLLHeadP = RemoveBlockLinkList(bllpH->BlockLLHeadP,HandPointer.CatchWhat);//deleteBlockH
				}else{//  *********** 結構串列 間 轉移
					// 要找出真正的相對位置
					x=0;y=0;
					bllp = HandPointer.CatchWhat->BackgroundBlockLLHead;
					while(bllp != SYSHOOK->BlockLLHeadP)
					{//4-26 走一遍串列，加出絕對座標
						x += bllp->bap->XY.x;
						y += bllp->bap->XY.y;
						bllp = bllp->SelfBlockLLHead;
					}
					
				// 5-7 將原串列修補起來
				if(HandPointer.CatchWhat->PREVNODE != NULL)// 這個串列節點沒有頭
					HandPointer.CatchWhat->PREVNODE->NEXTNODE = HandPointer.CatchWhat->NEXTNODE;
				if(HandPointer.CatchWhat->NEXTNODE != NULL)// 這個串列節點沒有尾
					HandPointer.CatchWhat->NEXTNODE->PREVNODE = HandPointer.CatchWhat->PREVNODE;
				bllpH = HandPointer.CatchWhat->SelfBlockLLHead;
				switch(bllpH->TYPE)
				{
				case LOOPBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// 這是最後一個鏈結節點
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->LoopBlockHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				case CASEBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// 這是最後一個鏈結節點
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->CurrentHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				}
				// 5-7 改變位置 及 所屬串列
				HandPointer.CatchWhat->bap->XY.x = now_x - HandPointer.RelativePoint.x - x;
				HandPointer.CatchWhat->bap->XY.y = now_y - HandPointer.RelativePoint.y - y;
				HandPointer.CatchWhat->SelfBlockLLHead=HandPointer.CatchWhat->BackgroundBlockLLHead;
				HandPointer.CatchWhat->NEXTNODE=NULL;

					// 進入新串列
					bllpH = HandPointer.CatchWhat->BackgroundBlockLLHead;
			switch(bllpH->TYPE)
			{
				case LOOPBLOCK :
					((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP = 
						InsertIntoBlockLLwithUID(
							((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP,
								HandPointer.CatchWhat);
				break;
				case CASEBLOCK :
					((HOOKP)bllpH->CurrentHOOKP)->BlockLLHeadP = 
						InsertIntoBlockLLwithUID(
							((HOOKP)bllpH->CurrentHOOKP)->BlockLLHeadP,
								HandPointer.CatchWhat);
				break;
			}
					
					
				}
			}
		}
}
Boolean PenDownInBlockToolArea(EventPtr eventP)
{
	Boolean In=false;
	
	// 是否落在工具圖示，若是則進入tools form
	if(CheckBoundary(140,0,18,18,eventP->screenX,eventP->screenY))
	{
		In =true;
		FrmPopupForm(BlockToolsForm);
	}else if(CheckBoundary(120,0,16,16,eventP->screenX,eventP->screenY)){
		In =true;
	// 是否落在方塊圖示，若是則進入functions form
		FrmPopupForm(FunctionsForm);
	}else if(CheckBoundary(100,0,16,16,eventP->screenX,eventP->screenY)){
		In =true;
		if(LAMP)
		{
			LAMP=false;
		}else{
			LAMP=true;
		}
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
	}else if(CheckBoundary(80,0,16,16,eventP->screenX,eventP->screenY)){
		In =true;
		if(STOP==1)
		{
			STOP=0;
			RUN=0;
			RUNFOREVER=false;
			//Animation=false;
			//RunLock=false;
			//RunWho=false;
			DrawIcon(RUNWAITBitmap,60,0);
			DrawIcon(STOPUNUSEBitmap,80,0);
			DrawIcon(RUNRUNWAITBitmap,40,0);
		}
	}else if(CheckBoundary(60,0,16,16,eventP->screenX,eventP->screenY)){
		In =true;
		if(RUN==0)
		{
			STOP=1;
			RUN=1;
			// 清除元件旗標
			CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
			// 線段 狀態重置
			ResetWireStatus(SYSHOOK->BlockLLHeadP->NEXTNODE,
			SYSHOOK->WireLLHeadP);

			FrmGotoForm(BlockDiagramForm);

		}else if(RUN==1){
			RUN=2;
			DrawIcon(PAUSEBitmap,60,0);
		}else if(RUN==2){
			RUN=1;
			DrawIcon(RUNBitmap,60,0);
		}
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
			FrmGotoForm(BlockDiagramForm);
		}
	}else if(CheckBoundary(0,0,33,15,eventP->screenX,eventP->screenY)){
		In =true;
	// 是否落在標題，若是則轉換 form
		FrmGotoForm(FrontPanelForm);
	}
	
	// 6-2 如果筆在非作用區 則不要執行動作
	
	if(CheckBoundary(0,0,160,16,eventP->screenX,eventP->screenY))
	{
		In =true;
	}

	return In;
}
extern void BlockpenDownProcess(EventPtr eventP)
{
	//Handle resH;
	//BitmapType *bitmap;
	UInt16 now_x,now_y;
	UInt16 x,y;
	BNP bllp;
	IONODEP i;
	CTRLNODEP ctrlnode;
	//HOOKP hookdebug;
	InAreaInfType InAreaInf;
	InAreaInf.InArea=false;
	
	// 5-9 修改邊界判斷 now_x 與 now_y 的設定
	CaculateNowXNowY(&now_x,&now_y,eventP->screenX,eventP->screenY);
	/*
		//if(eventP->screenX<10)now_x=10;else now_x = eventP->screenX;
		now_x = eventP->screenX;
		if(eventP->screenY<(10+15))now_y=10+15;else now_y = eventP->screenY;
	*/
	if(PenDownInBlockToolArea(eventP))
	{
	}else{
	switch(BlockPointerState){
		case HANDBitmap:
			// 6-2 執行中不可編輯
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}
		// 5-6 加入功能 凡落在範圍內的元件移到串列最後面 
		// 這樣會使得顯示在最上層 (得到最大Z值)
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
			// 5-30 跳開 預設元件
			if((InAreaInf.InArea)&&(InAreaInf.llp->TYPE != CTRLBLOCK))
			{
				// 移到串列最後面
				NestItemMoveToLastPosition(InAreaInf.llp);
				// in boundary
				switch(InAreaInf.llp->SelfBlockLLHead->TYPE)
				{
					case LOOPBLOCK :
		        	case CASEBLOCK :
						DrawIcon(CLENCHBitmap,now_x+offset_x,now_y+offset_y);
						HandPointer.HandState = CLENCHBitmap;
						HandPointer.CatchWhat = InAreaInf.llp; // point now bap->
				// 複製到 clone 中
					//HandPointer.Clone->TYPE=InAreaInf.llp->TYPE;
					//HandPointer.Clone->bap=InAreaInf.llp->bap;
					//HandPointer.Clone->LoopBlockHOOKP=InAreaInf.llp->LoopBlockHOOKP;
					//HandPointer.Clone->CurrentHOOKP=InAreaInf.llp->CurrentHOOKP;
					//HandPointer.Clone->CaseBlockHOOKLLHeadP=InAreaInf.llp->CaseBlockHOOKLLHeadP;
					//HandPointer.Clone->SelfBlockLLHead=InAreaInf.llp->SelfBlockLLHead;
					//HandPointer.Clone->BackgroundBlockLLHead=InAreaInf.llp->BackgroundBlockLLHead;
					//HandPointer.Clone->bap->ReDraw=true;
						// 相對位置要重算

						x=0;y=0;
						bllp = HandPointer.CatchWhat->BackgroundBlockLLHead;
						while(bllp != SYSHOOK->BlockLLHeadP)
						{
							x += bllp->bap->XY.x;
							y += bllp->bap->XY.y;
							bllp = bllp->SelfBlockLLHead;
						}


						HandPointer.RelativePoint.x = now_x - x - InAreaInf.llp->bap->XY.x;
						HandPointer.RelativePoint.y = now_y - y - InAreaInf.llp->bap->XY.y;
		        	break;
		        	
		        	default:
						DrawIcon(CLENCHBitmap,now_x+offset_x,now_y+offset_y);
						HandPointer.HandState = CLENCHBitmap;
						HandPointer.CatchWhat = InAreaInf.llp; // point now bap->
						// 相對位置要重算
						
						x=0;y=0;
						
						bllp = HandPointer.CatchWhat->BackgroundBlockLLHead;
						while(bllp != SYSHOOK->BlockLLHeadP)
						{
							x += bllp->bap->XY.x;
							y += bllp->bap->XY.y;
							bllp = bllp->SelfBlockLLHead;
						}
						
						
						HandPointer.RelativePoint.x = now_x - x - InAreaInf.llp->bap->XY.x;
						HandPointer.RelativePoint.y = now_y - y - InAreaInf.llp->bap->XY.y;
		        	break;
					
				}
				//////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////
				//HandPointer.StartbllpH = InAreaInf.llp;
				// 4-23 紀錄元件所屬串列頭，以備元件轉移之用
				// 4-24 刪除該變數，改為Block內部自己記住頭位置
			}// if inarea
			else
			{
				DrawIcon(HANDBitmap,now_x+offset_x,now_y+offset_y);
				HandPointer.HandState = HANDBitmap;
			}
		break;
		case THREADBitmap:
			// 6-2 執行中不可編輯
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); 
			if(InAreaInf.InArea)
			{
				// check on node? and check wire
				i=CheckOnNode(InAreaInf,now_x,now_y,true);
				CheckWireLink(InAreaInf,i);
			}
			else
			{
				DrawIcon(THREADBitmap,now_x+offset_x,now_y+offset_y);
		
			}
		break;
		case DRAGBitmap:
			// 6-2 執行中不可編輯
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); 
			if(InAreaInf.InArea)
			{
				// 移到串列最後面
				NestItemMoveToLastPosition(InAreaInf.llp);

			//if(InAreaInf.llp->bap->BITMAPID == WHILELOOPBitmap)
				switch(InAreaInf.llp->TYPE)
				{//按到 wHileloop
				 case LOOPBLOCK :
					if(((ctrlnode = CheckOnCTRLNode(InAreaInf,now_x,now_y)) != NULL )&& 
					       (ctrlnode->ID == DRAGNODE))
					{// 有按到 drag point
						DragStatus.llp = InAreaInf.llp;
						InAreaInf.llp->bap->ReDraw=true;
					}
				 break;
				 case CASEBLOCK:
					if((ctrlnode = CheckOnCTRLNode(InAreaInf,now_x,now_y)) != NULL )
					{
						InAreaInf.llp->bap->ReDraw=true;
						//hookdebug = (HOOKP)InAreaInf.llp->CurrentHOOKP;
						switch(ctrlnode->ID)
						{
							case DRAGNODE:
								DragStatus.llp = InAreaInf.llp;
							break;
							case SCPAGEUP:
								(HOOKP)InAreaInf.llp->CurrentHOOKP=((HOOKP)((HOOKP)InAreaInf.llp->CurrentHOOKP)->NEXT);
							break;
							case SCPAGEDOWN:
								(HOOKP)InAreaInf.llp->CurrentHOOKP=((HOOKP)((HOOKP)InAreaInf.llp->CurrentHOOKP)->PREV);
							break;
						}
					}
				 break;
				 
				}
			}else{
			// 畫 ICON
			}
		break;
		case SCISSORBitmap:
			// 6-2 執行中不可編輯
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
			if(InAreaInf.InArea)
			{
				// check on node? and check wire
				i=CheckOnNode(InAreaInf,now_x,now_y,true);
				CheckWireLink(InAreaInf,i);
			}
			else
			{
				DrawIcon(SCISSORBitmap,now_x+offset_x,now_y+offset_y);
		
			}
		break;
		case KILLBitmap:
			// 6-2 執行中不可編輯
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}
			DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,false,0,0,false);
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
			if(InAreaInf.InArea)
			{
				
				// clear Block bitmap image
				DrawIcon(BLANKBitmap,InAreaInf.llp->bap->XY.x,InAreaInf.llp->bap->XY.y);
				// remove wire connection
				BreakWireConnection(InAreaInf.llp);
				// remove Block Link
				// 5-30 這裡應該在審慎處理 因為有的方塊裡面有動態指標 需一個個移除
				SYSHOOK->BlockLLHeadP = RemoveBlockLinkList(SYSHOOK->BlockLLHeadP,InAreaInf.llp);

			}
			else
			{
				DrawIcon(KILLBitmap,now_x+offset_x,now_y+offset_y);
		
			}
			DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,false);
		break;
		case PENCILBitmap:
		/*
			if(KeyboardObject)
			{
				
			}else
			{
				InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
				if(InAreaInf.InArea)
				{
					KeyboardObject=InAreaInf.llp;
					InAreaInf.llp->bap->ReDraw=true;
					DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,false);
				}
			}
		*/
		break;
		default:
		break;
	}// switch end
	}// else if end
		//
			prev_x = now_x;
			prev_y = now_y;
			//penMove Average = 0;

	
}
extern void BlockpenMoveProcess(EventPtr eventP)
{

	//Handle resH;
	//BitmapType *bitmap;
	UInt16 now_x,now_y;
	BNP llp;
	BNP Dragllp;
	IONODEP ionode;
	//int i;
	//Boolean InAreaInf=false;
	InAreaInfType InAreaInf;
	InAreaInf.InArea=false;
	
	// 5-9 修改邊界判斷 now_x 與 now_y 的設定
	CaculateNowXNowY(&now_x,&now_y,eventP->screenX,eventP->screenY);
/*
		//if(eventP->screenX<10)now_x=10;else now_x = eventP->screenX;
		now_x = eventP->screenX;
		if(eventP->screenY<(10+15))now_y=10+15;else now_y = eventP->screenY;
*/		
		
		// tool = hand
	if(CheckBoundary(0,0,160,16,eventP->screenX,eventP->screenY))
	{
	}else{

	switch(BlockPointerState){
		case HANDBitmap:

		
			if(HandPointer.HandState == CLENCHBitmap)
			{
			/*
				// 5-12 元件重繪(清除)
				HandPointer.CatchWhat->bap->ReDraw = true;
			if(HandPointer.CatchWhat->SelfBlockLLHead == HandPointer.CatchWhat->BackgroundBlockLLHead)
			{
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
			}
			else
			{
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
				if(HandPointer.CatchWhat->BackgroundBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
				NestReDraw(HandPointer.CatchWhat->BackgroundBlockLLHead);
				// 脫離在這裡發生
				}
			}
			*/	
				/*
				// 5-12 巢狀重繪
				llp = HandPointer.CatchWhat->SelfBlockLLHead;
				while(llp != SYSHOOK->BlockLLHeadP)
				{
					llp->bap->ReDraw=true;
					llp=llp->SelfBlockLLHead;
				}
				llp->bap->ReDraw=true;
				*/
		 // clear diagram
		 // 5-13 這一次改成 只要是上一次有出現 ReDraw 的元件 這裡就把它清除
		 // 但是造成只要元件進去出來 該背景就會消失
		DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,false,0,0,false);
		// 
		
		// clear prev. pointer icon
		DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				// 設定新座標值
				setNewObjXY(now_x-HandPointer.RelativePoint.x,now_y-HandPointer.RelativePoint.y);
				// 複製到 clone 中
				// 5-12 為了讓元件搬移中 跨結構時還能正常顯示
				// 但是目前有問題 無法計算螢幕位置
				// 5-12 廢除
					//HandPointer.Clone->TYPE=HandPointer.CatchWhat->TYPE;
					//HandPointer.Clone->bap=HandPointer.CatchWhat->bap;
					//HandPointer.Clone->LoopBlockHOOKP=HandPointer.CatchWhat->LoopBlockHOOKP;
					//HandPointer.Clone->CurrentHOOKP=HandPointer.CatchWhat->CurrentHOOKP;
					//HandPointer.Clone->CaseBlockHOOKLLHeadP=HandPointer.CatchWhat->CaseBlockHOOKLLHeadP;
					//HandPointer.Clone->SelfBlockLLHead=HandPointer.CatchWhat->SelfBlockLLHead;
					//HandPointer.Clone->BackgroundBlockLLHead=HandPointer.CatchWhat->BackgroundBlockLLHead;
					//HandPointer.Clone->bap->ReDraw=true;

				// in boundary
				
				// 5-12 元件重繪
				HandPointer.CatchWhat->bap->ReDraw = true;
				HandPointer.CatchWhat->bap->Clean = true;

				llp = SYSHOOK->BlockLLHeadP->NEXTNODE;
				do
				{
					InAreaInf.InArea = false;
					InAreaInf = IsInArea(InAreaInf,llp,now_x,now_y,0,0); //LLP will set
					
					if(InAreaInf.InArea)
					{// 找到落在元件上，但或許是手中那個元件，因此先跳過
						llp = InAreaInf.llp->NEXTNODE;
					}
					
					
				}while((InAreaInf.InArea == true) &&
				(HandPointer.CatchWhat == InAreaInf.llp));//避免是手中那個元件
				
				if(InAreaInf.InArea)
				{

					switch(InAreaInf.llp->TYPE)
					{
						case LOOPBLOCK :
						// 5-12 巢狀重繪 向下探尋 往內部探尋
						NestReDraw(((HOOKP)InAreaInf.llp->LoopBlockHOOKP)->BlockLLHeadP);

						// 5-12 巢狀重繪 向下探尋 往來處探尋
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
						NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
				}
			        		//落點在新的結構中，畫外框
			        		InAreaInf.llp->bap->DrawBorder = true;
							// 修改background
							HandPointer.CatchWhat->BackgroundBlockLLHead = InAreaInf.llp;
							//BackgroundBlockLLHead;
						break;
		        		case CASEBLOCK :
						// 5-12 巢狀重繪 向下探尋 往內部探尋
						NestReDraw(((HOOKP)InAreaInf.llp->CurrentHOOKP)->BlockLLHeadP);
						
						// 5-12 巢狀重繪 向下探尋 往來處探尋
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
						NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
				}
			        		//落點在新的結構中，畫外框
			        		InAreaInf.llp->bap->DrawBorder = true;
							// 修改background
							HandPointer.CatchWhat->BackgroundBlockLLHead = InAreaInf.llp;
							//BackgroundBlockLLHead;
						break;
						default:
							// 不是落在結構中
							// 修改background
							//HandPointer.CatchWhat->BackgroundBlockLLHead = SYSHOOK->BlockLLHeadP;
						break;
					}
				}else{
					// 沒有落在元件上
					// 修改background
					HandPointer.CatchWhat->BackgroundBlockLLHead = SYSHOOK->BlockLLHeadP;
					
			// 5-12 巢狀重繪 向下探尋
			if(HandPointer.CatchWhat->SelfBlockLLHead == HandPointer.CatchWhat->BackgroundBlockLLHead)
			{
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
			}
			else
			{// 元件離開所屬背景
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
				if(HandPointer.CatchWhat->BackgroundBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
				NestReDraw(HandPointer.CatchWhat->BackgroundBlockLLHead);
				// 脫離在這裡發生
				}
			}
			
			
				}

				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0,true); 

			// 5-13 設定ESCout 變數
			if(HandPointer.CatchWhat->SelfBlockLLHead == HandPointer.CatchWhat->BackgroundBlockLLHead)
			{
				if(ESCout != NULL)
				{
					ESCout->bap->ReDraw = true;
					ESCout->bap->Clean = false;
					ESCout = NULL;
				}
			}
			else
			{// 元件離開所屬背景
				if(ESCout != HandPointer.CatchWhat->BackgroundBlockLLHead)
				{// 元件　雖離開所屬背景　但與上一次停留背景不同　必須令上次背景重繪
					if(ESCout != NULL)
					{
						ESCout->bap->ReDraw = true;
						ESCout->bap->Clean = false;
					}	
					ESCout = HandPointer.CatchWhat->BackgroundBlockLLHead;
					//NestReDraw(ESCout);
				}
			}
			
			// 5-12 巢狀重繪 向下探尋 一旦所抓本身是結構 本身又處串列最後一個
			NestReDraw(HandPointer.CatchWhat);
			
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,false); // redraw diagram
	// 只畫這個元件
	//DrawDiagram(HandPointer.Clone,NULL,true,0,0,false); //
				DrawIcon(CLENCHBitmap,now_x+offset_x,now_y+offset_y);
			}
			else if(HandPointer.HandState == HANDBitmap)
			{
		//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,0,0,0); // clear diagram
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,false); // redraw diagram
		// clear prev. pointer icon
		DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0,true); // redraw diagram
				DrawIcon(HANDBitmap,now_x+offset_x,now_y+offset_y);
			}

		//
		break;
		case DRAGBitmap:
			if(DragStatus.llp != NULL)
			{
				//DragStatus.llp->bap->Clean=true;// 為了清除這個元件
				/*
				// 5-12 巢狀重繪
				llp = DragStatus.llp->SelfBlockLLHead;
				while(llp != SYSHOOK->BlockLLHeadP)
				{
					llp->bap->Clean=true;
					llp=llp->SelfBlockLLHead;
				}
				llp->bap->Clean=true;
				*/
				DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,false,0,0,false); // clear diagram
				
		// clear prev. pointer icon
		//DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
		// 應該改為巢狀處理
		// 5-13 改為巢狀處理
				DragStatus.llp->bap->SIZE.x = now_x - 
					DragStatus.llp->bap->XY.x ;
				DragStatus.llp->bap->SIZE.y = now_y - 
					DragStatus.llp->bap->XY.y ;
				Dragllp = DragStatus.llp->SelfBlockLLHead;
				while(Dragllp != SYSHOOK->BlockLLHeadP)
				{
				DragStatus.llp->bap->SIZE.x -= Dragllp->bap->XY.x;
				DragStatus.llp->bap->SIZE.y -= Dragllp->bap->XY.y;
					Dragllp = Dragllp->SelfBlockLLHead;
				}
				/*
				DragStatus.llp->bap->SIZE.x = now_x - 
					DragStatus.llp->bap->XY.x - 
						DragStatus.llp->SelfBlockLLHead->bap->XY.x;
				DragStatus.llp->bap->SIZE.y = now_y - 
					DragStatus.llp->bap->XY.y - 
						DragStatus.llp->SelfBlockLLHead->bap->XY.y;
				*/		
				if(DragStatus.llp->bap->SIZE.x < 32)
					DragStatus.llp->bap->SIZE.x = 32;
				if(DragStatus.llp->bap->SIZE.y < 32)
					DragStatus.llp->bap->SIZE.y = 32;
					
				// 5-13 最大限制
				if(DragStatus.llp->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
					if((DragStatus.llp->bap->SIZE.x + DragStatus.llp->bap->XY.x) > 
					(DragStatus.llp->SelfBlockLLHead->bap->SIZE.x-5))
					DragStatus.llp->bap->SIZE.x = 
					(DragStatus.llp->SelfBlockLLHead->bap->SIZE.x-5) - DragStatus.llp->bap->XY.x;
					
					if((DragStatus.llp->bap->SIZE.y  + DragStatus.llp->bap->XY.y) > 
					(DragStatus.llp->SelfBlockLLHead->bap->SIZE.y-5))
					DragStatus.llp->bap->SIZE.y = 
					(DragStatus.llp->SelfBlockLLHead->bap->SIZE.y-5) - DragStatus.llp->bap->XY.y;
				}
				
				DragStatus.llp->bap->ReDraw=true;
				DragStatus.llp->bap->Clean=true;
				// 5-12 巢狀重繪 向下探尋 5-13 改
				if(DragStatus.llp->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
					NestReDraw(DragStatus.llp->SelfBlockLLHead);
				// 5-13 試驗
				Dragllp=DragStatus.llp->SelfBlockLLHead;
				while(Dragllp != SYSHOOK->BlockLLHeadP)
				{
					Dragllp->bap->ReDraw=true;
					Dragllp->bap->Clean=true;
					Dragllp=Dragllp->SelfBlockLLHead;
				}
				// 5-18 修改輸出節點位置
				ionode = DragStatus.llp->bap->IONodeLLHead;
				while(ionode!=NULL)
				{
					if(ionode->TYPE==0)
					{
						ionode->TL.x = DragStatus.llp->bap->SIZE.x-5;
					}
					ionode=ionode->NEXT;
				}
				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0,false); 
					//WinDrawLine(DragStatus.llp->bap->XY.x,DragStatus.llp->bap->XY.y,DragStatus.llp->bap->XY.x+DragStatus.llp->bap->SIZE.x,DragStatus.llp->bap->XY.y+DragStatus.llp->bap->SIZE.y);
			}else{
			// 畫 ICON
			}
		/*
		DragStatus.llp->bap->ReDraw=true;// 為了清除這個元件
		// 5-12 巢狀重繪
		llp = DragStatus.llp->SelfBlockLLHead;
		while(llp != SYSHOOK->BlockLLHeadP)
		{
			llp->bap->ReDraw=true;
			llp=llp->SelfBlockLLHead;
		}
		llp->bap->ReDraw=true;
		*/
		DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,false); // redraw diagram
		
		break;
		case THREADBitmap:
		// clear prev. pointer icon
		DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
			//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0,false); // redraw diagram
			
		//if(LLP->item[0].UID != NULL)
			//for(i=0;i<5 && !InAreaInf;i++) // now have 5 items on diagram
			
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0);
			// LLP is set
			//CheckOnNode(prev_x,prev_y,false);

			if(InAreaInf.InArea)
			{
				DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,true); // redraw diagram
			
				DrawIcon(THREADBitmap,now_x+offset_x,now_y+offset_y);
				// check on node?
				CheckOnNode(InAreaInf,now_x,now_y,true);
			}
			else
			{
			
		// clear prev. bitmap
				//DrawIcon(BLANKBitmap,prev_x-10,prev_y-10);
			
				DrawIcon(THREADBitmap,now_x+offset_x,now_y+offset_y);
			}
		
	//DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,true); // redraw diagram
		
		break;
		case SCISSORBitmap:
		// clear prev. pointer icon
		DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
			//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0,false); // redraw diagram
			
			
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0);
			// LLP is set

			if(InAreaInf.InArea)
			{
				DrawIcon(SCISSORBitmap,now_x+offset_x,now_y+offset_y);
				// check on node?
				CheckOnNode(InAreaInf,now_x,now_y,true);
			}
			else
			{
			
			
				DrawIcon(SCISSORBitmap,now_x+offset_x,now_y+offset_y);
			}
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,false); // redraw diagram
			//Draw Diagram(1); // redraw diagram
		break;
		case KILLBitmap:
				// clear prev. icon image
			DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,false); // redraw diagram
			//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0,false); // redraw diagram
		break;
		default:
		break;
		}
	}
//	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,true,0,0,false); // redraw diagram
			prev_x = now_x;
			prev_y = now_y;
	

	
}

 void BlockpenUpProcess(EventPtr eventP)
{
//	Handle resH;
//	BitmapType *bitmap;
    FormPtr frmP;
	UInt16 now_x,now_y;
	IONODEP i;
	CTRLNODEP ctrlnode;
	InAreaInfType InAreaInf;
	InAreaInf.InArea=false;

	
	// 5-9 修改邊界判斷 now_x 與 now_y 的設定
	CaculateNowXNowY(&now_x,&now_y,eventP->screenX,eventP->screenY);
/*
		//if(eventP->screenX<10)now_x=10;else now_x = eventP->screenX;
		now_x = eventP->screenX;
		if(eventP->screenY<(10+15))now_y=10+15;else now_y = eventP->screenY;
*/
	//DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,false,0,0,true); // erase diagram

		// tool = hand
	if(CheckBoundary(0,0,160,16,eventP->screenX,eventP->screenY))
	{
	}else{
	switch(BlockPointerState){
		case HANDBitmap:
		
//			if(eventP->screenY > 15 && HandPointer.HandState != NULL)
			if(HandPointer.HandState != NULL)
			{
		
				//if((prev_x) < 10)prev_x=10;
				//if((prev_y) < 10+15)prev_y=10+15;
			 if(HandPointer.HandState == CLENCHBitmap)
			 {
				// 4-23 計畫：當手放開，檢查...
				// 1. 起點所在的串列『頭』與終點所在串列頭 是否不同？
				// 2. 不同：表示需要轉移元件所在串列
				// 3. 同：表示只是元件移動
				// 5-13
				// 4. 全線移到串列最後面
				//
				// ChangeLinkList()
				// 4-24 完成
				HandPointer.CatchWhat->bap->Clean=true;//5-12
				DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,0,0,0,false); // erase diagram
				ChangeLinkList(now_x,now_y);
				// 巢狀 移到串列最後面
        		NestItemMoveToLastPosition(HandPointer.CatchWhat);
			 }
				// clear prev. icon image
				DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				
				frmP = FrmGetActiveForm(); //redraw form
				//MainFormInit( frmP);
				FrmDrawForm ( frmP);
				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0); // redraw diagram
				//DrawMainRUN();
			}
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
			HandPointer.CatchWhat = NULL;
			HandPointer.HandState = NULL;
		break;
		case DRAGBitmap:
			if(DragStatus.llp != NULL)
			{	// 多個控制點
			    ctrlnode = DragStatus.llp->bap->CTRLNodeLLHead;
			    while(ctrlnode != NULL)
			    {
			    	switch(ctrlnode->ID)
			    	{
			    		case DRAGNODE:
			    			ctrlnode->TL.x = 
								DragStatus.llp->bap->SIZE.x - 
									ctrlnode->EXT.x;
			    			ctrlnode->TL.y = 
								DragStatus.llp->bap->SIZE.y - 
									ctrlnode->EXT.y;
			    		break;
			    		case SCPAGEUP:
			    			ctrlnode->TL.x = 
								DragStatus.llp->bap->SIZE.x - 
									ctrlnode->EXT.x;
			    		break;
			    		case SCPAGEDOWN:
			    		// 不用改變
			    		break;
			    	}
					
			    	ctrlnode = ctrlnode->NEXT;
			    }
			    /*
				DragStatus.llp->bap->CTRLNodeLLHead->TL.x = 
				DragStatus.llp->bap->SIZE.x - 
					DragStatus.llp->bap->CTRLNodeLLHead->EXT.x;
				DragStatus.llp->bap->CTRLNodeLLHead->TL.y = 
				DragStatus.llp->bap->SIZE.y - 
					DragStatus.llp->bap->CTRLNodeLLHead->EXT.y;
				*/
				DragStatus.llp = NULL;

	//DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
			}else{
			// 清除 ICON
			}
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
		break;
		case THREADBitmap:
			//if(eventP->screenY > 15 )
			//{
		
				//if((prev_x) < 10)prev_x=10;
				//if((prev_y) < 10+15)prev_y=10+15;

			  InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
			  if(InAreaInf.InArea)
			  {
				// check on node? and check wire
				i=CheckOnNode(InAreaInf,now_x,now_y,true);
				CheckWireLink(InAreaInf,i);
			  }
				
				// clear prev. icon image
				DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				
				frmP = FrmGetActiveForm(); //redraw form
				//MainFormInit( frmP);
				FrmDrawForm ( frmP);
				
				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0); // redraw diagram
				//DrawMainRUN();
				WireStart.BlockP = NULL;
				WireStart.NodeP = NULL;
				WireStop.BlockP = NULL;
				WireStop.NodeP = NULL;

	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,true); // redraw diagram

			//}
		
		break;
		case SCISSORBitmap:
				DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,0,0,0,false); // erase diagram

				//if((prev_x) < 10)prev_x=10;
				//if((prev_y) < 10+15)prev_y=10+15;
				
			  InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
			  if(InAreaInf.InArea)
			  {
				// check on node? and check wire
				i = CheckOnNode(InAreaInf,now_x,now_y,true);
				CheckWireLink(InAreaInf,i);
			  }
				// clear prev. icon image
				DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				
				frmP = FrmGetActiveForm(); //redraw form
				//MainFormInit( frmP);
				FrmDrawForm ( frmP);
				
				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0); // redraw diagram
				//DrawMainRUN();
				WireStart.BlockP = NULL;
				WireStop.BlockP = NULL;

	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
			
		break;
		case KILLBitmap:
				//if((prev_x) < 10)prev_x=10;
				//if((prev_y) < 10+15)prev_y=10+15;
				
				// clear prev. icon image
				DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				
				frmP = FrmGetActiveForm(); //redraw form
				//MainFormInit( frmP);
				FrmDrawForm ( frmP);
				
				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0); // redraw diagram
				//DrawMainRUN();

	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
			
		break;
		case PENCILBitmap:
			//InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
			//if(InAreaInf.InArea)
			/*
			if(KeyboardObject)
			{
				KeyboardObject->bap->ReDraw=true;
				
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,false,0,0,false); // clear keyboard
	
				KeyboardObject=NULL;
			}
			*/
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
		break;

		default:
		break;
	}
	}
	//DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true); // redraw diagram
	DrawMainRUN();

}
/***********************************************************************
*	Block DoCommand routine
*	
*	
*	
*	
*	
*	
************************************************************************/
static Boolean BlockDiagramFormDoCommand(Word command)
{
	Boolean handled = false;
	//FormPtr frmP;

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
		case BlockDiagramWorksBlockDiagram:
			MenuEraseStatus (0);
			FrmGotoForm(BlockDiagramForm);
			handled = true;
			break;
		case BlockDiagramWorksFrontPanel:
			MenuEraseStatus (0);
			FrmGotoForm(FrontPanelForm);
			handled = true;
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
		case BlockDiagramWorksFunctions:
			MenuEraseStatus (0);
			//WaitFileLoad = false;
			FrmGotoForm(FunctionsForm);
			handled = true;
			break;
		case FileNEW:
			MenuEraseStatus (0);
			NEW();
			FrmGotoForm(BlockDiagramForm);
			handled = true;
			break;
		case FileSAVE:
			MenuEraseStatus (0);
			FrmPopupForm(SAVEForm);
			//SAVE("test");
			handled = true;
			break;
		case FileLOAD:
			MenuEraseStatus (0);
			CUSTOMLOAD=false;
			DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,false,0,0,true);
			FrmPopupForm(LOADForm);
			//FrmGotoForm(FilesForm);
			//WhoLoad = menuLoad;
			//FrmPopupForm(FilesForm);
			handled = true;
			break;
		}
	return handled;
}

/***********************************************************************
*	Block event process routine
*	
*	
*	
*	
*	
*	
************************************************************************/
extern Boolean BlockDiagramFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;


	switch (eventP->eType) 
		{
		case menuEvent:
			BlockPointerState=HANDBitmap;
			return BlockDiagramFormDoCommand(eventP->data.menu.itemID);

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm ( frmP);
			// Do draw diagram
			DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,1,0,0,true);
			// draw main run button
			DrawMainRUN();

			DragStatus.llp = NULL;
			ESCout = NULL;
			handled = true;
			break;
		case penDownEvent:
			//frmP = FrmGetActiveForm();
			//StrPrintF(str,"%d",FrmGetActiveFormID());
			//WinDrawChars(str,StrLen(str),eventP->screenX,eventP->screenY);
			
			
			//if(eventP->screenY > 15) //  5-9 為了tools palette 拿掉限制
			//{
				// Do check diagram
				BlockpenDownProcess(eventP);
			//}
			
			handled = true;
			break;
		case penMoveEvent:
				
			//penMoveAverage++;
			if(penMoveAverage == 0)
			{
				BlockpenMoveProcess(eventP);
				penMoveAverage =_PenMovAver;
			}else{
			 	penMoveAverage --;
			}
			
			handled = true;
			break;
		case penUpEvent:
		
			BlockpenUpProcess(eventP);
			
			handled = true;
			break;
		case keyDownEvent:
			switch(KeyCurrentState())
			{
				case keyBitPageUp:
					if(BlockPointerState == HANDBitmap)
					{
						BlockPointerState = DRAGBitmap;
					}
					else if(BlockPointerState == DRAGBitmap)
					{
						BlockPointerState = THREADBitmap;
					}
					else if(BlockPointerState == THREADBitmap)
					{
						BlockPointerState = SCISSORBitmap;
					}
					else if(BlockPointerState == SCISSORBitmap)
					{
						BlockPointerState = KILLBitmap;
					}
					else if(BlockPointerState == KILLBitmap)
					{
						BlockPointerState = HANDBitmap;
					}
					DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,false);
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

		// 等到 block 的formload 事件進來 才進行執行動作 以便
		// block form 可以顯示動畫
//		if((event.eType == frmLoadEvent)&&(event.data.frmLoad.formID ==BlockDiagramForm))
//			RunLock=true;
		
		// 2個情況 可以執行
		// 1. 燈打開 由 panel 下達執行 這時會轉往 block 處 後才執行
		// 2. 燈沒開 由 panel 下達執行
		//if((RUN==1 && RunLock)||(RUN==1 && LAMP==false))
		if(RUN==1)
		{
			DrawIcon(RUNBitmap,60,0);
			
			if(DoRun(SYSHOOK->BlockLLHeadP->NEXTNODE,SYSHOOK->WireLLHeadP))
			{
			}else{
				if(RUNFOREVER)
				{
					// 清除元件旗標
					CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
					// 線段 狀態重置
					ResetWireStatus(SYSHOOK->BlockLLHeadP->NEXTNODE,
					SYSHOOK->WireLLHeadP);

				}else
				{
					RUN=0;
					STOP=0;
				
				DrawIcon(RUNWAITBitmap,60,0);
				// 6-2 清除畫面
				frmP = FrmGetActiveForm();
				WinEraseWindow();
				FrmDrawForm ( frmP);
				DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,true);
				}
			}
			//if(LAMP)
			//	Animation=true;
			
			/*
			if(RunWho == false)
			{
				DoWireRun(SYSHOOK->WireLLHeadP);
				RunWho=true;
			}else{
				RunWho=false;
				if(DoBlockRun(SYSHOOK->BlockLLHeadP->NEXTNODE))
				{// 傳回 true 表示 執行完一次 wire 以及 block run
				 // 但還有未完成
				}else{// 傳回 false 表示整體執行一次結束
					RUN=0;
					STOP=0;
					RunLock=false;
					DrawIcon(RUNWAITBitmap,60,0);
				}
			}
			*/
		}
		


				// 線動畫	
			//if(Animation)
			//{
			//	Animation=false;
			//}
		break;
		default:
			break;
		
		}
	
	return handled;
}
