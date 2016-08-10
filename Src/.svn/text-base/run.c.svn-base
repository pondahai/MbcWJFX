#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"

#include "vpl.h"

Boolean DoRun(BNP BlockLLHeadP,WNP WireLLHeadP);


extern HOOKP SYSHOOK;
extern Boolean LAMP;

//extern void DrawDiagram(BNP bllpH,WNP wllpH,Boolean Animation,Boolean show,int window_x,int window_y,Boolean ReDrawAll);
extern Boolean DrawRunPointOnWire(WNP wnp,Boolean not_show_result);
extern void DrawWires(WNP wllp,Boolean show,Boolean ClearBit);


/***********************************************************************
 * 2003-5-17
 * 
 ***********************************************************************/
void ResetWireStatus(BNP BlockLLHeadP,WNP WireLLHeadP)
{
	WNP wnp;
	//DrawWires(WireLLHeadP,false,true);
	wnp = WireLLHeadP;
	while(wnp!=NULL)
	{
		//if((wnp->StartBlockP == SYSHOOK->BlockLLHeadP) || (wnp->EndBlockP == SYSHOOK->BlockLLHeadP))
		//{
		//wnp->DIRTY=true;
		//}else{
		wnp->DIRTY=false;
		wnp->STARTRUN=false;
		wnp->RUNSEG=0;
		wnp->RUNPOINT.x=0;
		wnp->RUNPOINT.y=0;
		//}
		wnp=wnp->NEXTNODE;
	}
	while(BlockLLHeadP!=NULL)
	{
		switch(BlockLLHeadP->TYPE)
		{
			case HOOKBLOCK:
				ResetWireStatus(((HOOKP)BlockLLHeadP->hookP)->BlockLLHeadP,((HOOKP)BlockLLHeadP->hookP)->WireLLHeadP);
			break;
			case LOOPBLOCK:
				ResetWireStatus(((HOOKP)BlockLLHeadP->LoopBlockHOOKP)->BlockLLHeadP,((HOOKP)BlockLLHeadP->LoopBlockHOOKP)->WireLLHeadP);
			break;
			case CASEBLOCK:
			break;
		}
		BlockLLHeadP=BlockLLHeadP->NEXTNODE;
	}
}
/***********************************************************************
 * 2003-4-12
 * 2003-5-17 改 巢狀
 ***********************************************************************/
void CheckConnection(BNP BL,Boolean IOBlockClear)
{

	//BlockLink BL;
//	int i;
	IONODEP ionode;
	//int inodeN,onodeN;
	
		//BL = BlockLLHeadP;
		while(BL!=NULL)
		{
			BL->bap->INodeNUM=0;
			BL->bap->ONodeNUM=0;
			ionode=BL->bap->IONodeLLHead;
			while(ionode != NULL)
			{
				if(ionode->TYPE==0)
					BL->bap->ONodeNUM++;
				if(ionode->TYPE==1)
					BL->bap->INodeNUM++;
					
				ionode = ionode->NEXT;
			}
			if(BL->bap->INodeNUM==0)
				BL->bap->INodeNUM=-1;
			if(BL->bap->ONodeNUM==0)
				BL->bap->ONodeNUM=-1;
				
			switch(BL->bap->BITMAPID)
			{
				case CTRLU8BLOCKBitmap: // 這些 block 只有一個IO點
				case LOGICCTRLBitmap:
				//case FORLOOPNBitmap: // 5-31 實驗
				// 如果不是SYSHOOK的元件 那麼就該設定notready
				if(IOBlockClear)
				{
					BL->bap->IONodeLLHead->NotReady = true;
					
				}
				else
				{
					BL->bap->IONodeLLHead->NotReady = false;
					BL->bap->ONodeNUM=0;
				}
				break;
				case FORLOOPIBitmap:
					BL->bap->IONodeLLHead->NotReady = true;
				break;
				case WHILELOOPBitmap:
					ionode=BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						ionode->NotReady=true;
						ionode = ionode->NEXT;
					}
					CheckConnection(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,IOBlockClear);
				break;
				case FORLOOPBitmap:
					ionode=BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						ionode->NotReady=true;
						ionode = ionode->NEXT;
					}
			   		// 6-2
						*((long *)
						((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->
						NEXTNODE->
						bap->IONodeLLHead->P) = 0;
					// 5-30 在這裡面 會順便reset forloop 的N ,I 的IO 狀態
					CheckConnection(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,IOBlockClear);
				break;
				case CUSTOMBLOCKBitmap:
					ionode=BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						ionode->NotReady=true;
						ionode = ionode->NEXT;
					}

					CheckConnection(((HOOKP)BL->hookP)->BlockLLHeadP,true);
				break;
				case SWITCHCASEBitmap:
				break;
				/*
				case INDICATOR8BLOCKBitmap:
				case LOGICINDICATORBitmap:
				case ADDBitmap:
				case SUBBitmap:
				case MULBitmap:
				case DIVBitmap:
				case ANDBitmap:
				case ORBitmap:
				case NOTBitmap:
				case CUSTOMBLOCKBitmap:
				*/
				default:
					ionode=BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						ionode->NotReady=true;
						ionode = ionode->NEXT;
					}
				/*
					for(i=0;i<BL->bap->IONodeN;i++)
					{
						BL->bap->IONodes[i].DATAnotREADY = true;
					}
				*/
				break;
			}
			BL = BL->NEXTNODE;
		}

}
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
 /*
BNP FindNodeByID(BNP HeadP,UInt16 ID)
{
	while(HeadP != NULL)
	{
		if(HeadP->ID == ID)
			break;
		HeadP = HeadP->NEXTNODE;
	}
	return HeadP;
}
*/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
void DoItemRUN(BNP BL)
{

	//int i;
	//BNP nodeP;
	//IONODEP innode;
	//IONODEP outnode;
	
		switch(BL->bap->BITMAPID)
		{
		   	case INDICATOR8BLOCKBitmap:
		   	
		   	break;
		   	case CTRLU8BLOCKBitmap:
		   	
		   	break;
		   	case LOGICINDICATORBitmap:
		   	
		   	break;
		   	case LOGICCTRLBitmap:
		   	
		   	break;
			case ADDBitmap:
			/*
				outnode = BL->bap->IONodeLLHead;
				while(outnode->TYPE != 0)
				{
					outnode=outnode->NEXT;
				}
				*(outnode->P) = 0;
				innode = BL->bap->IONodeLLHead;
				while(innode != NULL)
				{
					if(innode->TYPE == 1)
					{
						*(outnode->P) += *(innode->P);
					}
					innode=innode->NEXT;
				}
				outnode->NotReady=false;
			*/
				*((int *)BL->bap->IONodeLLHead->NEXT->NEXT->P) =
				*((int *)BL->bap->IONodeLLHead->P) +
				*((int *)BL->bap->IONodeLLHead->NEXT->P);
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
				/*
					BL->bap->IONodes[2].value =
					BL->bap->IONodes[0].value +
					BL->bap->IONodes[1].value;
					BL->bap->IONodes[2].DATAnotREADY = false;
				*/
			break;
			case SUBBitmap:
				*((int *)BL->bap->IONodeLLHead->NEXT->NEXT->P) =
				*((int *)BL->bap->IONodeLLHead->P) -
				*((int *)BL->bap->IONodeLLHead->NEXT->P);
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
				/*
					BL->bap->IONodes[2].value =
					BL->bap->IONodes[0].value -
					BL->bap->IONodes[1].value;
					BL->bap->IONodes[2].DATAnotREADY = false;
				*/
			break;
			case MULBitmap:
				*((int *)BL->bap->IONodeLLHead->NEXT->NEXT->P) =
				*((int *)BL->bap->IONodeLLHead->P) *
				*((int *)BL->bap->IONodeLLHead->NEXT->P);
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			/*
					BL->bap->IONodes[2].value =
					BL->bap->IONodes[0].value *
					BL->bap->IONodes[1].value;
					BL->bap->IONodes[2].DATAnotREADY = false;
			*/
			break;
			case DIVBitmap:
				*((int *)BL->bap->IONodeLLHead->NEXT->NEXT->P) =
				*((int *)BL->bap->IONodeLLHead->P) /
				*((int *)BL->bap->IONodeLLHead->NEXT->P);
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			/*
					BL->bap->IONodes[2].value =
					BL->bap->IONodes[0].value /
					BL->bap->IONodes[1].value;
					BL->bap->IONodes[2].DATAnotREADY = false;
			*/
			break;
			case ANDBitmap:
				*((int *)BL->bap->IONodeLLHead->NEXT->NEXT->P) =
				*((int *)BL->bap->IONodeLLHead->P) &
				*((int *)BL->bap->IONodeLLHead->NEXT->P);
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			/*
					BL->bap->IONodes[2].value =
					BL->bap->IONodes[0].value &
					BL->bap->IONodes[1].value;
					BL->bap->IONodes[2].DATAnotREADY = false;
					*/
			break;
			case ORBitmap:
				*((int *)BL->bap->IONodeLLHead->NEXT->NEXT->P) =
				*((int *)BL->bap->IONodeLLHead->P) |
				*((int *)BL->bap->IONodeLLHead->NEXT->P);
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			/*
					BL->bap->IONodes[2].value =
					BL->bap->IONodes[0].value |
					BL->bap->IONodes[1].value;
					BL->bap->IONodes[2].DATAnotREADY = false;
					*/
			break;
			case NOTBitmap:
				*((int *)BL->bap->IONodeLLHead->NEXT->P) =
				*((int *)BL->bap->IONodeLLHead->P)?false:true;
				BL->bap->IONodeLLHead->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			/*
					if(BL->bap->IONodes[0].value == true)
					  BL->bap->IONodes[1].value =false;
					else
					  BL->bap->IONodes[1].value =true;
					BL->bap->IONodes[1].DATAnotREADY = false;
					*/
			break;
			case CUSTOMBLOCKBitmap:
			/*
				//BL->BlockLLHeadP
				//BL->WireLLHeadP
				for(i=0;i<BL->bap->IONodeN;i++)
				{
					if(BL->bap->IONodes[i].IO == true) // is ICON's Input?
					{
						//nodeP = FindNodeByID(BL->BlockLLHeadP,BL->bap->IONodes[i].internalIONodeID); // ID version
						nodeP= (BlockLink)BL->bap->IONodes[i].internalIONodeP; // Ptr version
						nodeP->bap->IONodes[0].value = BL->bap->IONodes[i].value;
						nodeP->bap->IONodes[0].DATAnotREADY = false;
					}
				}
				// ICON node有兩個角色
				// 獨立時是串列的頭 處理時要跳過
				// 被『自訂方塊』讀入時 是系統串列的其中一個node
				// 做執行時沒有跳過的動作，因為BlockLLHeadP 就是記住串列第一個node 不是ICON
				DoRun(BL->BlockLLHeadP->NEXTNODE,BL->WireLLHeadP);// 
				for(i=0;i<BL->bap->IONodeN;i++)
				{
					if(BL->bap->IONodes[i].IO == false) // is ICON's Output?
					{
						//nodeP = FindNodeByID(BL->BlockLLHeadP,BL->bap->IONodes[i].internalIONodeID); // ID version
						nodeP= (BlockLink)BL->bap->IONodes[i].internalIONodeP; // Ptr version
						BL->bap->IONodes[i].value = nodeP->bap->IONodes[0].value;
						BL->bap->IONodes[i].DATAnotREADY = false;
					}
				}
			*/
			break;
		   	case ABOVEBitmap:
		   		if(*(int *)BL->bap->IONodeLLHead->P >
		   		*(int *)BL->bap->IONodeLLHead->NEXT->P)
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=true;
		   		else
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=false;
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			break;
		   	case LESSBitmap:
		   		if(*(int *)BL->bap->IONodeLLHead->P <
		   		*(int *)BL->bap->IONodeLLHead->NEXT->P)
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=true;
		   		else
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=false;
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			break;
		   	case EQUBitmap:
		   		if(*(int *)BL->bap->IONodeLLHead->P ==
		   		*(int *)BL->bap->IONodeLLHead->NEXT->P)
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=true;
		   		else
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=false;
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			break;
		   	case LESSEQUBitmap:
		   		if(*(int *)BL->bap->IONodeLLHead->P <=
		   		*(int *)BL->bap->IONodeLLHead->NEXT->P)
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=true;
		   		else
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=false;
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			break;
		   	case ABOVEEQUBitmap:
		   		if(*(int *)BL->bap->IONodeLLHead->P >=
		   		*(int *)BL->bap->IONodeLLHead->NEXT->P)
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=true;
		   		else
		   			*(int *)BL->bap->IONodeLLHead->NEXT->NEXT->P=false;
				BL->bap->IONodeLLHead->NEXT->NEXT->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			break;
		   	case FORLOOPBitmap:
			break;
		   	case WHILELOOPBitmap:
			break;
		   	case SWITCHCASEBitmap:
		   		
			break;
		   	case FORLOOPNBitmap:
		   		//*(long *)BL->NEXTNODE->bap->IONodeLLHead->P=*(long *)BL->bap->IONodeLLHead->P;
		   		BL->NEXTNODE->bap->IONodeLLHead->NotReady=false;
				// 6-3
				BL->bap->ONodeNUM=0;
			break;

		}

}
/*
void DoWireRun(WNP WireLLHeadP)
{
	WNP  WL;
	IONODEP startnode,endnode;

		WL = WireLLHeadP;
		while(WL!=NULL)
		{
		  if((WL->DIRTY ==false))
		  {
			//startblock = FindItemByID(BlockLLHeadP,WL->wire.StartBlockID);
			//endblock   = FindItemByID(BlockLLHeadP,WL->wire.EndBlockID);
			// 舊
			//startblock = (BNP)WL->wire.StartBlockP;
			//endblock   = (BNP)WL->wire.EndBlockP;
			startnode	=	WL->StartNodeP;
			endnode		=	WL->EndNodeP;
			// if one node is output then another node is input
			// 舊
			//if((startblock->bap->IONodes[WL->wire.StartNodeID].DATAnotREADY == false) &&
			//	(startblock->bap->IONodes[WL->wire.StartNodeID].IO == false))//check output status
			if((startnode->NotReady == false) &&
				(startnode->TYPE == 0))//check output status
			{
				//output data is ready
				//move data from output to input
				*(int *)endnode->P =
				  *(int *)startnode->P;
				
				endnode->NotReady = false;
					if(LAMP)
					{
						WL->DIRTY=true;
					}
			}
			if((endnode->NotReady == false) &&
				(endnode->TYPE == 0))
			{
				//output data is ready
				//move data from output to input
				*(int *)startnode->P = 
				  *(int *)endnode->P;
				  
				startnode->NotReady = false;
					if(LAMP)
					{
						WL->DIRTY=true;
					}
			}
			// if both node are input then 
			if((endnode->TYPE == 1) &&
				(startnode->TYPE == 1))
			{
				// if one node data is ready and another is not ready
				if((startnode->NotReady == true) &&
				    (endnode->NotReady == false))
				{
					*(int *)startnode->P = 
					  *(int *)endnode->P;
				
					startnode->NotReady = false;
					if(LAMP)
					{
						WL->DIRTY=true;
					}
				}
				if((startnode->NotReady == false) &&
				    (endnode->NotReady == true))
				{
					*(int *)endnode->P =
					  *(int *)startnode->P;
				
					endnode->NotReady = false;
					if(LAMP)
					{
						WL->DIRTY=true;
					}
				}
			}
		  }// if
		WL = WL->NEXTNODE;
		}

}
Boolean DoBlockRun(BNP BlockLLHeadP)
{
	BNP BL;
	Boolean INPUTREADY;
	IONODEP ionode;
	Boolean handle;

		handle = false;

		BL = BlockLLHeadP;
		while(BL!=NULL)
		{
			INPUTREADY = true;
		    	ionode = BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{
					if((ionode->NotReady == true) && 
					(ionode->TYPE == true)) // check input node's data status
					{
						INPUTREADY = false;
						handle = true;
					}
					
					ionode=ionode->NEXT;
				}
					if(INPUTREADY)
					{
						DoItemRUN(BL);
					}
			BL = BL->NEXTNODE;
		}
		
		
		
		
	return handle;
}
*/

// 6-2
void MoveNodeData(IONODEP desnode,IONODEP srcnode)
{
	//long lvalue;
	
	if(desnode->BYTES == srcnode->BYTES)
	{
		switch(desnode->BYTES)
		{
			case 2:
				*((int *)desnode->P)=*((int *)srcnode->P);
			break;
			case 4:
				*((long *)desnode->P)=*((long *)srcnode->P);
			break;
		}
	}else if((desnode->BYTES == 2)&&(srcnode->BYTES == 4))
	{
		*((int *)desnode->P)=(int)*((long *)srcnode->P);
	}else if((desnode->BYTES == 4)&&(srcnode->BYTES == 2))
	{
		*((long *)desnode->P)=*((int *)srcnode->P);
	}
}
void DoRun_WireRun(WNP WireLLHeadP)
{
	WNP  WL;
	IONODEP startnode,endnode;

		WL = WireLLHeadP;
		while(WL!=NULL)
		{
		  //if((WL->DIRTY ==false))//&&(WL->STARTRUN==false))
		  //{
          if(LAMP?((WL->RUNPOINT.x == -1)&&(WL->RUNPOINT.y == -1)):true)
		{  if((WL->DIRTY == false))
	  	  {if((WL->StartBlockP != SYSHOOK->BlockLLHeadP) && (WL->EndBlockP != SYSHOOK->BlockLLHeadP))
	  	 //{ if((WL->StartBlockP->TYPE != HOOKBLOCK) && (WL->EndBlockP->TYPE != HOOKBLOCK))
		  {//跳過 暗線段

			//startblock = FindItemByID(BlockLLHeadP,WL->wire.StartBlockID);
			//endblock   = FindItemByID(BlockLLHeadP,WL->wire.EndBlockID);
			// 舊
			//startblock = (BNP)WL->wire.StartBlockP;
			//endblock   = (BNP)WL->wire.EndBlockP;
			startnode	=	WL->StartNodeP;
			endnode		=	WL->EndNodeP;
			// if one node is output then another node is input
			// 舊
			//if((startblock->bap->IONodes[WL->wire.StartNodeID].DATAnotREADY == false) &&
			//	(startblock->bap->IONodes[WL->wire.StartNodeID].IO == false))//check output status
			
			// 5-18 加入尾端判斷 如果尾端也已經 資料備妥 就不要搬移
			if((startnode->NotReady == false) &&
				(startnode->TYPE == 0) && (endnode->NotReady == true))//check output status
			{
				//output data is ready
				//move data from output to input
				//*(endnode->P) =
				//  (int)*(startnode->P);
				
				// 6-2
				/*
				switch(endnode->BYTES)
				{
					case 2:
						*((int *)endnode->P)=0;
					break;
					case 4:
						*((long *)endnode->P)=0;
					break;
				}
				*/
				MoveNodeData(endnode,startnode);
				
				endnode->NotReady = false;
				WL->EndBlockP->bap->INodeNUM--;
				
				
					if(LAMP)
					{
						WL->DIRTY=true;
					}
			}
			if((endnode->NotReady == false) &&
				(endnode->TYPE == 0) && (startnode->NotReady == true))
			{
				//output data is ready
				//move data from output to input
				//*(startnode->P) = 
				  //(int)*(endnode->P);
				/*
				// 6-2
				switch(startnode->BYTES)
				{
					case 2:
						*((int *)startnode->P)=0;
					break;
					case 4:
						*((long *)startnode->P)=0;
					break;
				}
				*/
				MoveNodeData(startnode,endnode);
				  
				startnode->NotReady = false;
				WL->StartBlockP->bap->INodeNUM--;
					if(LAMP)
					{
						WL->DIRTY=true;
					}
			}
			// if both node are input then 
			if((endnode->TYPE == 1) &&
				(startnode->TYPE == 1))
			{
				// if one node data is ready and another is not ready
				if((startnode->NotReady == true) &&
				    (endnode->NotReady == false) )
				{
					//*(startnode->P) = 
					//  (int)*(endnode->P);
				/*
					// 6-2
					switch(startnode->BYTES)
					{
						case 2:
							*((int *)startnode->P)=0;
						break;
						case 4:
							*((long *)startnode->P)=0;
						break;
					}
				*/
					MoveNodeData(startnode,endnode);
				
					startnode->NotReady = false;
					WL->StartBlockP->bap->INodeNUM--;
					if(LAMP)
					{
						WL->DIRTY=true;
					}
				}
				if((startnode->NotReady == false) &&
				    (endnode->NotReady == true) )
				{
					//*(endnode->P) =
					//  (int)*(startnode->P);
				/*
					// 6-2
					switch(endnode->BYTES)
					{
						case 2:
							*((int *)endnode->P)=0;
						break;
						case 4:
							*((long *)endnode->P)=0;
						break;
					}
				*/
					MoveNodeData(endnode,startnode);
				
					endnode->NotReady = false;
					WL->EndBlockP->bap->INodeNUM--;
					if(LAMP)
					{
						WL->DIRTY=true;
					}
				}
			}
		  //}// if DIRTY
		  }}}
		WL = WL->NEXTNODE;
		}
}
WNP FindWireNodeByIONode(WNP wireLL,IONODEP ionode)
{
	while(wireLL!=NULL)
	{
		if((wireLL->StartNodeP == ionode) ||
			(wireLL->EndNodeP == ionode))
		{
			return wireLL;
		}
		wireLL=wireLL->NEXTNODE;
	}
	return 0;
}
Boolean DoRun_HOOKBLOCK(BNP BL)
{
	IONODEP ionode;
	Boolean		INPUTREADY = true;
	Boolean	OUTPUTNOTYET = false;
	Boolean		HAVENOOUTPUT = true;
	Boolean LAMPSTATUS;
	WNP  WL;
	Boolean handle=false;
	int swap_i;
	
				// 5-17 while 迴圈採用 內部預設一個元件 
				// 這個元件是用來決定這個迴圈是否迴圈執行
				//
/*
		    	ionode = BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{
					//if((BL->bap->IONodes[i].DATAnotREADY == true) && 
					//(BL->bap->IONodes[i].IO == true)) // check input node's data status
					if((ionode->NotReady == true) && 
					(ionode->TYPE == true)) // check input node's data status
					{
						INPUTREADY = false;
						handle = true;
					}
					
					ionode=ionode->NEXT;
				}
*/
					/////////////////////////////////
					// 6-3 
					// 改版 輸入備妥 測試方法
					//
					/////////////////////////////////
				if((BL->bap->INodeNUM != 0)&&(BL->bap->INodeNUM != -1))
				{
						INPUTREADY = false;
						handle = true;
				}
				
/*				
				// 6-2 加入判斷 輸出是否已完成 如果是 則不用再進去執行了
				//     如果有一個未完成 則NOTALLOK 為真
		    	ionode = BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{   
					if(ionode->TYPE == 0)
					{
						HAVENOOUTPUT = false;
						if(ionode->NotReady == true) // check output node's data status
						{
							OUTPUTNOTYET = true;
						}
					}
					
					ionode=ionode->NEXT;
				}
*/

				//if(INPUTREADY && (HAVENOOUTPUT?true:OUTPUTNOTYET))
				if(INPUTREADY && ((BL->bap->ONodeNUM == -1)?true:(BL->bap->ONodeNUM != 0)))
				{

				// 6-3 把輸入資料備妥
				ionode=BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{   
					if(ionode->TYPE == 1)
					{
						WL=FindWireNodeByIONode(((HOOKP)BL->hookP)->WireLLHeadP,ionode);
						if(WL!=NULL)
						{
							if(WL->StartBlockP == BL)
							{
								MoveNodeData(WL->EndNodeP,WL->StartNodeP);
								WL->EndNodeP->NotReady=false;
								WL->DIRTY=true;
							}
							if(WL->EndBlockP == BL)
							{
								MoveNodeData(WL->StartNodeP,WL->EndNodeP);
								WL->StartNodeP->NotReady=false;
								WL->DIRTY=true;
							}
						}
					}	
					ionode=ionode->NEXT;
				}
				// 6-3 把output wire 變DIRTY 這樣裡面的run wire 就不會執行
				ionode=BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{   
					if(ionode->TYPE == 0)
					{
						WL=FindWireNodeByIONode(((HOOKP)BL->hookP)->WireLLHeadP,ionode);
						if(WL!=NULL)
						{
							WL->DIRTY=true;
						}
					}	
					ionode=ionode->NEXT;
				}

				// 5-18 迴圈邊上的IO點 在這裡動態切換 因為進到裡面之後 角色變換

				swap_i=BL->bap->ONodeNUM;
				BL->bap->ONodeNUM=BL->bap->INodeNUM;
				BL->bap->INodeNUM=swap_i;
				

		    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
					if(ionode->TYPE == 1)
						ionode->TYPE = 0;
 					else
						ionode->TYPE = 1;
						
					ionode=ionode->NEXT;
					}
				//handle=
				LAMPSTATUS=LAMP;
				LAMP=false;
				if(DoRun(((HOOKP)BL->hookP)->BlockLLHeadP,((HOOKP)BL->hookP)->WireLLHeadP))
				{
					LAMP=LAMPSTATUS;

					// 5-18 迴圈邊上的IO點 在這裡動態切回來
				swap_i=BL->bap->ONodeNUM;
				BL->bap->ONodeNUM=BL->bap->INodeNUM;
				BL->bap->INodeNUM=swap_i;
				

			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
					if(ionode->TYPE == 1)
						ionode->TYPE = 0;
					else
						ionode->TYPE = 1;
						
					ionode=ionode->NEXT;
					}
					// 5-18 內部未完成 強制 handle 為 true
					handle=true;
					
					// 5-18 動畫內部未完成 輸出未完成
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{
							ionode->NotReady=true;
						}
						
					ionode=ionode->NEXT;
					}
					
				}
				else
				{	// 已完成 內部運算
					LAMP=LAMPSTATUS;

					// 5-18 迴圈邊上的IO點 在這裡動態切回來
				swap_i=BL->bap->ONodeNUM;
				BL->bap->ONodeNUM=BL->bap->INodeNUM;
				BL->bap->INodeNUM=swap_i;
				

			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
					if(ionode->TYPE == 1)
						ionode->TYPE = 0;
					else
						ionode->TYPE = 1;
						
					ionode=ionode->NEXT;
					}
					
					// 5-18 動畫內部已完成 輸出也完成
					/*
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{
							ionode->NotReady=false;
						}
						
					ionode=ionode->NEXT;
					}
					*/
					
				//FrmCustomAlert(MESSAGEAlert,"HOOKBLOCK is finish.","","");

				// 6-3 把資料抓出來
				ionode=BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{   
					if(ionode->TYPE == 0)
					{
						WL=FindWireNodeByIONode(((HOOKP)BL->hookP)->WireLLHeadP,ionode);
						if(WL!=NULL)
						{
							if((WL->StartBlockP == BL)&&(WL->EndNodeP->NotReady==false))
							{
								MoveNodeData(WL->StartNodeP,WL->EndNodeP);
								WL->StartNodeP->NotReady=false;
								BL->bap->ONodeNUM--;
								//WL->DIRTY=true;
							}
							if((WL->EndBlockP == BL)&&(WL->StartNodeP->NotReady==false))
							{
								MoveNodeData(WL->EndNodeP,WL->StartNodeP);
								WL->EndNodeP->NotReady=false;
								BL->bap->ONodeNUM--;
								//WL->DIRTY=true;
							}
						}
					}	
					ionode=ionode->NEXT;
				}
			
					// 6-3 要檢查迴圈方塊本身的所有輸出是否已經完成 否則繼續執行
				if((BL->bap->ONodeNUM != 0) && (BL->bap->ONodeNUM != -1))
				{
					handle=true;
				}	

/*
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{	
							if(ionode->NotReady==true)
							{
								handle=true;
							}
						}
						
					ionode=ionode->NEXT;
					}
*/
					// 6-1 將所有輸出點資料整理 送至 HOOKBLOCK 的輸出 IO點
				/*	
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{
							if(ionode->NotReady==true)
							{
								// 找出有連到這個點的線段
								WL=((HOOKP)BL->hookP)->WireLLHeadP;
								while(WL!=NULL)
								{
									if(WL->StartNodeP == ionode)
									{
										if(WL->EndNodeP->NotReady==false)
										{
											(*(int *)ionode->P)=(*(int *)WL->EndNodeP->P);
											ionode->NotReady=false;
										}else{
											handle=true;
										}
										break;
									}
									if(WL->EndNodeP == ionode)
									{
										if(WL->StartNodeP->NotReady==false)
										{
											(*(int *)ionode->P)=(*(int *)WL->StartNodeP->P);
											ionode->NotReady=false;
										}else{
											handle=true;
										}
										break;
									}
									WL=WL->NEXTNODE;
								}
							}
							ionode->NotReady=false;
						}
						
					ionode=ionode->NEXT;
					}
				*/	
				} // DoRUN / else
				

				}// if inputready
				
	return handle;
}
Boolean DoRun_LOOPBLOCK(BNP BL)
{
	IONODEP ionode;
	Boolean		INPUTREADY = true;
	Boolean	OUTPUTNOTYET = false;
	Boolean		HAVENOOUTPUT = true;
	//Boolean ALLOUTREADY;
	//int BITMAPID;
	Boolean handle=false;
	int swap_i;
				/*
				// for debug use
				if(BL->bap->BITMAPID == FORLOOPBitmap)
					BITMAPID=BL->bap->BITMAPID;
				if(BL->bap->BITMAPID == WHILELOOPBitmap)
					BITMAPID=BL->bap->BITMAPID;
				*/
				// 5-17 while 迴圈採用 內部預設一個元件 
				// 這個元件是用來決定這個迴圈是否迴圈執行
				//
				/*
		    	ionode = BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{
					//if((BL->bap->IONodes[i].DATAnotREADY == true) && 
					//(BL->bap->IONodes[i].IO == true)) // check input node's data status
					if((ionode->NotReady == true) && 
					(ionode->TYPE == true)) // check input node's data status
					{
						INPUTREADY = false;
						//*handle = true;
					}
					
					ionode=ionode->NEXT;
				}
				*/
					/////////////////////////////////
					// 6-3 
					// 改版 輸入備妥 測試方法
					//
					/////////////////////////////////
				if((BL->bap->INodeNUM != 0)&&(BL->bap->INodeNUM != -1))
				{
						INPUTREADY = false;
						handle = true;
				}
				
				// 6-2 加入判斷 輸出是否已完成 如果是 則不用再進去執行了
				//     沒輸出節點 則是屬於一定要執行
				//     有輸出節點 但有未完成 就屬於要執行
				//     有輸出節點 但全已執行 就不用再進入
				/*
		    	ionode = BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{   
					if(ionode->TYPE == 0)
					{
						HAVENOOUTPUT = false;
						if(ionode->NotReady == true) // check output node's data status
						{
							OUTPUTNOTYET = true;
						}
					}
					
					ionode=ionode->NEXT;
				}
				*/
				
				//if(INPUTREADY && (HAVENOOUTPUT?true:OUTPUTNOTYET))
				if(INPUTREADY && ((BL->bap->ONodeNUM == -1)?true:(BL->bap->ONodeNUM != 0)))
				{
				//
				// 5-18 迴圈邊上的IO點 在這裡動態切換 因為進到裡面之後 角色變換
				//
				swap_i=BL->bap->ONodeNUM;
				BL->bap->ONodeNUM=BL->bap->INodeNUM;
				BL->bap->INodeNUM=swap_i;
				
		    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
					if(ionode->TYPE == 1)
					{
						ionode->TYPE = 0;
					}
					else
					{
						ionode->TYPE = 1;
					}
						
					ionode=ionode->NEXT;
					}
			
				//handle=
				if(DoRun(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,((HOOKP)BL->LoopBlockHOOKP)->WireLLHeadP))
				{
					//
					// 5-18 迴圈邊上的IO點 在這裡動態切回來
					//
				swap_i=BL->bap->ONodeNUM;
				BL->bap->ONodeNUM=BL->bap->INodeNUM;
				BL->bap->INodeNUM=swap_i;

			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
					if(ionode->TYPE == 1)
					{
						ionode->TYPE = 0;
					}
					else
					{
						ionode->TYPE = 1;
					}
						
					ionode=ionode->NEXT;
					}

					// 5-18 內部未完成 強制 handle 為 true
					handle=true;
					
					// 5-18 動畫內部未完成 輸出未完成
					
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{
							ionode->NotReady=true;
						}
						
					ionode=ionode->NEXT;
					}
					
				}
				else
				{	// 已完成 內部運算
					//
					// 5-18 迴圈邊上的IO點 在這裡動態切回來
					//
				swap_i=BL->bap->ONodeNUM;
				BL->bap->ONodeNUM=BL->bap->INodeNUM;
				BL->bap->INodeNUM=swap_i;

			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
					if(ionode->TYPE == 1)
					{
						ionode->TYPE = 0;
					}
					else
					{
						ionode->TYPE = 1;
					}
						
					ionode=ionode->NEXT;
					}

					// 6-3 要檢查迴圈方塊本身的所有輸出是否已經完成 否則繼續執行
					//ALLOUTREADY=true;
				if((BL->bap->ONodeNUM != 0) && (BL->bap->ONodeNUM != -1))
				{
					handle=true;
				}	
				/*
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{	
							if(ionode->NotReady==true)
							{
								handle=true;
								//ALLOUTREADY=false;
							}
						}
						
					ionode=ionode->NEXT;
					}
				*/	
					// 6-3 所有輸出都已完成  則使 全部輸出重置狀態
					/*
					if(ALLOUTREADY )
					{
				    	ionode = BL->bap->IONodeLLHead;
						while(ionode != NULL)
						{
							if(ionode->TYPE == 0)
							{	
								ionode->NotReady=true;
							}
							
						ionode=ionode->NEXT;
						}
					}
					*/
					// 5-18 動畫內部已完成 輸出也完成
					/*
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{
							ionode->NotReady=false;
						}
						
					ionode=ionode->NEXT;
					}
					*/
					// 6-3 剩下本身的輸出點 未完成 則 不執行迴圈條件判斷
					if(handle==false)
					{
						/////////////////////////////////
						// 迴圈執行邏輯
						//
						// 內部執行完畢 但是 迴圈控制呢？
						// 5-30 while  迴圈 與 for 迴圈 在這裡不同
						switch(BL->bap->BITMAPID)
						{
						case WHILELOOPBitmap:
						if(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->bap->IONodeLLHead->NotReady == false)
							if(*(int *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->bap->IONodeLLHead->P)==true)
							{// 重置狀態
								handle=true;
								// 元件 IO 點 重置
								CheckConnection(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,false);
								// 線段 狀態重置
								ResetWireStatus(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,
								((HOOKP)BL->LoopBlockHOOKP)->WireLLHeadP);
								
								
								//BL->bap->ONodeNUM
								//5-18 如果繼續執行 則需要將輸出設定為未完成
						    	ionode = BL->bap->IONodeLLHead;
								while(ionode != NULL)
								{
									if(ionode->TYPE == 0)
									{
										BL->bap->ONodeNUM++;
										ionode->NotReady=true;
									}
								
									ionode=ionode->NEXT;
								}
								
								
							}
						break;
						case FORLOOPBitmap:
						if(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->bap->IONodeLLHead->NotReady == false)
						{
							//MemMove(&N
							//memMove(&I
							if(
							*((long *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->bap->IONodeLLHead->P)) !=
							*((long *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P))
							)
							{// if 兩值不等
								handle=true;
								// 元件 IO 點 重置
								CheckConnection(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,false);
								// 線段 狀態重置
								ResetWireStatus(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,
								((HOOKP)BL->LoopBlockHOOKP)->WireLLHeadP);
								
								
								//5-18 如果繼續執行 則需要將輸出設定為未完成
						    	ionode = BL->bap->IONodeLLHead;
								while(ionode != NULL)
								{
									if(ionode->TYPE == 0)
									{
										BL->bap->ONodeNUM++;
										ionode->NotReady=true;
									}
								
									ionode=ionode->NEXT;
								}
								
								
					// 5-30 設定 forloop 的 i 的值
					// N != I then I++
					(*((long *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P)))++;
//	(*(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P))=(int)(
//	(int)(*(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P)) +
//	(int)1);
	//MemMove(&ivalue,(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P),2);
	//ivalue++;
	//MemMove((((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P),
	//&ivalue,2);

							}// if 兩值不等
							else
							{// 兩值相等 清除 I 值  迴圈結束
	(*((long *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P)))=0;
							}
						}
						break;
						}// switch-case
						
						}// 迴圈內部 最後一步
				} // DoRUN / else
				

					//if(handle==false)
					//{
					//}
				}// if inputready
				
				
	return handle;
}
void DoRun_BlockRun(BNP BlockLLHeadP,Boolean *handle)
{
	BNP BL;//,startblock,endblock;
	Boolean INPUTREADY;
	Boolean OUTPUTNOTYET;
	Boolean HAVENOOUTPUT;
	//IONODEP ionode;
	//
	// 6-2 判斷元件是否可以執行
	//     依照下列規則
	//	   輸入必須全部完成 全部輸入呈現AND關係 符合條件 結果為真
	//     輸出只要有一個未完成 則必須繼續 全部輸出有OR關係 符合條件 結果為假
	//     目的 
	//     進去前 必定 全部輸入完成 且 至少有一個輸出未完成
	//     出來後 輸出可能會改變 下一次來時 再來判斷

		BL = BlockLLHeadP;
		while(BL!=NULL)
		{
			INPUTREADY = true;
			OUTPUTNOTYET = false;
			HAVENOOUTPUT = true;
			//if((BL->bap->IONodes[i].DATAnotREADY == true) && 
			//		(BL->bap->IONodes[i].IO == false)) // check output data ready?
		    //{
			switch(BL->TYPE)
			{	
				////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////
				// 自訂方塊 處理
				//
				case HOOKBLOCK:
					if(DoRun_HOOKBLOCK(BL))
					{
						*handle=true;
					}else{
						if(*handle)
						{
							*handle=true;
						}else{
							*handle=false;
						}
					}
				break;
				////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////
				// 迴圈執行 處理
				//
				case LOOPBLOCK:
					if(DoRun_LOOPBLOCK(BL))
					{
						*handle=true;
					}else{
						if(*handle)
						{
							*handle=true;
						}else{
							*handle=false;
						}
					}
					//*handle = DoRun_LOOPBLOCK(BL) || (*handle );
				break;
				////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////
				//一般元件處理
				//
				default :
				/*
		    	ionode = BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{
					//if((BL->bap->IONodes[i].DATAnotREADY == true) && 
					//(BL->bap->IONodes[i].IO == true)) // check input node's data status
					
					if((ionode->NotReady == true) && 
					(ionode->TYPE == 1)) // check input node's data status
					{
						INPUTREADY = false;
						*handle = true;
					}
					
					ionode=ionode->NEXT;
				}
				*/
					/////////////////////////////////
					// 6-3 
					// 改版 輸入備妥 測試方法
					//
					/////////////////////////////////
				if((BL->bap->INodeNUM != 0)&&(BL->bap->INodeNUM != -1))
				{
						INPUTREADY = false;
						*handle = true;
				}
				// 6-2 加入判斷 輸出是否已完成 如果是 則不用再進去執行了
				//     沒輸出節點 則是屬於一定要執行
				//     有輸出節點 但有未完成 就屬於要執行
				//     有輸出節點 但全已執行 就不用再進入
					/////////////////////////////////
					// 6-3 
					// 改版 輸出備妥 測試方法
					//
					/////////////////////////////////
				/*
		    	ionode = BL->bap->IONodeLLHead;
				while(ionode != NULL)
				{   
					if(ionode->TYPE == 0)
					{
						HAVENOOUTPUT = false;
						if(ionode->NotReady == true) // check output node's data status
						{
							OUTPUTNOTYET = true;
						}
					}
					
					ionode=ionode->NEXT;
				}
				*/
				if(INPUTREADY && ((BL->bap->ONodeNUM == -1)?true:(BL->bap->ONodeNUM != 0)))
				{
					DoItemRUN(BL);
				}

				break;
			}// switch-case
			//}
			BL = BL->NEXTNODE;
		}//while
}
/***********************************************************************
 * 2003-4-12
 * 2003-5-15 改變執行結構 這裡面只執行一次 
 * 2003-5-15 要把 block 與 wire 分成兩個函式 才能正常顯示run point
 * 2003-5-16 可是 wire 是緊接著 block 才行
 *           因為 DoItemRun 中 回遞迴到 DoRun 中 這時的wirellhead 會是新的
 * 2003-6-2  大整理 詳細說明.....重點
 *				1.動畫部分 先動畫 動畫完成後 才會進行處理線段資料搬移
 *				2.把一些程式區塊 再分成幾個副程式
 *				3.簡單的對執行速度作最佳化 ：已經做過的方塊 不會重複執行
 *
 ***********************************************************************/
Boolean DoRun(BNP BlockLLHeadP,WNP WireLLHeadP)
{

	Boolean handle;
	int debug_poing;
	//int i;
	//int ivalue;
	//long N,I;
	//WNP WL;
	
	//Boolean ItIsCUSTOMBLOCK;
	
	//CheckConnection(BlockLLHeadP);//5-15 移到外面
	//do{
		handle = false;
		
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	if(LAMP)
	{
		if(DrawRunPointOnWire(WireLLHeadP,true))
		{
			DrawWires(WireLLHeadP,true,false);
			handle = true;
		}else{
	//
	// 除錯用 中斷點
	///////////////////////////////////////////////////////////////////////////	//
	/*
	switch(BlockLLHeadP->SelfBlockLLHead->TYPE)
	{
		case HOOKBLOCK:
			debug_poing=BlockLLHeadP->SelfBlockLLHead->TYPE;
		break;
		case LOOPBLOCK:
			if(BlockLLHeadP->SelfBlockLLHead->bap->BITMAPID==FORLOOPBitmap)
			{
				debug_poing=BlockLLHeadP->SelfBlockLLHead->TYPE;
					// 6-4
					//FrmCustomAlert(MESSAGEAlert,"ForLoop is finish.","","");

			}
			if(BlockLLHeadP->SelfBlockLLHead->bap->BITMAPID==WHILELOOPBitmap)
			{
				debug_poing=BlockLLHeadP->SelfBlockLLHead->TYPE;
					// 6-4
					//FrmCustomAlert(MESSAGEAlert,"WhileLoop is finish.","","");

			}
		break;
		case CASEBLOCK:
			debug_poing=BlockLLHeadP->SelfBlockLLHead->TYPE;
		break;
		default:
			debug_poing=BlockLLHeadP->SelfBlockLLHead->TYPE;
		break;
		
	}
	*/
	////////////////////////////////////////////////////////////////////////////
			DoRun_WireRun(WireLLHeadP);
			DrawWires(WireLLHeadP,true,false);//更新訊息
	   		DrawRunPointOnWire(WireLLHeadP,false);//顯示終IO點數值
			DoRun_BlockRun(BlockLLHeadP,&handle);
			DrawWires(WireLLHeadP,true,false);// 更新訊息
		}
		
	
	
	
	}else{
		DoRun_WireRun(WireLLHeadP);
	
		DoRun_BlockRun(BlockLLHeadP,&handle);
	}
	
	
	
	
	
	
	

		// 5-16 這裡畫動畫 然後決定下面是否繼續
		/*
		DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,true,1,0,0,true);
	// 畫動畫
	// DrawIcon(WIRERUNPOINTBitmap,startx,i);
	if(Animation)
	{
		while(DrawRunPointOnWire(wllpH,true))
		{
				DrawWires(wllpH,show,false);
			//DrawRunPointOnWire(wllpH,false);
		}
	}
	*/
	
	/*
	// 5-31 加入判斷 是 HOOKBLOCK 及 不是 SYSHOOK 則 不動畫
	if(BlockLLHeadP!=NULL)
	ItIsCUSTOMBLOCK=(BlockLLHeadP->SelfBlockLLHead->TYPE == HOOKBLOCK)&&(BlockLLHeadP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP);
		if((ItIsCUSTOMBLOCK)?false:((LAMP)?DrawRunPointOnWire(WireLLHeadP,true):false))
		{
			// 畫線 不清除變數
			DrawWires(WireLLHeadP,true,false);
			handle = true;

		}else{
		
		// 線段動畫完成
	
		
		// 進行線段資料搬移
		DoRun_WireRun(WireLLHeadP);

		// 顯示線段終點 資料值
		
		if(ItIsCUSTOMBLOCK)
		{
		}else{
		   if(LAMP)
		   {
		   		DrawRunPointOnWire(WireLLHeadP,false);
		   }
		}


		DoRun_BlockRun(BlockLLHeadP,&handle);
		
		
		
		}// if 動畫
		
	*/
	//}while(handle);
	











	// 5-18
	// 線段執行過  Dirty 會設為 ture
	// 這裡要reset 它
	// 如果執行一次結束
	// 清除線段 dirty bit
	/*
	if(handle==false)
	{
		WL = WireLLHeadP;
		while(WL!=NULL)
		{
			WL->DIRTY=false;
			WL = WL->NEXTNODE;
		}
	}
	*/
	//
	// restore ICON node's data
	//
	return handle;
}
