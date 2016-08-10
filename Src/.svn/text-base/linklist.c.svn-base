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
extern WNP WLLP;
extern HOOKP SYSHOOK;	// 程式主串列的頭
extern EndPoint WireStart,WireStop;
extern UID;
extern BNP BlockToolsLLHeadP;
extern BNP PanelToolsLLHeadP;
extern UInt16 BlockPointerState;// = HANDBitmap; // block : HAND , THREAD
extern UInt16 PanelPointerState; // panel : HAND , FINGER
/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/

/***********************************************************************
 * Link List rotuine
 ***********************************************************************/

WNP allocate_a_new_wire_node()
{
	int i;
	char *p;

	p = MemHandleLock(MemHandleNew(sizeof(WN)));
	for(i=0;i<sizeof(WN);i++)
		*(p+i) = 0;

	return (WNP)p;
}
 BNP allocate_a_new_node()
{
	int i;
	char *p;
	p = MemHandleLock(MemHandleNew(sizeof(BN)));
	for(i=0;i<sizeof(BN);i++)
	{
		*(p+i) = 0;
	 //MemMove((p+i),&j,1);
	}
	return (BNP)p;
}
IONODEP allocate_a_new_IOnode()
{
	int i;
	Ptr p;

	p = MemHandleLock(MemHandleNew(sizeof(struct IONODE)));
	for(i=0;i<sizeof(struct IONODE);i++)
		*(p+i) = 0;
	
	return (IONODEP)p;
}
CTRLNODEP allocate_a_new_CTRLnode()
{
	int i;
	Ptr p;

	p = MemHandleLock(MemHandleNew(sizeof(struct CTRLNODE)));
	for(i=0;i<sizeof(struct CTRLNODE);i++)
		*(p+i) = 0;
	
	return (CTRLNODEP)p;
}
BAP allocate_a_new_bap()
{
	int i;
	Ptr p;
	
	p = MemHandleLock(MemHandleNew(sizeof(BA)));
	for(i=0;i<sizeof(BA);i++)
		*(p+i) = 0;
	
	return (BAP)p;
}
PAP allocate_a_new_pap()
{
	int i;
	Ptr p;

	p = MemHandleLock(MemHandleNew(sizeof(PA)));
	for(i=0;i<sizeof(PA);i++)
		*(p+i) = 0;
	
	return (PAP)p;
}
HOOKP allocate_a_new_hook()
{
	int i;
	Ptr p;
	p = MemHandleLock(MemHandleNew(sizeof(struct HOOK)));
	for(i=0;i<sizeof(struct HOOK);i++)
		*(p+i) = 0;
	
	return (HOOKP)p;
}
// ********************************************************************************
// * 插入新Block
// *
// * 4-25 畫面上出現的元件都需要唯一碼
// *
// *
// ********************************************************************************
/*
 BNP InsertIntoBlockLL(BNP bLLP,BNP node,Boolean generate_new_ID)
{
	UInt16 IDn = 0; //UID is for wire connection use
	BNP tempLLP;
	//BNP bLLP ;
	//bLLP = sysHead.BlockLLHead;

	if(bLLP != SYSHOOK->BlockLLHeadP)
	{// 不是系統頭，找出最大UID值
		tempLLP = SYSHOOK->BlockLLHeadP;
		while(tempLLP != )
		{
		}
	}
	if(bLLP == NULL)
	{// 如果是第一筆資料
		bLLP = node ;
		bLLP->PREVNODE = NULL;
		if(generate_new_ID)
		{
			bLLP->bap->UID = 1;
		}
		return bLLP;
	}
	
	 while(true)
	 {
		if((bLLP->bap->UID > IDn) && generate_new_ID)
		{// 需要產生ID
			IDn = bLLP->bap->UID; // generate the uID
		}
		if(bLLP->NEXTNODE == NULL)
		{
			break;
		}
		bLLP = bLLP->NEXTNODE;
	 }
	 if(generate_new_ID)
	 {
	 	node->bap->UID = IDn + 1;
	 }
	 
	bLLP->NEXTNODE = node;
	node->PREVNODE = bLLP;
	
	while(bLLP->PREVNODE != NULL)
		bLLP = bLLP->PREVNODE;
		
	return bLLP;
}
*/
// ********************************************************************************
// * 插入新Block
// *
// * 4-25 畫面上出現的元件都需要唯一碼，分成兩個function
// *
// *
// ********************************************************************************
 BNP InsertIntoBlockLLwithUID(BNP bLLP,BNP node)
{
	// UID

	UID ++;
		
	if(bLLP == NULL)
	{// 如果是第一筆資料
		bLLP = node ;
		bLLP->PREVNODE = NULL;

			bLLP->ID = UID;
		
		return bLLP;
	}
	
	 while(bLLP->NEXTNODE != NULL)
	 {// 走到最後一筆 
	 	bLLP = bLLP->NEXTNODE;
	 }
	
	node->ID = UID;
	bLLP->NEXTNODE = node;
	node->PREVNODE = bLLP;
	
	// 走到頭筆
	while(bLLP->PREVNODE != NULL)
		bLLP = bLLP->PREVNODE;
		
	return bLLP;
}

// ********************************************************************************
// * 插入新Block
// *
// * 4-25 畫面上出現的元件都需要唯一碼，分成兩個function
// *
// *
// ********************************************************************************
BNP InsertIntoBlockLLNoUID(BNP bLLP,BNP node)
{
	//bLLP = sysHead.BlockLLHead;
	// 6-1 確保都有唯一碼 
	if((node->ID)>UID)
		UID=node->ID;
		
	if(bLLP == NULL)
	{// 如果是第一筆資料
		bLLP = node ;
		bLLP->PREVNODE = NULL;
		return bLLP;
	}
	
	 while(bLLP->NEXTNODE != NULL)
	 {// 走到最後一筆 
	 	bLLP = bLLP->NEXTNODE;
	 }
	 
	bLLP->NEXTNODE = node;
	node->PREVNODE = bLLP;
	
	// 回到頭位址
	while(bLLP->PREVNODE != NULL)
		bLLP = bLLP->PREVNODE;
		
	return bLLP;
}
WNP InsertIntoWireLL(WNP Head,WNP node)
{
	//WNP wLLP;
	//wLLP = sysHead.WireLLHead;
	
	node->DIRTY=false; // 5-15 動畫用
	node->STARTRUN=false; // 5-15 動畫用
	node->RUNSEG=0;
	// condition 1
	if(Head == NULL)
	{
		Head = node ;
		Head->PREVNODE = NULL;
   		//sysHead.BlockLLHead->WireLLHeadP = sysHead.WireLLHead;//ver.2 >>>BLLHead<<<

		return Head;
	}
	
	// condition 2
	while(true)
	{
		if(Head->NEXTNODE == NULL)
			break;
		Head = Head->NEXTNODE;
	}
	
	Head->NEXTNODE = node;
	node->PREVNODE = Head;
	
	//4-25 回到開頭
	while(Head->PREVNODE != NULL)
	{
		Head = Head->PREVNODE;
	}
	
	return Head;
}

IONODEP InsertIntoIONodesLL(IONODEP IONodeH,IONODEP node )
{
	if(IONodeH == NULL)
	{
		IONodeH = node;
		return IONodeH;
	}
	
	while(IONodeH->NEXT != NULL)
	{
		
		IONodeH = IONodeH->NEXT;
	}
	
	IONodeH->NEXT = node;
	node->PREV = IONodeH;
	
	while(IONodeH->PREV != NULL)
	{
		IONodeH = IONodeH->PREV;
	}
	
	return IONodeH;
}
CTRLNODEP InsertIntoCTRLNodesLL(CTRLNODEP ctrlnodeH,CTRLNODEP ctrlnode )
{
	if(ctrlnodeH == NULL)
	{
		ctrlnodeH = ctrlnode;
		return ctrlnodeH;
	}
	
	while(ctrlnodeH->NEXT != NULL)
	{
		
		ctrlnodeH = ctrlnodeH->NEXT;
	}
	
	ctrlnodeH->NEXT = ctrlnode;
	ctrlnode->PREV = ctrlnodeH;
	
	while(ctrlnodeH->PREV != NULL)
	{
		ctrlnodeH = ctrlnodeH->PREV;
	}
	
	return ctrlnodeH;
}
HOOKP InsertHOOKNode(HOOKP sysHead)
{
	BAP bap;
	BNP newp;
	HOOKP hook;
		
		newp = allocate_a_new_node();

	bap = allocate_a_new_bap();
	bap->BITMAPID = NULL;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 0;
	bap->SIZE.y = 0; 
	newp->bap = bap;


		newp->TYPE=HOOKBLOCK;
   		newp->NEXTNODE = NULL;

   		hook = allocate_a_new_hook();
   		hook->BlockLLHeadP = newp;
   		hook->WireLLHeadP = NULL;
   		(HOOKP)newp->hookP = hook;

   		newp->SelfBlockLLHead = newp;
   		//newp->PrevBlockLLHeadP = NULL;
		sysHead->BlockLLHeadP = InsertIntoBlockLLwithUID(sysHead->BlockLLHeadP,newp);
		return sysHead;
}
HOOKP AddWireLinkListNode(HOOKP hookp,EndPoint Start,EndPoint Stop)
{
	WNP  newwireptr;
	
	newwireptr = allocate_a_new_wire_node();
	newwireptr->WireID = 0x0000;
	newwireptr->StartBlockP = Start.BlockP;
//	(BNP)newwireptr->wire.StartBlockP = Start.BlockP;
	newwireptr->StartNodeP = Start.NodeP;
	newwireptr->EndBlockP = Stop.BlockP;
//	(BNP)newwireptr->wire.EndBlockP = Stop.BlockP;
	newwireptr->EndNodeP = Stop.NodeP;
	newwireptr->NEXTNODE = NULL;
	hookp->WireLLHeadP = InsertIntoWireLL(hookp->WireLLHeadP,newwireptr);

	return hookp;
}

//
// 可遞迴
//
 BNP RemoveBlockLinkList(BNP bllpH,BNP  WhichBlockPtr)
{
	if((WhichBlockPtr->PREVNODE == NULL) && (WhichBlockPtr->NEXTNODE == NULL)){
		bllpH = NULL;
	}else if((WhichBlockPtr->PREVNODE != NULL) && (WhichBlockPtr->NEXTNODE == NULL)){
		WhichBlockPtr->PREVNODE->NEXTNODE = NULL;
	}else if((WhichBlockPtr->PREVNODE == NULL) && (WhichBlockPtr->NEXTNODE != NULL)){
		bllpH = WhichBlockPtr->NEXTNODE;
		WhichBlockPtr->NEXTNODE->PREVNODE = NULL;
	}else if((WhichBlockPtr->PREVNODE != NULL) && (WhichBlockPtr->NEXTNODE != NULL)){
		WhichBlockPtr->PREVNODE->NEXTNODE = WhichBlockPtr->NEXTNODE;
		WhichBlockPtr->NEXTNODE->PREVNODE = WhichBlockPtr->PREVNODE;
	}
	MemHandleUnlock(MemPtrRecoverHandle(WhichBlockPtr));
	MemHandleFree(MemPtrRecoverHandle(WhichBlockPtr));

	
	return bllpH;
}
//
// 可遞迴
//
WNP RemoveWireLinkList(WNP WireLLHead,WNP  wireptr)
{
	if((wireptr->PREVNODE == NULL) && (wireptr->NEXTNODE == NULL)){
		WireLLHead = NULL;
   		//BlockLLHead->WireLLHeadP = WireLLHead;//ver.2

	}else if((wireptr->PREVNODE != NULL) && (wireptr->NEXTNODE == NULL)){
		wireptr->PREVNODE->NEXTNODE = NULL;
	}else if((wireptr->PREVNODE == NULL) && (wireptr->NEXTNODE != NULL)){
		WireLLHead = wireptr->NEXTNODE;
   		//BlockLLHead->WireLLHeadP = WireLLHead;//ver.2

		wireptr->NEXTNODE->PREVNODE = NULL;
	}else if((wireptr->PREVNODE != NULL) && (wireptr->NEXTNODE != NULL)){
		wireptr->PREVNODE->NEXTNODE = wireptr->NEXTNODE;
		wireptr->NEXTNODE->PREVNODE = wireptr->PREVNODE;
	}
	/*
	if(wireptr->PREVNODE == NULL)
		wireptr->NEXTNODE->PREVNODE = NULL;
	else
		wireptr->NEXTNODE->PREVNODE = wireptr->PREVNODE;
	if(wireptr->NEXTNODE == NULL)
		wireptr->PREVNODE->NEXTNODE = NULL;
	else
		wireptr->PREVNODE->NEXTNODE = wireptr->NEXTNODE;
	*/	
	MemHandleUnlock(MemPtrRecoverHandle(wireptr));
	MemHandleFree(MemPtrRecoverHandle(wireptr));
	
	return WireLLHead;
}
IONODEP RemoveIONodeLinkList(IONODEP IONodeHead,IONODEP  ionode)
{
	if((ionode->PREV == NULL) && (ionode->NEXT == NULL)){
		IONodeHead = NULL;

	}else if((ionode->PREV != NULL) && (ionode->NEXT == NULL)){
		ionode->PREV->NEXT = NULL;
	}else if((ionode->PREV == NULL) && (ionode->NEXT != NULL)){
		IONodeHead = ionode->NEXT;

		ionode->NEXT->PREV = NULL;
	}else if((ionode->PREV != NULL) && (ionode->NEXT != NULL)){
		ionode->PREV->NEXT = ionode->NEXT;
		ionode->NEXT->PREV = ionode->PREV;
	}
	MemHandleUnlock(MemPtrRecoverHandle(ionode));
	MemHandleFree(MemPtrRecoverHandle(ionode));
	
	return IONodeHead;
}
void ReleaseHOOKNode(HOOKP syshook)
{
	MemHandleUnlock(MemPtrRecoverHandle(syshook->BlockLLHeadP->bap));
	MemHandleFree(MemPtrRecoverHandle(syshook->BlockLLHeadP->bap));
	MemHandleUnlock(MemPtrRecoverHandle(syshook->BlockLLHeadP->hookP));
	MemHandleFree(MemPtrRecoverHandle(syshook->BlockLLHeadP->hookP));
	MemHandleUnlock(MemPtrRecoverHandle(syshook->BlockLLHeadP));
	MemHandleFree(MemPtrRecoverHandle(syshook->BlockLLHeadP));
	syshook->BlockLLHeadP=NULL;
}
void ReleaseLLMemPtr(BNP llhead)
{
	BNP bnp;
	IONODEP ionode;
	IONODEP ionodeT;
	CTRLNODEP ctrlnode;
	CTRLNODEP ctrlnodeT;
	HOOKP hook;
	HOOKP hookt;
	
	while(llhead != NULL)
	{
		bnp=llhead;
		llhead=llhead->NEXTNODE;
		if(bnp->bap != NULL)
		{
			if(bnp->bap->IONodeLLHead != NULL)
			{
				ionode=bnp->bap->IONodeLLHead;
				while(ionode!=NULL)
				{
					ionodeT=ionode;
					ionode=ionode->NEXT;
					if(ionodeT->P != NULL)
					{
						MemHandleUnlock(MemPtrRecoverHandle(ionodeT->P));
						MemHandleFree(MemPtrRecoverHandle(ionodeT->P));
					}
					MemHandleUnlock(MemPtrRecoverHandle(ionodeT));
					MemHandleFree(MemPtrRecoverHandle(ionodeT));
				}
			}
			if(bnp->bap->CTRLNodeLLHead != NULL)
			{
				ctrlnode=bnp->bap->CTRLNodeLLHead;
				while(ctrlnode!=NULL)
				{
					ctrlnodeT=ctrlnode;
					ctrlnode=ctrlnode->NEXT;
					MemHandleUnlock(MemPtrRecoverHandle(ctrlnodeT));
					MemHandleFree(MemPtrRecoverHandle(ctrlnodeT));
				}
			}
			MemHandleUnlock(MemPtrRecoverHandle(bnp->bap));
			MemHandleFree(MemPtrRecoverHandle(bnp->bap));
		}
		if(bnp->pap!=NULL)
		{
			if(bnp->pap->CTRLNodeLLHead != NULL)
			{
				ctrlnode=bnp->pap->CTRLNodeLLHead;
				while(ctrlnode!=NULL)
				{
					ctrlnodeT=ctrlnode;
					ctrlnode=ctrlnode->NEXT;
					MemHandleUnlock(MemPtrRecoverHandle(ctrlnodeT));
					MemHandleFree(MemPtrRecoverHandle(ctrlnodeT));
				}
			}
			MemHandleUnlock(MemPtrRecoverHandle(bnp->pap));
			MemHandleFree(MemPtrRecoverHandle(bnp->pap));
		}
		if(bnp->LoopBlockHOOKP!=NULL)
		{
			MemHandleUnlock(MemPtrRecoverHandle(bnp->LoopBlockHOOKP));
			MemHandleFree(MemPtrRecoverHandle(bnp->LoopBlockHOOKP));
		}
		/*
		if(bnp->CurrentHOOKP!=NULL)
		{
			MemHandleUnlock(MemPtrRecoverHandle(bnp->CurrentHOOKP));
			MemHandleFree(MemPtrRecoverHandle(bnp->CurrentHOOKP));
		}
		*/
		/*
		if(bnp->CaseBlockHOOKLLHeadP!=NULL)
		{
			//hook=bnp->CaseBlockHOOKLLHeadP;
			MemHandleUnlock(MemPtrRecoverHandle(bnp->CaseBlockHOOKLLHeadP));
			MemHandleFree(MemPtrRecoverHandle(bnp->CaseBlockHOOKLLHeadP));
		}
		*/
		if(bnp->hookP!=NULL)
		{
			hook=(HOOKP)bnp->hookP;
			while(hook!=NULL)
			{
				hookt=hook;
				hook=hook->NEXT;
				
				ReleaseLLMemPtr(hookt->BlockLLHeadP);
				MemHandleUnlock(MemPtrRecoverHandle(hookt));
				MemHandleFree(MemPtrRecoverHandle(hookt));
			}
		}
		MemHandleUnlock(MemPtrRecoverHandle(bnp));
		MemHandleFree(MemPtrRecoverHandle(bnp));
	}
}

//
// 4/13 加入可遞迴 修
//
// 6-1 改版

void NEW(void )
{
	UID=0;
	ReleaseLLMemPtr(SYSHOOK->BlockLLHeadP->NEXTNODE);
	ReleaseHOOKNode(SYSHOOK);
	SYSHOOK = allocate_a_new_hook();
	SYSHOOK->BlockLLHeadP = NULL;
	SYSHOOK->WireLLHeadP = NULL;
	SYSHOOK = InsertHOOKNode(SYSHOOK);
}

Boolean MatchThread(EndPoint EP,WNP WL)
{
	if((EP.BlockP == WL->StartBlockP) && (EP.NodeP == WL->StartNodeP))
		return true;
	else if((EP.BlockP == WL->EndBlockP) && (EP.NodeP == WL->EndNodeP))
		return true;
	else
		return false;
}
// 
// InWNPList
//
// notice the WireLLHead status! NULL or not.
//
WNP InWireLinkList(HOOKP hook,EndPoint start,EndPoint end)
{
	WNP WLP;
	Boolean thread1=false,thread2=false;
	WNP WLLP = hook->WireLLHeadP;
	WLP = NULL;
	do
	{
	   if(start.BlockP != end.BlockP)
	   {
	   /*
		thread1 = (
			((start.BlockP == WLLP->wire.StartBlockID) && (start.Node == WLLP->wire.StartNodeID))
			||
			((start.BlockP == WLLP->wire.EndBlockID) && (start.Node == WLLP->wire.EndNodeID))
		);
		thread2 = (
			((end.BlockP == WLLP->wire.StartBlockID) && (end.Node == WLLP->wire.StartNodeID))
			||
			((end.BlockP == WLLP->wire.EndBlockID) && (end.Node == WLLP->wire.EndNodeID))
		);
		*/
		thread1 = MatchThread(start,WLLP);
		thread2 = MatchThread(end,WLLP);
	    if((thread1 == true) && (thread2 == true))
	    {
		   	WLP = WLLP;
		    break;
	    }
	   }
		/*
		if(start.BlockP == WLLP->wire.StartBlockID)
		 if(start.Node == WLLP->wire.StartNodeID)
		  if(end.BlockP == WLLP->wire.EndBlockID)
		   if(end.Node == WLLP->wire.EndNodeID)
		   {
		   	WLP = WLLP;
		    break;
		   }
		*/
	}while((WLLP = WLLP->NEXTNODE));

	return WLP;
}
UInt16 CountIONODEs(IONODEP IO)
{
	UInt16 i=0;
	
	while(IO != NULL)
	{
		i++;
		IO = IO->NEXT;
	}
	
	return i;
}
/*
	5-6 加入 將bnp移到串列最後一個
*/
BNP ItemMoveToLastPosition(BNP BLLHead,BNP bnp)
{
	BNP bLLlast;
	
	bLLlast = BLLHead;
	// 找出串列的最後一個
	while(bLLlast->NEXTNODE != NULL)
	{
		bLLlast = bLLlast->NEXTNODE;
	}
	// 如果 bnp 已經是最後一個 不動作
	if(bnp != bLLlast)
	{
		// 將 bnp 的上下 node 接起來
		if(bnp->PREVNODE != NULL) // 如果 bnp 位在第一位
			bnp->PREVNODE->NEXTNODE = bnp->NEXTNODE;
		
		if(bnp->NEXTNODE != NULL)	
			bnp->NEXTNODE->PREVNODE = bnp->PREVNODE;
		
		// 將 bnp 接到最後一個位置
		bLLlast->NEXTNODE = bnp;
		bnp->PREVNODE = bLLlast;
		bnp->NEXTNODE = NULL;
	}
	// 回到頭
	while(bLLlast->PREVNODE != NULL)
	{
		bLLlast = bLLlast->PREVNODE;
	}
	
	return bLLlast;
}

void set_panel_tools_palette()
{
	BAP bap;
	BNP newp;
	
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = HANDBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	PanelToolsLLHeadP = InsertIntoBlockLLNoUID(PanelToolsLLHeadP,newp);
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = FINGERBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	PanelToolsLLHeadP = InsertIntoBlockLLNoUID(PanelToolsLLHeadP,newp);
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = PENCILBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	PanelToolsLLHeadP = InsertIntoBlockLLNoUID(PanelToolsLLHeadP,newp);

	PanelPointerState = HANDBitmap;
}
void set_block_tools_palette(void )
{
	BAP bap;
	BNP newp;
	
	/*
HANDBitmap)
DRAGBitmap)
THREADBitmap)
SCISSORBitmap)
KILLBitmap)
	*/
	
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = HANDBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	BlockToolsLLHeadP = InsertIntoBlockLLNoUID(BlockToolsLLHeadP,newp);
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = DRAGBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	BlockToolsLLHeadP = InsertIntoBlockLLNoUID(BlockToolsLLHeadP,newp);
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = THREADBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	BlockToolsLLHeadP = InsertIntoBlockLLNoUID(BlockToolsLLHeadP,newp);
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = SCISSORBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	BlockToolsLLHeadP = InsertIntoBlockLLNoUID(BlockToolsLLHeadP,newp);
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = KILLBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	BlockToolsLLHeadP = InsertIntoBlockLLNoUID(BlockToolsLLHeadP,newp);
	/********************************************************************/
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = PENCILBitmap;
	bap->XY.x = 0;
	bap->XY.y = 0;
	bap->SIZE.x = 16;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	//insert into linked list
	BlockToolsLLHeadP = InsertIntoBlockLLNoUID(BlockToolsLLHeadP,newp);

	BlockPointerState = HANDBitmap;
}
