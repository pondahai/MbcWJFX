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
 * 2003-5-17 �� �_��
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
				case CTRLU8BLOCKBitmap: // �o�� block �u���@��IO�I
				case LOGICCTRLBitmap:
				//case FORLOOPNBitmap: // 5-31 ����
				// �p�G���OSYSHOOK������ ����N�ӳ]�wnotready
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
					// 5-30 �b�o�̭� �|���Kreset forloop ��N ,I ��IO ���A
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
				// ICON node����Ө���
				// �W�߮ɬO��C���Y �B�z�ɭn���L
				// �Q�y�ۭq����zŪ�J�� �O�t�Φ�C���䤤�@��node
				// ������ɨS�����L���ʧ@�A�]��BlockLLHeadP �N�O�O���C�Ĥ@��node ���OICON
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
			// ��
			//startblock = (BNP)WL->wire.StartBlockP;
			//endblock   = (BNP)WL->wire.EndBlockP;
			startnode	=	WL->StartNodeP;
			endnode		=	WL->EndNodeP;
			// if one node is output then another node is input
			// ��
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
		  {//���L �t�u�q

			//startblock = FindItemByID(BlockLLHeadP,WL->wire.StartBlockID);
			//endblock   = FindItemByID(BlockLLHeadP,WL->wire.EndBlockID);
			// ��
			//startblock = (BNP)WL->wire.StartBlockP;
			//endblock   = (BNP)WL->wire.EndBlockP;
			startnode	=	WL->StartNodeP;
			endnode		=	WL->EndNodeP;
			// if one node is output then another node is input
			// ��
			//if((startblock->bap->IONodes[WL->wire.StartNodeID].DATAnotREADY == false) &&
			//	(startblock->bap->IONodes[WL->wire.StartNodeID].IO == false))//check output status
			
			// 5-18 �[�J���ݧP�_ �p�G���ݤ]�w�g ��ƳƧ� �N���n�h��
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
	
				// 5-17 while �j��ĥ� �����w�]�@�Ӥ��� 
				// �o�Ӥ���O�ΨӨM�w�o�Ӱj��O�_�j�����
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
					// �睊 ��J�Ƨ� ���դ�k
					//
					/////////////////////////////////
				if((BL->bap->INodeNUM != 0)&&(BL->bap->INodeNUM != -1))
				{
						INPUTREADY = false;
						handle = true;
				}
				
/*				
				// 6-2 �[�J�P�_ ��X�O�_�w���� �p�G�O �h���ΦA�i�h����F
				//     �p�G���@�ӥ����� �hNOTALLOK ���u
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

				// 6-3 ���J��ƳƧ�
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
				// 6-3 ��output wire ��DIRTY �o�˸̭���run wire �N���|����
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

				// 5-18 �j����W��IO�I �b�o�̰ʺA���� �]���i��̭����� �����ܴ�

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

					// 5-18 �j����W��IO�I �b�o�̰ʺA���^��
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
					// 5-18 ���������� �j�� handle �� true
					handle=true;
					
					// 5-18 �ʵe���������� ��X������
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
				{	// �w���� �����B��
					LAMP=LAMPSTATUS;

					// 5-18 �j����W��IO�I �b�o�̰ʺA���^��
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
					
					// 5-18 �ʵe�����w���� ��X�]����
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

				// 6-3 ���Ƨ�X��
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
			
					// 6-3 �n�ˬd�j�����������Ҧ���X�O�_�w�g���� �_�h�~�����
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
					// 6-1 �N�Ҧ���X�I��ƾ�z �e�� HOOKBLOCK ����X IO�I
				/*	
			    	ionode = BL->bap->IONodeLLHead;
					while(ionode != NULL)
					{
						if(ionode->TYPE == 0)
						{
							if(ionode->NotReady==true)
							{
								// ��X���s��o���I���u�q
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
				// 5-17 while �j��ĥ� �����w�]�@�Ӥ��� 
				// �o�Ӥ���O�ΨӨM�w�o�Ӱj��O�_�j�����
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
					// �睊 ��J�Ƨ� ���դ�k
					//
					/////////////////////////////////
				if((BL->bap->INodeNUM != 0)&&(BL->bap->INodeNUM != -1))
				{
						INPUTREADY = false;
						handle = true;
				}
				
				// 6-2 �[�J�P�_ ��X�O�_�w���� �p�G�O �h���ΦA�i�h����F
				//     �S��X�`�I �h�O�ݩ�@�w�n����
				//     ����X�`�I ���������� �N�ݩ�n����
				//     ����X�`�I �����w���� �N���ΦA�i�J
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
				// 5-18 �j����W��IO�I �b�o�̰ʺA���� �]���i��̭����� �����ܴ�
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
					// 5-18 �j����W��IO�I �b�o�̰ʺA���^��
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

					// 5-18 ���������� �j�� handle �� true
					handle=true;
					
					// 5-18 �ʵe���������� ��X������
					
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
				{	// �w���� �����B��
					//
					// 5-18 �j����W��IO�I �b�o�̰ʺA���^��
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

					// 6-3 �n�ˬd�j�����������Ҧ���X�O�_�w�g���� �_�h�~�����
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
					// 6-3 �Ҧ���X���w����  �h�� ������X���m���A
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
					// 5-18 �ʵe�����w���� ��X�]����
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
					// 6-3 �ѤU��������X�I ������ �h ������j�����P�_
					if(handle==false)
					{
						/////////////////////////////////
						// �j������޿�
						//
						// �������槹�� ���O �j�鱱��O�H
						// 5-30 while  �j�� �P for �j�� �b�o�̤��P
						switch(BL->bap->BITMAPID)
						{
						case WHILELOOPBitmap:
						if(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->bap->IONodeLLHead->NotReady == false)
							if(*(int *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->bap->IONodeLLHead->P)==true)
							{// ���m���A
								handle=true;
								// ���� IO �I ���m
								CheckConnection(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,false);
								// �u�q ���A���m
								ResetWireStatus(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,
								((HOOKP)BL->LoopBlockHOOKP)->WireLLHeadP);
								
								
								//BL->bap->ONodeNUM
								//5-18 �p�G�~����� �h�ݭn�N��X�]�w��������
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
							{// if ��Ȥ���
								handle=true;
								// ���� IO �I ���m
								CheckConnection(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,false);
								// �u�q ���A���m
								ResetWireStatus(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP,
								((HOOKP)BL->LoopBlockHOOKP)->WireLLHeadP);
								
								
								//5-18 �p�G�~����� �h�ݭn�N��X�]�w��������
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
								
								
					// 5-30 �]�w forloop �� i ����
					// N != I then I++
					(*((long *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P)))++;
//	(*(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P))=(int)(
//	(int)(*(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P)) +
//	(int)1);
	//MemMove(&ivalue,(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P),2);
	//ivalue++;
	//MemMove((((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P),
	//&ivalue,2);

							}// if ��Ȥ���
							else
							{// ��Ȭ۵� �M�� I ��  �j�鵲��
	(*((long *)(((HOOKP)BL->LoopBlockHOOKP)->BlockLLHeadP->NEXTNODE->bap->IONodeLLHead->P)))=0;
							}
						}
						break;
						}// switch-case
						
						}// �j�餺�� �̫�@�B
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
	// 6-2 �P�_����O�_�i�H����
	//     �̷ӤU�C�W�h
	//	   ��J������������ ������J�e�{AND���Y �ŦX���� ���G���u
	//     ��X�u�n���@�ӥ����� �h�����~�� ������X��OR���Y �ŦX���� ���G����
	//     �ت� 
	//     �i�h�e ���w ������J���� �B �ܤ֦��@�ӿ�X������
	//     �X�ӫ� ��X�i��|���� �U�@���Ӯ� �A�ӧP�_

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
				// �ۭq��� �B�z
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
				// �j����� �B�z
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
				//�@�뤸��B�z
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
					// �睊 ��J�Ƨ� ���դ�k
					//
					/////////////////////////////////
				if((BL->bap->INodeNUM != 0)&&(BL->bap->INodeNUM != -1))
				{
						INPUTREADY = false;
						*handle = true;
				}
				// 6-2 �[�J�P�_ ��X�O�_�w���� �p�G�O �h���ΦA�i�h����F
				//     �S��X�`�I �h�O�ݩ�@�w�n����
				//     ����X�`�I ���������� �N�ݩ�n����
				//     ����X�`�I �����w���� �N���ΦA�i�J
					/////////////////////////////////
					// 6-3 
					// �睊 ��X�Ƨ� ���դ�k
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
 * 2003-5-15 ���ܰ��浲�c �o�̭��u����@�� 
 * 2003-5-15 �n�� block �P wire ������Ө禡 �~�ॿ�`���run point
 * 2003-5-16 �i�O wire �O�򱵵� block �~��
 *           �]�� DoItemRun �� �^���j�� DoRun �� �o�ɪ�wirellhead �|�O�s��
 * 2003-6-2  �j��z �Բӻ���.....���I
 *				1.�ʵe���� ���ʵe �ʵe������ �~�|�i��B�z�u�q��Ʒh��
 *				2.��@�ǵ{���϶� �A�����X�ӰƵ{��
 *				3.²�檺�����t�ק@�̨Τ� �G�w�g���L����� ���|���ư���
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
	
	//CheckConnection(BlockLLHeadP);//5-15 ����~��
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
	// ������ ���_�I
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
			DrawWires(WireLLHeadP,true,false);//��s�T��
	   		DrawRunPointOnWire(WireLLHeadP,false);//��ܲ�IO�I�ƭ�
			DoRun_BlockRun(BlockLLHeadP,&handle);
			DrawWires(WireLLHeadP,true,false);// ��s�T��
		}
		
	
	
	
	}else{
		DoRun_WireRun(WireLLHeadP);
	
		DoRun_BlockRun(BlockLLHeadP,&handle);
	}
	
	
	
	
	
	
	

		// 5-16 �o�̵e�ʵe �M��M�w�U���O�_�~��
		/*
		DrawDiagram(SYSHOOK->BlockLLHeadP->NEXTNODE,((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP,true,1,0,0,true);
	// �e�ʵe
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
	// 5-31 �[�J�P�_ �O HOOKBLOCK �� ���O SYSHOOK �h ���ʵe
	if(BlockLLHeadP!=NULL)
	ItIsCUSTOMBLOCK=(BlockLLHeadP->SelfBlockLLHead->TYPE == HOOKBLOCK)&&(BlockLLHeadP->SelfBlockLLHead != SYSHOOK->BlockLLHeadP);
		if((ItIsCUSTOMBLOCK)?false:((LAMP)?DrawRunPointOnWire(WireLLHeadP,true):false))
		{
			// �e�u ���M���ܼ�
			DrawWires(WireLLHeadP,true,false);
			handle = true;

		}else{
		
		// �u�q�ʵe����
	
		
		// �i��u�q��Ʒh��
		DoRun_WireRun(WireLLHeadP);

		// ��ܽu�q���I ��ƭ�
		
		if(ItIsCUSTOMBLOCK)
		{
		}else{
		   if(LAMP)
		   {
		   		DrawRunPointOnWire(WireLLHeadP,false);
		   }
		}


		DoRun_BlockRun(BlockLLHeadP,&handle);
		
		
		
		}// if �ʵe
		
	*/
	//}while(handle);
	











	// 5-18
	// �u�q����L  Dirty �|�]�� ture
	// �o�̭nreset ��
	// �p�G����@������
	// �M���u�q dirty bit
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
