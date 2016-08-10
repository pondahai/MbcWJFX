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
extern HOOKP SYSHOOK;	// �{���D��C���Y
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

// ��������쥻���ݦ�C
BNP ESCout;
// �ʵe
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
// * 4-24 �[�J�B�z���c�\��
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
		
		// ���L�t�u�q
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
			// ���b�ʵe�� �ӥB ���b run �h �M���ܼ�
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
			// ���B�z���c�����󱵽u
			// �p�⵴��y��
			if(startblock->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
			{
				StartWindow.x=0;StartWindow.y=0;
				bllp = startblock->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 ���@�M��C�A�[�X����y��
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
				{//4-26 ���@�M��C�A�[�X����y��
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
			// debug �q�X�`�I���A
			StrPrintF(debugtxt,"%c",(startnode->NotReady)?'T':'F');
				WinDrawChars(debugtxt,
					StrLen(debugtxt),
			 			start.x ,
						start.y -16);
			// debug �q�X�`�I���A
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
			// debug �q�X�`�I���A
			StrPrintF(debugtxt,"%c",(startnode->NotReady)?'T':'F');
				WinEraseChars(debugtxt,
					StrLen(debugtxt),
			 			start.x ,
						start.y -16);
			// debug �q�X�`�I���A
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
		}// if �t�u�q
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
	//�e�خ�
	WinDrawLine(tl.x,tl.y,bl.x,bl.y);
	WinDrawLine(bl.x,bl.y,br.x,br.y);
	WinDrawLine(br.x,br.y,tr.x,tr.y);
	WinDrawLine(tr.x,tr.y,tl.x,tl.y);
	WinDrawLine(tl.x+(llp->bap->SIZE.x/2),tl.y,tl.x+(llp->bap->SIZE.x/2),bl.y);
	//�e��l ���@�MIOnode�쵲��C
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
		// ���L�t�u�q
		ItIsNotDarkLine=true;
		
	if((wnp->StartBlockP->TYPE==HOOKBLOCK)&&(wnp->StartBlockP == SYSHOOK->BlockLLHeadP))
		ItIsNotDarkLine=false;
		
	if((wnp->EndBlockP->TYPE==HOOKBLOCK)&&(wnp->EndBlockP == SYSHOOK->BlockLLHeadP))
		ItIsNotDarkLine=false;
	

	// �M�w�ʵe��V
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
		//if(wnp->DIRTY)// dirty ��ܸӽu�q�w�g����L
		// 6-2 �ӽu�q�ǳư��� �٨S����I
		if( ( (wnp->StartNodeP->NotReady == false) && (wnp->EndNodeP->NotReady == true) 
		&& ( ((wnp->RUNPOINT.x == -1)&&(wnp->RUNPOINT.y == -1))?false:true) ) )
		{
	// 6-2
	No_Any_Data_Move=false;
	// �p�⵴��y��
	StartNode.x=wnp->StartNodeP->TL.x +(wnp->StartNodeP->EXT.x/2)+ wnp->StartBlockP->bap->XY.x;
	StartNode.y=wnp->StartNodeP->TL.y +(wnp->StartNodeP->EXT.y/2)+ wnp->StartBlockP->bap->XY.y;
	if(wnp->StartBlockP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
	{
		bllp = wnp->StartBlockP->SelfBlockLLHead;
		while(bllp != SYSHOOK->BlockLLHeadP)
		{//4-26 ���@�M��C�A�[�X����y��
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
		{//4-26 ���@�M��C�A�[�X����y��
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
				// ��J���I ���o�|�I
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
				
				// ����I
				if((wnp->RUNPOINT.x == EndNode.x)&&
				(wnp->RUNPOINT.y == EndNode.y))
				{
					wnp->RUNPOINT.x = -1;
					wnp->RUNPOINT.y = -1;
					wnp->STARTRUN = false;
					//wnp->DIRTY = false;
					not_finish=false;
				}else{
					//not_finish=true; // �٦� ���������u 
				}
				

				// �_�I
		if(SHOW_VALUE)
		{
			//6-7 �]�w�r������
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
			// debug �q�X�`�I���A
			StrPrintF(debugtxt,"%d",wnp->StartNodeP->NotReady);
				WinDrawChars(debugtxt,
					StrLen(debugtxt),
			 			StartNode.x ,
						StartNode.y -16);
	*/					


				//MYTimeTicks=TimGetTicks();
				//while((TimGetTicks()-MYTimeTicks)<(TicksPerSecend/100)){}
			
			

			}// if �u�q������
		}// if not dark line
	} // not show result
	else
	{	
				// 5-30 �[�J��ܼƭ�
				//if((wnp->RUNPOINT.x == StartNode.x)&&
				//(wnp->RUNPOINT.y == StartNode.y))
				//{
				//}
				/*
	// �p�⵴��y��
	StartNode.x=wnp->StartNodeP->TL.x +(wnp->StartNodeP->EXT.x/2)+ wnp->StartBlockP->bap->XY.x;
	StartNode.y=wnp->StartNodeP->TL.y +(wnp->StartNodeP->EXT.y/2)+ wnp->StartBlockP->bap->XY.y;
	if(wnp->StartBlockP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
	{
		bllp = wnp->StartBlockP->SelfBlockLLHead;
		while(bllp != SYSHOOK->BlockLLHeadP)
		{//4-26 ���@�M��C�A�[�X����y��
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
		{//4-26 ���@�M��C�A�[�X����y��
			EndNode.x += bllp->bap->XY.x;
			EndNode.y += bllp->bap->XY.y;
			bllp = bllp->SelfBlockLLHead;
		}
	}
	
	
	*/
				// ����I
				// 6-2 �ץ�
	if(wnp->DIRTY)// dirty ��ܸӽu�q�w�g����L
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
				{//4-26 ���@�M��C�A�[�X����y��
					EndNode.x += bllp->bap->XY.x;
					EndNode.y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
			}
			
			//6-7 �]�w�r������
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
			// debug �q�X�`�I���A
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
// �n�אּ�i���j�Φ�
// �p�J��y���c�znode�A�h���j�I�s���禡�e�X�C/
// 5-12 ���c�[�JReDraw �ܼ� �A�קK�����W�q�Ϫ��{�{
// �]�N�O���ӵe�A�e �]��������b�ӺC
// 5-12 �s�W�޼� ReDrawAll �j��e
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
		{//���
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
						// �M������
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
					{// ���ܹw�]���󪺦�m �۹��m
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.x=bllpH->bap->SIZE.x-15;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.y=bllpH->bap->SIZE.y-18 ;
//	DrawIcon(WHILELOOPLOOPBitmap,bllpH->bap->XY.x +bllpH->bap->SIZE.x+ window_x-10,
//	bllpH->bap->XY.y + bllpH->bap->SIZE.y-18 + window_y);

					}
					if(bllpH->bap->BITMAPID==FORLOOPBitmap)
					{// 5-30 �[�J for �j�骺�w�]����B�z
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.x=8 ;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->bap->XY.y=13 ;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->XY.x=8 ;
((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->XY.y=bllpH->bap->SIZE.y-17 ;
					}
					// 5-8 �B�z�ųs����IO�`�I
					ionode = 	bllpH->bap->IONodeLLHead;
					InSideHaveNoWire = true;
					OutSideHaveNoWire = true;
					while(ionode != NULL)
					{
					    // ��X���ݦ�C����J
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
						// �~�u�q��C
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								OutSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								OutSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// �� �`�N ��J��H
						wirep = ((HOOKP)bllpH->LoopBlockHOOKP)->WireLLHeadP;
						// ���u�q��C
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								InSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								InSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// �Y�� IO �`�I�S�����u�h������
						if((InSideHaveNoWire == true) && (OutSideHaveNoWire == true))
						{// ������ �u�q�ܼƭ��]
							bllpH->bap->IONodeLLHead = RemoveIONodeLinkList(
									bllpH->bap->IONodeLLHead,ionode);
							ionode = bllpH->bap->IONodeLLHead;
						}else{
							ionode = ionode->NEXT;
						}
					}
					
					// �e�XIO �`�I
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
					//���۳B�z��������(���j) // �קK�Ŧ�C
							DrawDiagram(((HOOKP)bllpH->LoopBlockHOOKP)->BlockLLHeadP,
									((HOOKP)bllpH->LoopBlockHOOKP)->WireLLHeadP,
									false,
									true,
									bllpH->bap->XY.x + window_x,
									bllpH->bap->XY.y + window_y,ReDrawAll);
						
				break;
		        case CASEBLOCK :
						// �M������
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
						

					// 5-8 �B�z�ųs����IO�`�I
					ionode = 	bllpH->bap->IONodeLLHead;
					InSideHaveNoWire = true;
					OutSideHaveNoWire = true;
					while(ionode != NULL)
					{
					    // ��X���ݦ�C����J
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
						// �~�u�q��C
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								OutSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								OutSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// �� �`�N ��J��H
						wirep = ((HOOKP)bllpH->CurrentHOOKP)->WireLLHeadP;
						// ���u�q��C
						while(wirep != NULL)
						{
							if(wirep->StartNodeP == ionode)
								InSideHaveNoWire = false;
							if(wirep->EndNodeP == ionode)
								InSideHaveNoWire = false;
							wirep = wirep->NEXTNODE;
						}
						// �Y�� IO �`�I�S�����u�h������
						if((InSideHaveNoWire == true) && (OutSideHaveNoWire == true))
						{// ������ �u�q�ܼƭ��]
							bllpH->bap->IONodeLLHead = RemoveIONodeLinkList(
									bllpH->bap->IONodeLLHead,ionode);
							ionode = bllpH->bap->IONodeLLHead;
						}else{
							ionode = ionode->NEXT;
						}
					}
					
					
						// �e�XIO �`�I
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
						//���۳B�z��������(���j)
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
				// �e�~��
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
// �M���~�ؽu	
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
				
			// DEBUG �q�Xself �H�� background blockllhead
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
		{//�M��
			
			if(bllpH->bap->Clean || ReDrawAll)
			{
				// �M���D�n�~�[
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
						// �M���~�ؽu
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
						// �M���~�ؽu
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
        		    	// �M�Ỽ�j
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
	
	// �e�X�ثe�u��ϥ�
	DrawIcon(BlockPointerState,140,0);
	// �e�X���
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
 �w��_ ���I�I�����P
 �ӻݭn�B�z
 ���X�ر��p
 1. �@�I�bSYSHOOK �@�I�b���c��
 2. �@�I�b���c�� �@�I�b���c��
 ���ޫ�˳��ݭn�B�z�_�����c�����D
 
 5-31 ���ϥΤ��@�ܼ� SYSHOOK ��H�P�_ TYPE != HOOKBLOCK
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
	
	// 5-18 �վ��Y������ ���l�׬���J
	if(WireStop.NodeP->TYPE==0)
	{
		tEP = WireStop;
		WireStop = WireStart;
		WireStart = tEP;
	}
	// 5-7 �p���Y�����O�_���`��
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
	// 5-7 �����@�_�I���`�ס@���@�̤p
	// ���O�٬O�L�k�ѨM  �󵲺c �����D
	// 5-18 ����
	/*
	if(DeepthStart > DeepthEnd)
	{
		tEP = WireStop;
		WireStop = WireStart;
		WireStart = tEP;
	}
	*/
	// 5-8 �P�_�󵲺c
	//if((DeepthStart != 0) && (DeepthEnd != 0))
	// if(WireStart.BlockP->SelfBlockLLHead)
	
	
	// 5-8 �M�w�s�`�I����m
	// ����X�u���m
	true_x_start=0;
	tBNP = WireStart.BlockP->SelfBlockLLHead;
//	while(tBNP != SYSHOOK->BlockLLHeadP)
	while(tBNP->TYPE != HOOKBLOCK)
	{//4-26 ���@�M��C�A�[�X����y��
		true_x_start += tBNP->bap->XY.x;
		tBNP = tBNP->SelfBlockLLHead;
	}
	true_x_start += WireStart.BlockP->bap->XY.x;
	true_x_stop=0;
	tBNP = WireStop.BlockP->SelfBlockLLHead;
//	while(tBNP != SYSHOOK->BlockLLHeadP)
	while(tBNP->TYPE != HOOKBLOCK)
	{//4-26 ���@�M��C�A�[�X����y��
		true_x_stop += tBNP->bap->XY.x;
		tBNP = tBNP->SelfBlockLLHead;
	}
	true_x_stop += WireStop.BlockP->bap->XY.x;
	
	// 5-8 �ثe�O�H�̹��m�ӧP�_���k��
	// �N�ӭn�H��X�J�ӧP�_���k��
	// 5-18 �H�_���`�קP�_���k�� �o�ɪ�start�@�w�O��ư_�I
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
	// WireStart �O�_�P WireStop �P�ݤ@��C�A�O�h���}
	
	// 5-18 ��������� 
	// 1.�p�G�_�I�̲` �h�q�_�I���X��
	// 2.�p�G���I�̲` �h�q���I�˨��X�Ө�_�I
	while(WireStart.BlockP->SelfBlockLLHead != WireStop.BlockP->SelfBlockLLHead)
	{
	// ��XWireStart���ݵ��c
	// 5-18 �Ѳ`�רM�w
		if(DeepthStart > DeepthEnd)
		{
			StructBlockP=WireStart.BlockP->SelfBlockLLHead;
		}
		else
		{
			StructBlockP=WireStop.BlockP->SelfBlockLLHead;
		}
		
	// 6-1 ��XStructBlock �� IO �`�I�̤jID��
	MaxID=0;
	ionode=StructBlockP->bap->IONodeLLHead;
	
	while(ionode!=NULL)
	{
		if((ionode->ID) > MaxID)
			MaxID=ionode->ID;
			
		ionode=ionode->NEXT;
	}
	MaxID++;
	
	// �s�WIO�`�I
	//if( == 1)
	//{//�@WireStop���`�I = ��J �[�J�`�I
	
		ionode = allocate_a_new_IOnode();
		ionode->ID = MaxID;
		// �p�G��ư_�I����` ��ܥL�b�̭� �h�sIO�I�ݩʬO...
		if(DeepthStart > DeepthEnd)
		{
			ionode->TYPE = 0;
		}
		else
		{
			ionode->TYPE = 1;
		}
		//ionode->TYPE = WireStop.NodeP->TYPE; // �Ӹ`�I�ݩ�
		ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
		ionode->BYTES=sizeof(int);
		ionode->NotReady=true;
		ionode->NUMS=1;
		// �b�����٬O�k��
		if(SIDE)
		{// ����
			ionode->TL.x=0;
		}else{ // �k��
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
	// �s�W�u�q
	tEP.BlockP=StructBlockP;
	tEP.NodeP=ionode;
	// 5-7 �M�w�ϥέ��@�� �u�q��C�Y
	// 5-7 �H�K�N�u�q��C�Y��s��s
	//if(IsTheSameWireLLHead(hookp,WireStop.BlockP))
	//{
		//                                      v start  v stop
	//	hookp = AddWireLinkListNode(hookp,WireStop,tEP);
	//}else{
		// �q WireStop �����󤤧�X��C�Y���O
		//5-18
	if(DeepthStart > DeepthEnd)
	{
		switch(WireStart.BlockP->SelfBlockLLHead->TYPE)
		{// �H�K���w��s���@�ӽu�q��C�Y
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
		{// �H�K���w��s���@�ӽu�q��C�Y
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
	
	
	// �N�����NWireStop�A�í��Ʋ�1�B�J
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
	
	
	// �s�W�u�q�s��WireStart WireStop �o�̬O�̫�@�q �u�q
	// 5-18
	//if(DeepthStart > DeepthEnd)
	//{
		switch(WireStart.BlockP->SelfBlockLLHead->TYPE)
		{// �H�K���w��s���@�ӽu�q��C�Y
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
// �q CheckWireLink �����X��
//
void StartStopAddIntoWLL(void )
{
	EndPoint tPoint;

						if((WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP->SelfBlockLLHead)
						|| (WireStart.BlockP == WireStop.BlockP->SelfBlockLLHead) ||
						(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP)) 
						// 5-8 �[�W�P�_���c������P���c IOnode �s��
						{// ��_�B���I�����ݦ�C�Y�ۦP��
						    // ��X���ݦ�C����J
						    // 5-8 ��������s�ʧ@�A�_�h������J��wirellhead�|���s
						    // 5-8 �o�̭n�M�w�ϥν֪���J
						    // �p�G�O�ۦP���ݫh�O�P�@�Ӫ�J
						    // �p�G���P �h�n��ܥ��T������
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
							// �i�J�󵲺c���u�B�z�禡
							// �p�G�u�q�Y�P�u�q�����ݤ��P��C �h
							//   �n�ݭn���q
							// �o�ǻݭn���q���u�q
							// �|�]���b���P�����c�� �ӻݭn���J���P����q
							// 5-8 �o���٦��@�ӷN�q
							// ���c�������� �P ���c IOnode �s���ɤ]�|�i�ӳo��
							// �n���}��@��s���B�z(�W���P�_���ק�)
							// 5-8 CrossWire() �אּ�Ѥ����M�w
							// �ݭn��s��wirellhead����
							CrossWire();
						}
}
// ******************************************
// * �ˬd�O�_�i�H�إ߳s�u
// * �p�G�O��1���i�ӡA�|�O��}�Y
// * 
// *
// *
// * 2003-4-22 �Ӧ�CheckOnNode�����W�ߥX��
// *
// ******************************************
// i�N�O�ĴX���I �H0����(�w�g��1�F)
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
				{ //���O�P�@�I
					WireStop.BlockP = InAreaInf.llp;
					WireStop.NodeP = NodeP;
				}
				
				if((WireStart.NodeP != NULL)&&(WireStop.NodeP != NULL))
				{// ���u�إߡA�ˬd���u
					if((WireStart.NodeP->TYPE == false) &&
				 	(WireStop.NodeP->TYPE == false)
				  	)
					{// ��X���i���b�@�_
						WireStart.BlockP = NULL;
						WireStop.BlockP = NULL;
					}
				}
				
				if((WireStop.NodeP != NULL) 
					&& (WireStart.NodeP != NULL)
					&& (WireStart.BlockP != WireStop.BlockP)
					)
				{// �s�u�إ� �B�z
					// THREAD or SCISSOR ?
					if(BlockPointerState == THREADBitmap)
					{
						StartStopAddIntoWLL();
						// 5-31 �W�ߥX�h
						/*
						if((WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP->SelfBlockLLHead)
						|| (WireStart.BlockP == WireStop.BlockP->SelfBlockLLHead) ||
						(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP)) 
						// 5-8 �[�W�P�_���c������P���c IOnode �s��
						{// ��_�B���I�����ݦ�C�Y�ۦP��
						    // ��X���ݦ�C����J
						    // 5-8 ��������s�ʧ@�A�_�h������J��wirellhead�|���s
						    // 5-8 �o�̭n�M�w�ϥν֪���J
						    // �p�G�O�ۦP���ݫh�O�P�@�Ӫ�J
						    // �p�G���P �h�n��ܥ��T������
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
							// �i�J�󵲺c���u�B�z�禡
							// �p�G�u�q�Y�P�u�q�����ݤ��P��C �h
							//   �n�ݭn���q
							// �o�ǻݭn���q���u�q
							// �|�]���b���P�����c�� �ӻݭn���J���P����q
							// 5-8 �o���٦��@�ӷN�q
							// ���c�������� �P ���c IOnode �s���ɤ]�|�i�ӳo��
							// �n���}��@��s���B�z(�W���P�_���ק�)
							// 5-8 CrossWire() �אּ�Ѥ����M�w
							// �ݭn��s��wirellhead����
							CrossWire();
						}
						
						*/
					}
					else if(BlockPointerState == SCISSORBitmap)
					{
					/*
					  if(WireStart.BlockP->SelfBlockLLHead == WireStop.BlockP->SelfBlockLLHead)
					  {// ��_�B���I�����ݦ�C�Y�ۦP��
					   if(hook->WireLLHeadP != NULL)
						if((WLLP = InWireLinkList(hook,WireStart,WireStop)) != NULL)
						{
							hook->WireLLHeadP = RemoveWireLinkList(hook->WireLLHeadP,WLLP); // ��XLL�����u�A������
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
// ���F�y���c�z�A�n�אּ�i���j���c
//
//
// �i���j 4/13
//
// blockptr �O�n����������
//
// 5-8 �����u�q
// �אּ�� blockptr ������ selfblockllhead ��X HOOK �M�� �H��wirellhead���Y
void BreakWireConnection(BNP  blockptr)
{
	Boolean exist=false;
	WNP WireLLHead,wnp;
	HOOKP thook;
	
	// 5-8 ��X hook
    // ��X���ݦ�C����J
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
    // Ū�X
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
	// ��s
    // ��X���ݦ�C����J
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
	�\�� �ಾ����
	5-7 �ಾ����אּ�ק�e��`�I
	�쥻�O�s�W->�[�J �אּ
	�u�O�q�o���쵲 ��� �t�@���쵲 �ӭ쥻���쵲���a��ɰ_��
	�p�G�쥻���쵲�u�Ѥ@�� �N�n�`�N �쥻���쵲�Y�n��s
	
	�]���~���]�p�u�n��쪺����N�N������쵲���̫᭱
	�ҥH�o�̦p�G���|�����X�쵲�Ĥ@�Ӫ�����
	�ҥH���ݭn��s�쵲�Y 
	�u���b����Ѥ@�Ӫ��ɭԤ~�ݭn�C
*/
static void ChangeLinkList(UInt16 now_x,UInt16 now_y)
{
	//InAreaInfType InAreaInf;
	BNP bllpH;//,insertBlock;
	BNP bllp;
	UInt16 x,y;
	//
	// 4-24 �s��k�A�d�߫��Хثe���I����C�Y
	//
		if(HandPointer.CatchWhat->BackgroundBlockLLHead != 
				HandPointer.CatchWhat->SelfBlockLLHead)
		{// ���I�P�_�I���ݤ��P��C�A�ݭn�h��
				  // �h���e�A�Ѱ��s�u
			BreakWireConnection(HandPointer.CatchWhat);
				  				
			// �o�̭n���g�A�w��_���j��ݭn
			if(HandPointer.CatchWhat->SelfBlockLLHead == SYSHOOK->BlockLLHeadP)
			{// ************�q �D��C �ಾ�� ���c��C
				
				
					// �n��X�u�����۹��m
				x=0;y=0;
				bllp = HandPointer.CatchWhat->BackgroundBlockLLHead;
				
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 ���@�M��C�A�[�X����y��
					x += bllp->bap->XY.x;
					y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
				// 5-7 ��
				HandPointer.CatchWhat->bap->XY.x = now_x - HandPointer.RelativePoint.x - x;
				HandPointer.CatchWhat->bap->XY.y = now_y - HandPointer.RelativePoint.y - y;
				HandPointer.CatchWhat->SelfBlockLLHead=HandPointer.CatchWhat->BackgroundBlockLLHead;
				HandPointer.CatchWhat->NEXTNODE=NULL;
				// 5-7 �N���C�׸ɰ_��
				if(HandPointer.CatchWhat->PREVNODE != NULL)// �o�Ӧ�C�`�I�S���Y
					HandPointer.CatchWhat->PREVNODE->NEXTNODE = HandPointer.CatchWhat->NEXTNODE;
				if(HandPointer.CatchWhat->NEXTNODE != NULL)// �o�Ӧ�C�`�I�S����
					HandPointer.CatchWhat->NEXTNODE->PREVNODE = HandPointer.CatchWhat->PREVNODE;
				/*
				// �t�Φ�C�ܤ��٦��@�Ӥ���s�b�A���|�����Ŧ�C
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// �o�O�̫�@���쵲�`�I
					HandPointer.CatchWhat->SelfBlockLLHead->
				}
				*/


					// �[�J�y���c�zbllpH �O�ثe���c
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
				
				
			}else{// �q ���c��C �ಾ�� �D��C �άO ���c��C �� �ಾ
				if(HandPointer.CatchWhat->BackgroundBlockLLHead == SYSHOOK->BlockLLHeadP)
				{// ********* �q ���c��C �ಾ�� �D��C
					
				// 5-7 �N���C�׸ɰ_��
				if(HandPointer.CatchWhat->PREVNODE != NULL)// �o�Ӧ�C�`�I�S���Y
					HandPointer.CatchWhat->PREVNODE->NEXTNODE = HandPointer.CatchWhat->NEXTNODE;
				if(HandPointer.CatchWhat->NEXTNODE != NULL)// �o�Ӧ�C�`�I�S����
					HandPointer.CatchWhat->NEXTNODE->PREVNODE = HandPointer.CatchWhat->PREVNODE;
				bllpH = HandPointer.CatchWhat->SelfBlockLLHead;
				switch(bllpH->TYPE)
				{
				case LOOPBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// �o�O�̫�@���쵲�`�I
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->LoopBlockHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				case CASEBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// �o�O�̫�@���쵲�`�I
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->CurrentHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				}
				// 5-7 ���ܦ�m �� ���ݦ�C
				HandPointer.CatchWhat->bap->XY.x = now_x - HandPointer.RelativePoint.x;
				HandPointer.CatchWhat->bap->XY.y = now_y - HandPointer.RelativePoint.y;
				HandPointer.CatchWhat->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
				HandPointer.CatchWhat->NEXTNODE=NULL;

					// �^��D��C
				SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,HandPointer.CatchWhat);
					// �q����ݦ�C������
			//		bllpH->BlockLLHeadP = RemoveBlockLinkList(bllpH->BlockLLHeadP,HandPointer.CatchWhat);//deleteBlockH
				}else{//  *********** ���c��C �� �ಾ
					// �n��X�u�����۹��m
					x=0;y=0;
					bllp = HandPointer.CatchWhat->BackgroundBlockLLHead;
					while(bllp != SYSHOOK->BlockLLHeadP)
					{//4-26 ���@�M��C�A�[�X����y��
						x += bllp->bap->XY.x;
						y += bllp->bap->XY.y;
						bllp = bllp->SelfBlockLLHead;
					}
					
				// 5-7 �N���C�׸ɰ_��
				if(HandPointer.CatchWhat->PREVNODE != NULL)// �o�Ӧ�C�`�I�S���Y
					HandPointer.CatchWhat->PREVNODE->NEXTNODE = HandPointer.CatchWhat->NEXTNODE;
				if(HandPointer.CatchWhat->NEXTNODE != NULL)// �o�Ӧ�C�`�I�S����
					HandPointer.CatchWhat->NEXTNODE->PREVNODE = HandPointer.CatchWhat->PREVNODE;
				bllpH = HandPointer.CatchWhat->SelfBlockLLHead;
				switch(bllpH->TYPE)
				{
				case LOOPBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// �o�O�̫�@���쵲�`�I
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->LoopBlockHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				case CASEBLOCK :
				if((HandPointer.CatchWhat->PREVNODE==NULL)&&(HandPointer.CatchWhat->NEXTNODE==NULL))
				{// �o�O�̫�@���쵲�`�I
					((HOOKP)HandPointer.CatchWhat->SelfBlockLLHead->CurrentHOOKP)->
						BlockLLHeadP=NULL;
				}
				break;
				}
				// 5-7 ���ܦ�m �� ���ݦ�C
				HandPointer.CatchWhat->bap->XY.x = now_x - HandPointer.RelativePoint.x - x;
				HandPointer.CatchWhat->bap->XY.y = now_y - HandPointer.RelativePoint.y - y;
				HandPointer.CatchWhat->SelfBlockLLHead=HandPointer.CatchWhat->BackgroundBlockLLHead;
				HandPointer.CatchWhat->NEXTNODE=NULL;

					// �i�J�s��C
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
	
	// �O�_���b�u��ϥܡA�Y�O�h�i�Jtools form
	if(CheckBoundary(140,0,18,18,eventP->screenX,eventP->screenY))
	{
		In =true;
		FrmPopupForm(BlockToolsForm);
	}else if(CheckBoundary(120,0,16,16,eventP->screenX,eventP->screenY)){
		In =true;
	// �O�_���b����ϥܡA�Y�O�h�i�Jfunctions form
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
			// �M������X��
			CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
			// �u�q ���A���m
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
			// �M������X��
			CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
			// �u�q ���A���m
			ResetWireStatus(SYSHOOK->BlockLLHeadP->NEXTNODE,
			SYSHOOK->WireLLHeadP);
			FrmGotoForm(BlockDiagramForm);
		}
	}else if(CheckBoundary(0,0,33,15,eventP->screenX,eventP->screenY)){
		In =true;
	// �O�_���b���D�A�Y�O�h�ഫ form
		FrmGotoForm(FrontPanelForm);
	}
	
	// 6-2 �p�G���b�D�@�ΰ� �h���n����ʧ@
	
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
	
	// 5-9 �ק���ɧP�_ now_x �P now_y ���]�w
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
			// 6-2 ���椤���i�s��
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}
		// 5-6 �[�J�\�� �Z���b�d�򤺪����󲾨��C�̫᭱ 
		// �o�˷|�ϱo��ܦb�̤W�h (�o��̤jZ��)
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); //LLP will set
			// 5-30 ���} �w�]����
			if((InAreaInf.InArea)&&(InAreaInf.llp->TYPE != CTRLBLOCK))
			{
				// �����C�̫᭱
				NestItemMoveToLastPosition(InAreaInf.llp);
				// in boundary
				switch(InAreaInf.llp->SelfBlockLLHead->TYPE)
				{
					case LOOPBLOCK :
		        	case CASEBLOCK :
						DrawIcon(CLENCHBitmap,now_x+offset_x,now_y+offset_y);
						HandPointer.HandState = CLENCHBitmap;
						HandPointer.CatchWhat = InAreaInf.llp; // point now bap->
				// �ƻs�� clone ��
					//HandPointer.Clone->TYPE=InAreaInf.llp->TYPE;
					//HandPointer.Clone->bap=InAreaInf.llp->bap;
					//HandPointer.Clone->LoopBlockHOOKP=InAreaInf.llp->LoopBlockHOOKP;
					//HandPointer.Clone->CurrentHOOKP=InAreaInf.llp->CurrentHOOKP;
					//HandPointer.Clone->CaseBlockHOOKLLHeadP=InAreaInf.llp->CaseBlockHOOKLLHeadP;
					//HandPointer.Clone->SelfBlockLLHead=InAreaInf.llp->SelfBlockLLHead;
					//HandPointer.Clone->BackgroundBlockLLHead=InAreaInf.llp->BackgroundBlockLLHead;
					//HandPointer.Clone->bap->ReDraw=true;
						// �۹��m�n����

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
						// �۹��m�n����
						
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
				// 4-23 ����������ݦ�C�Y�A�H�Ƥ����ಾ����
				// 4-24 �R�����ܼơA�אּBlock�����ۤv�O���Y��m
			}// if inarea
			else
			{
				DrawIcon(HANDBitmap,now_x+offset_x,now_y+offset_y);
				HandPointer.HandState = HANDBitmap;
			}
		break;
		case THREADBitmap:
			// 6-2 ���椤���i�s��
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
			// 6-2 ���椤���i�s��
			if(RUN!=0)
			{
				RUN=0;
				STOP=0;
				RUNFOREVER=false;
			}
			InAreaInf = IsInArea(InAreaInf,SYSHOOK->BlockLLHeadP->NEXTNODE,now_x,now_y,0,0); 
			if(InAreaInf.InArea)
			{
				// �����C�̫᭱
				NestItemMoveToLastPosition(InAreaInf.llp);

			//if(InAreaInf.llp->bap->BITMAPID == WHILELOOPBitmap)
				switch(InAreaInf.llp->TYPE)
				{//���� wHileloop
				 case LOOPBLOCK :
					if(((ctrlnode = CheckOnCTRLNode(InAreaInf,now_x,now_y)) != NULL )&& 
					       (ctrlnode->ID == DRAGNODE))
					{// ������ drag point
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
			// �e ICON
			}
		break;
		case SCISSORBitmap:
			// 6-2 ���椤���i�s��
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
			// 6-2 ���椤���i�s��
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
				// 5-30 �o�����Ӧb�f�V�B�z �]����������̭����ʺA���� �ݤ@�ӭӲ���
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
	
	// 5-9 �ק���ɧP�_ now_x �P now_y ���]�w
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
				// 5-12 ����ø(�M��)
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
				// �����b�o�̵o��
				}
			}
			*/	
				/*
				// 5-12 �_����ø
				llp = HandPointer.CatchWhat->SelfBlockLLHead;
				while(llp != SYSHOOK->BlockLLHeadP)
				{
					llp->bap->ReDraw=true;
					llp=llp->SelfBlockLLHead;
				}
				llp->bap->ReDraw=true;
				*/
		 // clear diagram
		 // 5-13 �o�@���令 �u�n�O�W�@�����X�{ ReDraw ������ �o�̴N�⥦�M��
		 // ���O�y���u�n����i�h�X�� �ӭI���N�|����
		DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,false,0,0,false);
		// 
		
		// clear prev. pointer icon
		DrawIcon(BLANKBitmap,prev_x+offset_x,prev_y+offset_y);
				// �]�w�s�y�Э�
				setNewObjXY(now_x-HandPointer.RelativePoint.x,now_y-HandPointer.RelativePoint.y);
				// �ƻs�� clone ��
				// 5-12 ���F������h���� �󵲺c���ٯॿ�`���
				// ���O�ثe�����D �L�k�p��ù���m
				// 5-12 �o��
					//HandPointer.Clone->TYPE=HandPointer.CatchWhat->TYPE;
					//HandPointer.Clone->bap=HandPointer.CatchWhat->bap;
					//HandPointer.Clone->LoopBlockHOOKP=HandPointer.CatchWhat->LoopBlockHOOKP;
					//HandPointer.Clone->CurrentHOOKP=HandPointer.CatchWhat->CurrentHOOKP;
					//HandPointer.Clone->CaseBlockHOOKLLHeadP=HandPointer.CatchWhat->CaseBlockHOOKLLHeadP;
					//HandPointer.Clone->SelfBlockLLHead=HandPointer.CatchWhat->SelfBlockLLHead;
					//HandPointer.Clone->BackgroundBlockLLHead=HandPointer.CatchWhat->BackgroundBlockLLHead;
					//HandPointer.Clone->bap->ReDraw=true;

				// in boundary
				
				// 5-12 ����ø
				HandPointer.CatchWhat->bap->ReDraw = true;
				HandPointer.CatchWhat->bap->Clean = true;

				llp = SYSHOOK->BlockLLHeadP->NEXTNODE;
				do
				{
					InAreaInf.InArea = false;
					InAreaInf = IsInArea(InAreaInf,llp,now_x,now_y,0,0); //LLP will set
					
					if(InAreaInf.InArea)
					{// ��츨�b����W�A���γ\�O�⤤���Ӥ���A�]�������L
						llp = InAreaInf.llp->NEXTNODE;
					}
					
					
				}while((InAreaInf.InArea == true) &&
				(HandPointer.CatchWhat == InAreaInf.llp));//�קK�O�⤤���Ӥ���
				
				if(InAreaInf.InArea)
				{

					switch(InAreaInf.llp->TYPE)
					{
						case LOOPBLOCK :
						// 5-12 �_����ø �V�U���M ���������M
						NestReDraw(((HOOKP)InAreaInf.llp->LoopBlockHOOKP)->BlockLLHeadP);

						// 5-12 �_����ø �V�U���M ���ӳB���M
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
						NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
				}
			        		//���I�b�s�����c���A�e�~��
			        		InAreaInf.llp->bap->DrawBorder = true;
							// �ק�background
							HandPointer.CatchWhat->BackgroundBlockLLHead = InAreaInf.llp;
							//BackgroundBlockLLHead;
						break;
		        		case CASEBLOCK :
						// 5-12 �_����ø �V�U���M ���������M
						NestReDraw(((HOOKP)InAreaInf.llp->CurrentHOOKP)->BlockLLHeadP);
						
						// 5-12 �_����ø �V�U���M ���ӳB���M
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
						NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
				}
			        		//���I�b�s�����c���A�e�~��
			        		InAreaInf.llp->bap->DrawBorder = true;
							// �ק�background
							HandPointer.CatchWhat->BackgroundBlockLLHead = InAreaInf.llp;
							//BackgroundBlockLLHead;
						break;
						default:
							// ���O���b���c��
							// �ק�background
							//HandPointer.CatchWhat->BackgroundBlockLLHead = SYSHOOK->BlockLLHeadP;
						break;
					}
				}else{
					// �S�����b����W
					// �ק�background
					HandPointer.CatchWhat->BackgroundBlockLLHead = SYSHOOK->BlockLLHeadP;
					
			// 5-12 �_����ø �V�U���M
			if(HandPointer.CatchWhat->SelfBlockLLHead == HandPointer.CatchWhat->BackgroundBlockLLHead)
			{
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
			}
			else
			{// �������}���ݭI��
				if(HandPointer.CatchWhat->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
				NestReDraw(HandPointer.CatchWhat->SelfBlockLLHead);
				if(HandPointer.CatchWhat->BackgroundBlockLLHead != SYSHOOK->BlockLLHeadP)
				{
				NestReDraw(HandPointer.CatchWhat->BackgroundBlockLLHead);
				// �����b�o�̵o��
				}
			}
			
			
				}

				//Draw Diagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,1,0,0,true); 

			// 5-13 �]�wESCout �ܼ�
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
			{// �������}���ݭI��
				if(ESCout != HandPointer.CatchWhat->BackgroundBlockLLHead)
				{// ����@�����}���ݭI���@���P�W�@�����d�I�����P�@�����O�W���I����ø
					if(ESCout != NULL)
					{
						ESCout->bap->ReDraw = true;
						ESCout->bap->Clean = false;
					}	
					ESCout = HandPointer.CatchWhat->BackgroundBlockLLHead;
					//NestReDraw(ESCout);
				}
			}
			
			// 5-12 �_����ø �V�U���M �@���ҧ쥻���O���c �����S�B��C�̫�@��
			NestReDraw(HandPointer.CatchWhat);
			
	DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,true,0,0,false); // redraw diagram
	// �u�e�o�Ӥ���
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
				//DragStatus.llp->bap->Clean=true;// ���F�M���o�Ӥ���
				/*
				// 5-12 �_����ø
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
		// ���ӧאּ�_���B�z
		// 5-13 �אּ�_���B�z
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
					
				// 5-13 �̤j����
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
				// 5-12 �_����ø �V�U���M 5-13 ��
				if(DragStatus.llp->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
					NestReDraw(DragStatus.llp->SelfBlockLLHead);
				// 5-13 ����
				Dragllp=DragStatus.llp->SelfBlockLLHead;
				while(Dragllp != SYSHOOK->BlockLLHeadP)
				{
					Dragllp->bap->ReDraw=true;
					Dragllp->bap->Clean=true;
					Dragllp=Dragllp->SelfBlockLLHead;
				}
				// 5-18 �ק��X�`�I��m
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
			// �e ICON
			}
		/*
		DragStatus.llp->bap->ReDraw=true;// ���F�M���o�Ӥ���
		// 5-12 �_����ø
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

	
	// 5-9 �ק���ɧP�_ now_x �P now_y ���]�w
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
				// 4-23 �p�e�G����}�A�ˬd...
				// 1. �_�I�Ҧb����C�y�Y�z�P���I�Ҧb��C�Y �O�_���P�H
				// 2. ���P�G��ܻݭn�ಾ����Ҧb��C
				// 3. �P�G��ܥu�O���󲾰�
				// 5-13
				// 4. ���u�����C�̫᭱
				//
				// ChangeLinkList()
				// 4-24 ����
				HandPointer.CatchWhat->bap->Clean=true;//5-12
				DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,false,0,0,0,false); // erase diagram
				ChangeLinkList(now_x,now_y);
				// �_�� �����C�̫᭱
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
			{	// �h�ӱ����I
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
			    		// ���Χ���
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
			// �M�� ICON
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
			
			
			//if(eventP->screenY > 15) //  5-9 ���Ftools palette ��������
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

		// ���� block ��formload �ƥ�i�� �~�i�����ʧ@ �H�K
		// block form �i�H��ܰʵe
//		if((event.eType == frmLoadEvent)&&(event.data.frmLoad.formID ==BlockDiagramForm))
//			RunLock=true;
		
		// 2�ӱ��p �i�H����
		// 1. �O���} �� panel �U�F���� �o�ɷ|�੹ block �B ��~����
		// 2. �O�S�} �� panel �U�F����
		//if((RUN==1 && RunLock)||(RUN==1 && LAMP==false))
		if(RUN==1)
		{
			DrawIcon(RUNBitmap,60,0);
			
			if(DoRun(SYSHOOK->BlockLLHeadP->NEXTNODE,SYSHOOK->WireLLHeadP))
			{
			}else{
				if(RUNFOREVER)
				{
					// �M������X��
					CheckConnection(SYSHOOK->BlockLLHeadP->NEXTNODE,false);
					// �u�q ���A���m
					ResetWireStatus(SYSHOOK->BlockLLHeadP->NEXTNODE,
					SYSHOOK->WireLLHeadP);

				}else
				{
					RUN=0;
					STOP=0;
				
				DrawIcon(RUNWAITBitmap,60,0);
				// 6-2 �M���e��
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
				{// �Ǧ^ true ��� ���槹�@�� wire �H�� block run
				 // ���٦�������
				}else{// �Ǧ^ false ��ܾ������@������
					RUN=0;
					STOP=0;
					RunLock=false;
					DrawIcon(RUNWAITBitmap,60,0);
				}
			}
			*/
		}
		


				// �u�ʵe	
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
