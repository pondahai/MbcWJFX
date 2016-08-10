#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"

#include "vpl.h"


BNP KeyboardObject=NULL; // 6-8


//extern void FunctionspenUpProcess(EventPtr eventP);
//extern void FunctionspenDownProcess(EventPtr eventP);
//extern void FunctionspenMoveProcess(EventPtr eventP);
//extern Boolean FunctionsFormHandleEvent(EventPtr eventP);
//extern void    OpenDB(char *DBname);
//extern void    CloseDB(void);
extern WNP allocate_a_new_wire_node();
extern WNP InsertIntoWireLL(WNP ,WNP node);
extern BNP allocate_a_new_node();
//extern BlockLink InsertIntoBlockLL(BlockLink,BlockLink node,Boolean generate_new_ID);
extern BNP InsertIntoBlockLLNoUID(BNP,BNP node);
//extern FUNCTIONSLLDATAtype InsertIntoFunctionsLL(FUNCTIONSLLDATAtype Fdata,BNP node );
extern FUNCTIONSLLDATAtype SetFunctionsLL(FUNCTIONSLLDATAtype  Fdata);
//extern int ConvertFileName2RecordIndex(char *filename);
extern BNP RemoveBlockLinkList(BNP,BNP  blockptr);
extern WNP RemoveWireLinkList(WNP,WNP  wireptr);
extern HOOKP InsertHOOKNode(HOOKP sysHead);
//extern HOOKP NEW(HOOKP sysHead);
extern WNP InWireLinkList(HOOKP ,EndPoint start,EndPoint end);
extern HOOKP AddWireLinkListNode(HOOKP ,EndPoint,EndPoint);
extern IONODEP allocate_a_new_IOnode();
extern IONODEP InsertIntoIONodesLL(IONODEP IONodeH,IONODEP node );
extern BNP ItemMoveToLastPosition(BNP BLLHead,BNP bnp);

//void DoRun(BlockLink BlockLLHeadP,WireLink WireLLHeadP);


extern numRecords;
extern db;
extern EndPoint WireStop;
extern EndPoint WireStart;
extern HOOKP SYSHOOK;
extern BlockPointerState;
extern  struct hand HandPointer;
extern  VoidHand 		ChoicesHandle , ChoicesPtrsHandle;

/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
 /*
extern BNP FindItemByID(BNP BLHead, UInt16 ID)
{
	//BlockLink BL;
	//BL = SYSHOOK->BlockLLHeadP;
		while(BLHead != NULL)
		{
			if(BLHead->ID == ID)
				break;
			BLHead = BLHead->NEXTNODE;
		}
	return BLHead;
}
*/

/***********************************************************************
 * 2003-4-12
 * �i�Ӫ��@�w�OCustom block
 * 
 ***********************************************************************/
void AutoFindIOnodesSetIntoICONnode(BNP LLHead)
{
}


/*
*/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
void DrawIcon(UInt16 BitMapID,UInt16 x,UInt16 y)
{
	Handle resH;
	BitmapType *bitmap;
	
	if(BitMapID)
	{
		resH =	DmGetResource (bitmapRsc, BitMapID);
		bitmap = MemHandleLock (resH);
		WinDrawBitmap(bitmap, x,y);
		MemHandleUnlock(resH);
		DmReleaseResource( resH);
	}
	

}
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
extern Boolean CheckBoundary(int x,int y,int XX,int YY,int X,int Y)
{
    //    <---XX--->
    //  (x,y)
    //    ********* ^
    //    * (X,Y) * |
    //    *   *   * YY
    //    *       * |
    //    ********* v
    //
	if((X > x && X < (x+XX)) && (Y > y && Y < (y+YY)))
		return true;
	else
		return false;
}
/*
	�O�_�ݦP�@�ӽu�q��C
*/
Boolean IsTheSameWireLLHead(HOOKP hookp,BNP bnp)
{
	HOOKP thookp;
	// ��X bnp �u�q�Y
	switch(bnp->SelfBlockLLHead->TYPE)
	{
		case HOOKBLOCK :
			thookp=((HOOKP)bnp->SelfBlockLLHead->hookP);
		break;
		case LOOPBLOCK :
			thookp=((HOOKP)bnp->SelfBlockLLHead->LoopBlockHOOKP);
		break;
		case CASEBLOCK :
			thookp=((HOOKP)bnp->SelfBlockLLHead->CurrentHOOKP);
		break;
	}
	if(thookp == hookp)
		return true;
	else
		return false;
}
CTRLNODEP CheckOnCTRLNode(InAreaInfType InAreaInf,int now_x,int now_y)
{
	Boolean OnNode = false;
	CTRLNODEP ctrlnode=NULL;
	UInt16 window_x=0,window_y=0;
	BNP bllp;
	RectangleType Rtest;

	switch(FrmGetActiveFormID())
	{
		case BlockDiagramForm:
			// �p�⵴��y��
			if(InAreaInf.llp->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
			{
				window_x=0;window_y=0;
				bllp = InAreaInf.llp->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 ���@�M��C�A�[�X����y��
					window_x += bllp->bap->XY.x;
					window_y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
			
				//window_x = InAreaInf.llp->SelfBlockLLHead->bap->XY.x;
				//window_y = InAreaInf.llp->SelfBlockLLHead->bap->XY.y;
			}
			// �O�_�b����`�I�W
			ctrlnode = InAreaInf.llp->bap->CTRLNodeLLHead;
			while((ctrlnode != NULL ) )
			{
				OnNode = CheckBoundary(
					ctrlnode->TL.x + 
					InAreaInf.llp->bap->XY.x + window_x,
					ctrlnode->TL.y + 
					InAreaInf.llp->bap->XY.y + window_y,
					ctrlnode->EXT.x ,
					ctrlnode->EXT.y ,now_x,now_y);
				if(OnNode)
					break;
						
				ctrlnode = ctrlnode->NEXT;
			}
			// �p�G�O�e�X�¤��
			if(OnNode)
			{
				Rtest.topLeft.x=ctrlnode->TL.x + 
				InAreaInf.llp->bap->XY.x + window_x;
				Rtest.topLeft.y=ctrlnode->TL.y + 
				InAreaInf.llp->bap->XY.y + window_y;
				Rtest.extent.x=ctrlnode->EXT.x;
				Rtest.extent.y=ctrlnode->EXT.y;

				WinDrawRectangle(&Rtest,0);
			}
		break;
		case FrontPanelForm:
			// �O�_�b����`�I�W
			ctrlnode = InAreaInf.llp->pap->CTRLNodeLLHead;
			while((ctrlnode != NULL ) )
			{
				OnNode = CheckBoundary(
					ctrlnode->TL.x + InAreaInf.llp->pap->XY.x ,
					ctrlnode->TL.y + InAreaInf.llp->pap->XY.y ,
					ctrlnode->EXT.x ,
					ctrlnode->EXT.y ,now_x,now_y);
				if(OnNode)
					break;
						
				ctrlnode = ctrlnode->NEXT;
			}
			// �p�G�O�e�X�¤��
			if(OnNode)
			{
				Rtest.topLeft.x=ctrlnode->TL.x + InAreaInf.llp->pap->XY.x ;
				Rtest.topLeft.y=ctrlnode->TL.y + InAreaInf.llp->pap->XY.y ;
				Rtest.extent.x=ctrlnode->EXT.x;
				Rtest.extent.y=ctrlnode->EXT.y;

				WinDrawRectangle(&Rtest,0);
			}
		break;		
	}
	
	return ctrlnode;
}
//
// i is point which bap->
// LLP
//
/***********************************************************************
 * �ˬd���S���bIO�`�I�W
 * �Ǧ^�G�ĴX��IO�`�I�H
 * 2003-4-12 �ثe�٬O�ϥΤ��@�ܼ�(LLP)�ǻ�
 *           ���t���u�q�s�����\��
 * 2003-4-13 �אּ�i���j�Φ�
 * 4-22      ���������ˬd���u�A�W�߬��~���Ƶ{��CheckWireLink
 * 4-24      �[�J�i�P�_���c�����󤧱��u
 *
 ***********************************************************************/
 IONODEP CheckOnNode(InAreaInfType InAreaInf,int now_x,int now_y,Boolean Draw)
{
	//int i;
	Boolean OnNode = false;
	RectangleType Rtest;
	BNP bllp;
	IONODEP ionode;
	UInt16 window_x=0,window_y=0;
	
	if(InAreaInf.llp != NULL)
	switch(FrmGetActiveFormID())
	{
		case BlockDiagramForm:
			// 4-24 �����c�ӥ[
			if(InAreaInf.llp->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
			{
				window_x=0;window_y=0;
				bllp = InAreaInf.llp->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 ���@�M��C�A�[�X����y��
					window_x += bllp->bap->XY.x;
					window_y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
			
				//window_x = InAreaInf.llp->SelfBlockLLHead->bap->XY.x;
				//window_y = InAreaInf.llp->SelfBlockLLHead->bap->XY.y;
			}
			//if(Items[0].UID != NULL) // data was loaded
				ionode = InAreaInf.llp->bap->IONodeLLHead;
				while((ionode != NULL ) )
				{
					OnNode = CheckBoundary(
						ionode->TL.x + 
						InAreaInf.llp->bap->XY.x + window_x,
						ionode->TL.y + 
						InAreaInf.llp->bap->XY.y + window_y,
						ionode->EXT.x ,
						ionode->EXT.y ,now_x,now_y);
					if(OnNode)
						break;
						
					ionode = ionode->NEXT;
				}
				/*
				for(i=0;i<InAreaInf.llp->bap->IONodeN && !OnNode;i++)
				{	
					OnNode = CheckBoundary(
						InAreaInf.llp->bap->IONodes[i].PieceRelativeTopLeft.x + 
						InAreaInf.llp->bap->XY.x + window_x,
						InAreaInf.llp->bap->IONodes[i].PieceRelativeTopLeft.y + 
						InAreaInf.llp->bap->XY.y + window_y,
						InAreaInf.llp->bap->IONodes[i].PieceExtent.x ,
						InAreaInf.llp->bap->IONodes[i].PieceExtent.y ,now_x,now_y);
				}
				*/
			// draw black block
			// if OnNode the (i-1) is the node.
			
			if(OnNode)
			{
				Rtest.topLeft.x=ionode->TL.x + 
				InAreaInf.llp->bap->XY.x + window_x;
				Rtest.topLeft.y=ionode->TL.y + 
				InAreaInf.llp->bap->XY.y + window_y;
				Rtest.extent.x=ionode->EXT.x;
				Rtest.extent.y=ionode->EXT.y;
				/*
				Rtest.topLeft.x=InAreaInf.llp->bap->IONodes[i-1].PieceRelativeTopLeft.x + 
				InAreaInf.llp->bap->XY.x + window_x;
				Rtest.topLeft.y=InAreaInf.llp->bap->IONodes[i-1].PieceRelativeTopLeft.y + 
				InAreaInf.llp->bap->XY.y + window_y;
				Rtest.extent.x=InAreaInf.llp->bap->IONodes[i-1].PieceExtent.x;
				Rtest.extent.y=InAreaInf.llp->bap->IONodes[i-1].PieceExtent.y;
				*/
				if(Draw)
					WinDrawRectangle(&Rtest,0);
				else
					WinEraseRectangle(&Rtest,0);
				//
				//
			}
		
		break;
		case FrontPanelForm:
		/*
			if(InAreaInf.llp->SelfBlockLLHead != SYSHOOK->BlockLLHeadP)
			{
				window_x=0;window_y=0;
				bllp = InAreaInf.llp->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{//4-26 ���@�M��C�A�[�X����y��
					window_x += bllp->bap->XY.x;
					window_y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}
			
				//window_x = InAreaInf.llp->SelfBlockLLHead->bap->XY.x;
				//window_y = InAreaInf.llp->SelfBlockLLHead->bap->XY.y;
			}
			// 
		*/
			/*
			for(i=0;i<InAreaInf.llp->bap->PanelControlNodeN && !OnNode;i++)
			{	
				OnNode = CheckBoundary(
					InAreaInf.llp->bap->PanelNodes[i].ControlNodeAreaTopLeft.x + 
					InAreaInf.llp->bap->PanelXY.x + window_x,
					InAreaInf.llp->bap->PanelNodes[i].ControlNodeAreaTopLeft.y + 
					InAreaInf.llp->bap->PanelXY.y + window_y,
					InAreaInf.llp->bap->PanelNodes[i].ControlNodeAreaExtent.x,
					InAreaInf.llp->bap->PanelNodes[i].ControlNodeAreaExtent.y,now_x,now_y);
			}
			*/
			// draw black block
			if(OnNode)
			{
			
			/*
				Rtest.topLeft.x=InAreaInf.llp->bap->PanelNodes[i-1].ControlNodeAreaTopLeft.x + 
				InAreaInf.llp->bap->PanelXY.x + window_x;
				Rtest.topLeft.y=InAreaInf.llp->bap->PanelNodes[i-1].ControlNodeAreaTopLeft.y + 
				InAreaInf.llp->bap->PanelXY.y + window_y;
				Rtest.extent.x=InAreaInf.llp->bap->PanelNodes[i-1].ControlNodeAreaExtent.x;
				Rtest.extent.y=InAreaInf.llp->bap->PanelNodes[i-1].ControlNodeAreaExtent.y;
			*/
				if(Draw)
					WinDrawRectangle(&Rtest,0);
				else
					WinEraseRectangle(&Rtest,0);
			}
			
		break;		
	}
	/*
	if(OnNode)
		return i-1;
	else
		return -1;
	*/
	return ionode;
}

/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
 void DrawMainRUN()
{
/*
	RectangleType Rtest;
	
		Rtest.topLeft.x=78;
		Rtest.topLeft.y=0;
		Rtest.extent.x=142-78;
		Rtest.extent.y=15;
		
		WinEraseRectangle(&Rtest,0);
*/
		//DrawIcon(LAMPBitmap,80,0);
		//DrawIcon(RUNBitmap,100,0);
		//DrawIcon(STOPBitmap,120,0);
		//if(RUN)
		//	DrawIcon(RUNMANBitmap,80,0);
		//else
		//	DrawIcon(STOPMANBitmap,80,0);

}
void NestItemMoveToLastPosition(BNP llp)
{
			switch(llp->SelfBlockLLHead->TYPE)
			{
				case LOOPBLOCK :
				((HOOKP)llp->SelfBlockLLHead->LoopBlockHOOKP)->BlockLLHeadP = 
					ItemMoveToLastPosition(
						((HOOKP)llp->SelfBlockLLHead->LoopBlockHOOKP)->BlockLLHeadP,
							llp);
				NestItemMoveToLastPosition(llp->SelfBlockLLHead);
				break;
				case CASEBLOCK :
				((HOOKP)llp->SelfBlockLLHead->CurrentHOOKP)->BlockLLHeadP = 
					ItemMoveToLastPosition(
						((HOOKP)llp->SelfBlockLLHead->CurrentHOOKP)->BlockLLHeadP,
							llp);
				NestItemMoveToLastPosition(llp->SelfBlockLLHead);
				break;
				default:
				SYSHOOK->BlockLLHeadP = 
					ItemMoveToLastPosition(
						SYSHOOK->BlockLLHeadP,
							llp);

				break;
			}
}
void NestReDraw(BNP llp)
{
	// 5-12 �_����ø
	// �V�U���M
	//bnp=((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP;
	while(llp != NULL)
	{
		llp->bap->ReDraw=true;
		llp->bap->Clean=true;
		switch(llp->TYPE)
			{
				case LOOPBLOCK :
	        		if(((HOOKP)llp->LoopBlockHOOKP) != NULL)
	        		{
						NestReDraw(((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP);
	        		}
	        	break;
	        	case CASEBLOCK :
	        		if(((HOOKP)llp->CurrentHOOKP) != NULL)
	        		{
						NestReDraw(((HOOKP)llp->CurrentHOOKP)->BlockLLHeadP);
	        		}
				break;
				default:
//					llp->bap->ReDraw=true;
				break;
			}
		
		llp = llp->NEXTNODE;
	}
}
/***********************************************************************
 * �\��G��X���в{�b�b���@�Ӥ���W
 *
 * 2003-4-12 �Τ��@�ܼ�(LLP)�ǻ�
 * 2003-4-13 �אּ�i���j�Φ�
 * 2003-4-22 �[�J�y���c�z���P�_
 * 2003-4-26 �[�J�ư��y�⤤�ҧ�z���\��
 * 2003-5-09 �[�J�P�_tools form���ƥ�
 ***********************************************************************/
extern InAreaInfType IsInArea(InAreaInfType InAreaInf,BNP llp,UInt16 now_x,UInt16 now_y,UInt16 window_x,UInt16 window_y)
{
	//Boolean InArea=false;
	//InAreaInf.InArea=false;
	//InAreaInf.llp = NULL;
	//LLP = SYSHOOK->BlockLLHeadP->NEXTNODE; //jump over the ICON node;
	//InAreaInf.llp = syshead.BlockLLHead->NEXTNODE; //jump over the ICON node;
	//InAreaInf.llp = llp;
	Boolean InArea = false;
	// llp �o�ɫ��VLL���Y
	//BNP bnp;
	// scan whole link-list,
	//if  hit then stop,
	//if touch link-list end then stop,too.
	// 5-13 ����
	// �������
	// �A�˦^��
	if(llp != NULL)
	while(llp->NEXTNODE != NULL)
	{
		llp = llp->NEXTNODE;
	}
	
	while((llp !=NULL) && (llp != SYSHOOK->BlockLLHeadP))
	{
	  if(llp != HandPointer.CatchWhat)
	  {// �ư��⤤�ҧ�
  		switch(FrmGetActiveFormID())
		{
			case BlockDiagramForm:

				InArea = CheckBoundary(
				llp->bap->XY.x + window_x,
				llp->bap->XY.y + window_y,
				llp->bap->SIZE.x,
				llp->bap->SIZE.y,now_x,now_y);
				// �P�_�O�_�i�J�y���c�z
				if(InArea == true )
				{// �p�G�O�b���c�� �ӥB ���c������C������
	        		llp->bap->ReDraw=true;
	        		llp->bap->Clean=true;
					switch(llp->TYPE)
					{
						case LOOPBLOCK :
			        		if(((HOOKP)llp->LoopBlockHOOKP) != NULL)
			        		{
			        		InAreaInf = IsInArea(InAreaInf,((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,now_x,now_y,llp->bap->XY.x + window_x,llp->bap->XY.y + window_y);
			        		if(InAreaInf.InArea)
			        		{
				        		llp->bap->ReDraw=true;
				        		llp->bap->Clean=true;
			        			llp = InAreaInf.llp;
			        		}
			        		}
			        	break;
			        	case CASEBLOCK :
			        		if(((HOOKP)llp->CurrentHOOKP) != NULL)
			        		{
			        		InAreaInf = IsInArea(InAreaInf,((HOOKP)llp->CurrentHOOKP)->BlockLLHeadP,now_x,now_y,llp->bap->XY.x + window_x,llp->bap->XY.y + window_y);
			        		if(InAreaInf.InArea)
			        		{
				        		llp->bap->ReDraw=true;
				        		llp->bap->Clean=true;
			        			llp = InAreaInf.llp;
			        		}
			        		}
						break;
						default:
						break;
					}
				}
			break;
			case FrontPanelForm:
			/*
				if(llp->pap != NULL)
				InArea = CheckBoundary(
				llp->pap->XY.x,
				llp->pap->XY.y,
				llp->pap->SIZE.x,
				llp->pap->SIZE.y,now_x,now_y);
*/
//				if(InArea == true )
//				{// �p�G�O�b���c�� �ӥB ���c������C������
//	        		llp->bap->ReDraw=true;
//	        		llp->bap->Clean=true;
					switch(llp->TYPE)
					{
						case LOOPBLOCK :
			        		if(((HOOKP)llp->LoopBlockHOOKP) != NULL)
			        		{
			        		InAreaInf = IsInArea(InAreaInf,((HOOKP)llp->LoopBlockHOOKP)->BlockLLHeadP,now_x,now_y,0,0);
			        		if(InAreaInf.InArea)
			        		{
				        		llp->bap->ReDraw=true;
				        		llp->bap->Clean=true;
			        			llp = InAreaInf.llp;
			        			InArea=InAreaInf.InArea;
			        		}
			        		}
			        	break;
			        	case CASEBLOCK :
			        		if(((HOOKP)llp->CurrentHOOKP) != NULL)
			        		{
			        		InAreaInf = IsInArea(InAreaInf,((HOOKP)llp->CurrentHOOKP)->BlockLLHeadP,now_x,now_y,0,0);
			        		if(InAreaInf.InArea)
			        		{
				        		llp->bap->ReDraw=true;
				        		llp->bap->Clean=true;
			        			llp = InAreaInf.llp;
			        			InArea=InAreaInf.InArea;
			        		}
			        		}
						break;
						default:
						if(llp->pap!=NULL)
						{
							InArea = CheckBoundary(
							llp->pap->XY.x,
							llp->pap->XY.y,
							llp->pap->SIZE.x,
							llp->pap->SIZE.y,now_x,now_y);
							InAreaInf.InArea=InArea;
						}
						break;
					}
//				}

			break; 
			case BlockToolsForm:
				InArea = CheckBoundary(
				llp->bap->XY.x,
				llp->bap->XY.y,
				llp->bap->SIZE.x,
				llp->bap->SIZE.y,now_x,now_y);
			break;
			case PanelToolsForm:
				InArea = CheckBoundary(
				llp->bap->XY.x,
				llp->bap->XY.y,
				llp->bap->SIZE.x,
				llp->bap->SIZE.y,now_x,now_y);
			break;
			case FunctionsForm:
				InArea = CheckBoundary(
				llp->bap->XY.x,
				llp->bap->XY.y,
				llp->bap->SIZE.x,
				llp->bap->SIZE.y,now_x,now_y);
				/*
				if(InArea == true )
				{
					if(llp->TYPE == 9000)
					{
	        			InAreaInf = IsInArea(InAreaInf,((HOOKP)llp->hookP)->BlockLLHeadP,now_x,now_y,llp->bap->XY.x + window_x,llp->bap->XY.y + window_y);
	        			if(InAreaInf.InArea)
		        			{
		        				llp = InAreaInf.llp;
		        			}
		        	}
				}
				*/
			break;
			default:
			break;
	
			}
			
			if(InArea == true)// jump out when In the area.l
				break;
	  }
	  // 5-13 ����
	  llp = llp->PREVNODE;
	  //llp = llp->NEXTNODE;
	}

	InAreaInf.InArea = InArea;
	InAreaInf.llp = llp;
	return InAreaInf;
}
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
 void setNewObjXY(UInt16 newx,UInt16 newy)
{
	//HandPointer.CatchWhat;
	UInt16 x,y;
	BNP bllp;
	
	switch(FrmGetActiveFormID())
	{
		case BlockDiagramForm:
		  switch(HandPointer.CatchWhat->SelfBlockLLHead->TYPE)
		  { //�b�M�w�s�y�Ф��e�A�P�_�O�_���y���c�z������A�p�O�A�n��h�y���c�z�Ҧb�y��
		  // ���ͬ۹��m
				case LOOPBLOCK :
	        	case CASEBLOCK :

				// �_�����c��R
				x=0;y=0;
				bllp = HandPointer.CatchWhat->SelfBlockLLHead;
				while(bllp != SYSHOOK->BlockLLHeadP)
				{
					x += bllp->bap->XY.x;
					y += bllp->bap->XY.y;
					bllp = bllp->SelfBlockLLHead;
				}

        	
        		newx -= x;
        		newy -= y;
        	break;
        	default:
        	break;
		  }
			HandPointer.CatchWhat->bap->XY.x = newx;
			HandPointer.CatchWhat->bap->XY.y = newy;
		break;
		case FrontPanelForm:
			HandPointer.CatchWhat->pap->XY.x = newx;
			HandPointer.CatchWhat->pap->XY.y = newy;
		break;
		default:
		break;
	}
}
///////////////////////////////////////////////////////////////////
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//
//  CAUTION
//  recursive area
//
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////

/***********************************************************************
 *
 * FUNCTION:    GetObjectPtr
 *
 * DESCRIPTION: This routine returns a pointer to an object in the current
 *              form.
 *
 * PARAMETERS:  formId - id of the form to display
 *
 * RETURNED:    VoidPtr
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
VoidPtr GetObjectPtr(Word objectID)
	{
	FormPtr frmP;


	frmP = FrmGetActiveForm();
	return (FrmGetObjectPtr(frmP, FrmGetObjectIndex(frmP, objectID)));
}


/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:  frm - pointer to the MainForm form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
static void MainFormInit(FormPtr frmP)
{
}


/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the 
 *              "MainForm" of this application.
 *
 * PARAMETERS:  eventP  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

/*
 5-9 �ק���ɧP�_ now_x �P now_y ���]�w
*/
void CaculateNowXNowY(UInt16 *now_x,UInt16 *now_y,UInt16 screenX,UInt16 screenY)
{
	#define PointIconYExt 18
	
	if(BlockPointerState == HANDBitmap)
	{// 
		if(HandPointer.HandState == CLENCHBitmap)
		{// ����F�� x,y ����
			if(screenX <(
				HandPointer.RelativePoint.x))
			{
			*now_x = 
				HandPointer.RelativePoint.x;
			}
			else
				*now_x = screenX;
			if(screenY <((15)+
				HandPointer.RelativePoint.y))
			{
				*now_y = (15) +
				HandPointer.RelativePoint.y;
			}
			else
				*now_y = screenY;
			
		}else{// �S��F�� y = hand ������
			*now_x = screenX;
			if(screenY <(PointIconYExt))
				*now_y = (PointIconYExt);
			else
				*now_y = screenY;
		}
	}else{
			*now_x = screenX;
			if(screenY <(PointIconYExt))
				*now_y = (PointIconYExt);
			else
				*now_y = screenY;
	}
		
}

long GetNodeData(IONODEP ionode)
{
	switch(ionode->BYTES)
	{
	case 2:
		return *((int *)ionode->P);
	break;
	case 4:
		return *((long *)ionode->P);
	break;
	}
	return 0;
}
// 6-8
void DrawDecimalIntKeyboard(UInt16 x,UInt16 y)
{
	FontID fontid;
	int i,j;
	int keyindex;
	RectangleType Rtest;
	char *keyboardtext[15]={"1","2","3","4",
							"5","6","7","8","9","0","c","E"
							,"-","",""};
		
	Rtest.topLeft.x=x;
	Rtest.topLeft.y=y;
	Rtest.extent.x=30;
	Rtest.extent.y=60;
	WinEraseRectangle(&Rtest,0);

	WinDrawLine(x,y,x+30,y);
	WinDrawLine(x+30,y,x+30,y+60);
	WinDrawLine(x+30,y+60,x,y+60);
	WinDrawLine(x,y+60,x,y);
	
	fontid=FntGetFont();
	FntSetFont(boldFont);
	
	keyindex=0;
	for(j=0;j<60;j+=12)
	{
		for(i=0;i<30;i+=10)
		{
			WinDrawChars(keyboardtext[keyindex],
				StrLen(keyboardtext[keyindex]),
 			i+x+3,
			j+y+1);
			
			keyindex++;
		}
	}		
	FntSetFont(fontid);

}
// 6-8
void EraseDecimalIntKeyboard(UInt16 x,UInt16 y)
{
	FontID fontid;
	int i,j;
	int keyindex;
	char *keyboardtext[15]={"1","2","3","4",
							"5","6","7","8","9","0","c","E"
							,"-","",""};
		
	WinEraseLine(x,y,x+30,y);
	WinEraseLine(x+30,y,x+30,y+60);
	WinEraseLine(x+30,y+60,x,y+60);
	WinEraseLine(x,y+60,x,y);
	
	fontid=FntGetFont();
	FntSetFont(boldFont);
	
	keyindex=0;
	for(j=0;j<60;j+=12)
	{
		for(i=0;i<30;i+=10)
		{
			WinEraseChars(keyboardtext[keyindex],
				StrLen(keyboardtext[keyindex]),
 			i+x+3,
			j+y+1);
			
			keyindex++;
		}
	}		
	FntSetFont(fontid);

}
// 6-8
Boolean KeyboardPendownProcess(int keyboard_x,int keyboard_y,BNP objectB,int x,int y)
{
	char datavaluetxt[8];
	char datavaluetxt2[8];
	//int datavalue;
	
	// Ū�X��ƭ�
	switch(objectB->pap->BITMAPID)
	{
		case CTRLU8Bitmap:
			StrIToA(datavaluetxt,*(int *)objectB->bap->IONodeLLHead->P);
		break;
	}
	// ���U���@�ӫ��s�H
	if(CheckBoundary(keyboard_x+0,keyboard_y+0,10,12,x,y))
	{
		StrCat(datavaluetxt,"1");
	}
	else if(CheckBoundary(keyboard_x+10,keyboard_y+0,10,12,x,y))
	{
		StrCat(datavaluetxt,"2");
	}
	else if(CheckBoundary(keyboard_x+20,keyboard_y+0,10,12,x,y))
	{
		StrCat(datavaluetxt,"3");
	}
	else if(CheckBoundary(keyboard_x+0,keyboard_y+12,10,12,x,y))
	{
		StrCat(datavaluetxt,"4");
	}
	else if(CheckBoundary(keyboard_x+10,keyboard_y+12,10,12,x,y))
	{
		StrCat(datavaluetxt,"5");
	}
	else if(CheckBoundary(keyboard_x+20,keyboard_y+12,10,12,x,y))
	{
		StrCat(datavaluetxt,"6");
	}
	else if(CheckBoundary(keyboard_x+0,keyboard_y+24,10,12,x,y))
	{
		StrCat(datavaluetxt,"7");
	}
	else if(CheckBoundary(keyboard_x+10,keyboard_y+24,10,12,x,y))
	{
		StrCat(datavaluetxt,"8");
	}
	else if(CheckBoundary(keyboard_x+20,keyboard_y+24,10,12,x,y))
	{
		StrCat(datavaluetxt,"9");
	}
	else if(CheckBoundary(keyboard_x+0,keyboard_y+36,10,12,x,y))
	{
		StrCat(datavaluetxt,"0");
	}
	else if(CheckBoundary(keyboard_x+10,keyboard_y+36,10,12,x,y))
	{
		datavaluetxt[0]='\0';
		StrCat(datavaluetxt,"0");
	}
	else if(CheckBoundary(keyboard_x+20,keyboard_y+36,10,12,x,y))
	{
		// �g�^��ƭ�
		switch(objectB->pap->BITMAPID)
		{
			case CTRLU8Bitmap:
				*(int *)objectB->bap->IONodeLLHead->P=StrAToI(datavaluetxt);
			break;
		}
		return false;
	}
	else if(CheckBoundary(keyboard_x+0,keyboard_y+48,10,12,x,y))
	{
		if(datavaluetxt[0]=='-')
		{
			StrCopy(datavaluetxt2,datavaluetxt+1);
			StrCopy(datavaluetxt,datavaluetxt2);
		}else{
			datavaluetxt2[0]='-';
			datavaluetxt2[1]='\0';
			StrCat(datavaluetxt2,datavaluetxt);
			StrCopy(datavaluetxt,datavaluetxt2);
		}
	}
	else
	{
		return false;
	}
	// �g�^��ƭ�
	switch(objectB->pap->BITMAPID)
	{
		case CTRLU8Bitmap:
			*(int *)objectB->bap->IONodeLLHead->P=StrAToI(datavaluetxt);
		break;
	}
	return true;
}