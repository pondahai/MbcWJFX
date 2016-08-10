#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"
#include "MemoryMgr.h"
#include "FileStream.h"

#include "vpl.h"

extern HOOKP SYSHOOK;
extern EndPoint WireStart,WireStop;

extern VoidPtr GetObjectPtr(Word objectID);
extern IONODEP allocate_a_new_IOnode();
extern IONODEP InsertIntoIONodesLL(IONODEP IONodeH,IONODEP node );
extern HOOKP AddWireLinkListNode(HOOKP hookp,EndPoint Start,EndPoint Stop);
extern WNP RemoveWireLinkList(WNP WireLLHead,WNP  wireptr);


void AddString(VoidHand strH,char *str)
{
	char *strP;

	MemHandleResize(strH,MemHandleSize(strH)+StrLen(str));			//調整大小
	strP=MemHandleLock(strH);								// 鎖
	StrNCat(strP,str,MemHandleSize(strH));					//串接
	MemHandleUnlock(strH);									// 解鎖
}
/***********************************************************************
 * 2003-5-20
 * 2003-5-31 如果是 HOOKBLOCK 的話並不會去處理 其內部串列
 *           而是僅僅記載 這個 HOOKBLOCK 而已 等到 LOAD 的時候
 *           才會去處理
 ***********************************************************************/
void DataStruct2ASCII(VoidHand strH,BNP bnp,WNP wnp)
{
//	char *strP;
	char Str[30];
	IONODEP ionode;
	CTRLNODEP ctrlnode;
	HOOKP hookp;
	

	AddString(strH,"STARTBLOCK\n");
	while(bnp!=NULL)
	{
			//case HOOKBLOCK:
			//break;
			//case FUNCBLOCK:
			AddString(strH,"NEWNODE\n");
			//ID
			StrPrintF(Str,"ID\n%d\n",bnp->ID);							
			AddString(strH,Str);
			
			//TYPE
			StrPrintF(Str,"TYPE\n%d\n",bnp->TYPE);							
			AddString(strH,Str);
			//bap
			AddString(strH,"NEWBAP\n");
				//BITMAPID
				StrPrintF(Str,"BITMAPID\n%d\n",bnp->bap->BITMAPID);							
				AddString(strH,Str);
				//XY.x
				StrPrintF(Str,"XY\n%d\n",bnp->bap->XY.x);							
				AddString(strH,Str);
				//XY.y
				StrPrintF(Str,"%d\n",bnp->bap->XY.y);							
				AddString(strH,Str);
				//SIZE.x
				StrPrintF(Str,"SIZE\n%d\n",bnp->bap->SIZE.x);							
				AddString(strH,Str);
				//SIZE.y
				StrPrintF(Str,"%d\n",bnp->bap->SIZE.y);							
				AddString(strH,Str);
				//IONODEP (if != NULL)
				if(bnp->bap->IONodeLLHead!=NULL)
				{
					AddString(strH,"NEWIONODEP\n");
					ionode=bnp->bap->IONodeLLHead;
					//while()
					while(ionode!=NULL)
					{
					AddString(strH,"ADDIONODE\n");
					//ID
					StrPrintF(Str,"ID\n%d\n",ionode->ID);							
					AddString(strH,Str);
					//TYPE
					StrPrintF(Str,"TYPE\n%d\n",ionode->TYPE);							
					AddString(strH,Str);
					//BYTEs  6-2
					StrPrintF(Str,"BYTES\n%d\n",ionode->BYTES);							
					AddString(strH,Str);
					//value
					switch(ionode->BYTES)
					{
						case 2:
							StrPrintF(Str,"VALUE\n%d\n",*((int *)ionode->P));
						break;
						case 4:
							StrPrintF(Str,"VALUE\n%d\n",*((int *)ionode->P));
						break;
						default:
							StrPrintF(Str,"");
						break;
					}
					AddString(strH,Str);
					//TL.x
					StrPrintF(Str,"TL\n%d\n",ionode->TL.x);
					AddString(strH,Str);
					//TL.y
					StrPrintF(Str,"%d\n",ionode->TL.y);
					AddString(strH,Str);
					//SIZE.x
					StrPrintF(Str,"SIZE\n%d\n",ionode->EXT.x);
					AddString(strH,Str);
					//SIZE.y
					StrPrintF(Str,"%d\n",ionode->EXT.y);
					AddString(strH,Str);
					
						ionode=ionode->NEXT;
					}
				}
				//CTRLNODEP (if != NULL)
				if(bnp->bap->CTRLNodeLLHead!=NULL)
				{
					AddString(strH,"NEWCTRLNODEP\n");
					ctrlnode=bnp->bap->CTRLNodeLLHead;
					//while()
					while(ctrlnode!=NULL)
					{
					AddString(strH,"ADDCTRLNODE\n");
					//ID
					StrPrintF(Str,"ID\n%d\n",ctrlnode->ID);
					AddString(strH,Str);
					//TL.x
					StrPrintF(Str,"TL\n%d\n",ctrlnode->TL.x);
					AddString(strH,Str);
					//TL.y
					StrPrintF(Str,"%d\n",ctrlnode->TL.y);
					AddString(strH,Str);
					//SIZE.x
					StrPrintF(Str,"SIZE\n%d\n",ctrlnode->EXT.x);
					AddString(strH,Str);
					//SIZE.y
					StrPrintF(Str,"%d\n",ctrlnode->EXT.y);
					AddString(strH,Str);
						ctrlnode=ctrlnode->NEXT;
					}
				}
			AddString(strH,"ENDBAP\n");
			//pap(if != NULL)
			if(bnp->pap!=NULL)
			{
			AddString(strH,"NEWPAP\n");
				//BITMAPID
				StrPrintF(Str,"BITMAPID\n%d\n",bnp->pap->BITMAPID);							
				AddString(strH,Str);
				//XY.x
				StrPrintF(Str,"XY\n%d\n",bnp->pap->XY.x);							
				AddString(strH,Str);
				//XY.y
				StrPrintF(Str,"%d\n",bnp->pap->XY.y);							
				AddString(strH,Str);
				//SIZE.x
				StrPrintF(Str,"SIZE\n%d\n",bnp->pap->SIZE.x);							
				AddString(strH,Str);
				//SIZE.y
				StrPrintF(Str,"%d\n",bnp->pap->SIZE.y);							
				AddString(strH,Str);
				//CTRLNODEP (if != NULL)
				if(bnp->pap->CTRLNodeLLHead!=NULL)
				{
					AddString(strH,"NEWCTRLNODEP\n");
					ctrlnode=bnp->pap->CTRLNodeLLHead;
					//while()
					while(ctrlnode!=NULL)
					{
					AddString(strH,"ADDCTRLNODE\n");
					//ID
					StrPrintF(Str,"ID\n%d\n",ctrlnode->ID);
					AddString(strH,Str);
					//TL.x
					StrPrintF(Str,"TL\n%d\n",ctrlnode->TL.x);
					AddString(strH,Str);
					//TL.y
					StrPrintF(Str,"%d\n",ctrlnode->TL.y);
					AddString(strH,Str);
					//SIZE.x
					StrPrintF(Str,"SIZE\n%d\n",ctrlnode->EXT.x);
					AddString(strH,Str);
					//SIZE.y
					StrPrintF(Str,"%d\n",ctrlnode->EXT.y);
					AddString(strH,Str);
						ctrlnode=ctrlnode->NEXT;
					}
				}
			AddString(strH,"ENDPAP\n");
			}
			
			//LoopBlockHOOKP 只會有一個 不用end
			if(bnp->TYPE==LOOPBLOCK)
			{
				AddString(strH,"NEWLOOPHOOK\n");
				AddString(strH,"NEWHOOK\n");
				//遞迴呼叫
				DataStruct2ASCII(strH,((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP,((HOOKP)bnp->LoopBlockHOOKP)->WireLLHeadP);
			}
			if(bnp->TYPE==CASEBLOCK)
			{
			//CaseBlockHOOKLLHeadP
			hookp = (HOOKP)bnp->CaseBlockHOOKLLHeadP;
			AddString(strH,"NEWCASEHOOK\n");
			while(hookp!=NULL)
			{
				AddString(strH,"NEWHOOK\n");
				//遞迴呼叫
				DataStruct2ASCII(strH,hookp->BlockLLHeadP,hookp->WireLLHeadP);
				hookp=hookp->NEXT;
				if(hookp == (HOOKP)bnp->CaseBlockHOOKLLHeadP)//因為 case的loop是循環的
					break;
			}

			//CurrentHOOKP
			
			}
			
			// filename
			if(bnp->filename != NULL)
			{
				StrPrintF(Str,"FILENAME\n%s\n",bnp->filename);
				AddString(strH,Str);
			}
			AddString(strH,"ENDNODE\n");
		
		bnp=bnp->NEXTNODE;
	}
	AddString(strH,"ENDBLOCK\n");
	AddString(strH,"STARTWIRE\n");
	while(wnp!=NULL)
	{
		// 5-31 決定 HOOKBLOCK 的ID為0
		//if(wnp->StartBlockP->TYPE==HOOKBLOCK)
		//{
		//wnp->StartBlockP->ID;
		//StrPrintF(Str,"STARTBID\n0\n");
		//}else{
		//wnp->StartBlockP->ID;
		StrPrintF(Str,"STARTBID\n%d\n",wnp->StartBlockP->ID);
		//}
		AddString(strH,Str);
		//wnp->StartNodeP
		StrPrintF(Str,"STARTNID\n%d\n",wnp->StartNodeP->ID);
		AddString(strH,Str);
		//if(wnp->EndBlockP->TYPE==HOOKBLOCK)
		//{
		//wnp->EndBlockP
		//StrPrintF(Str,"ENDBID\n0\n");
		//}else{
		//wnp->EndBlockP
		StrPrintF(Str,"ENDBID\n%d\n",wnp->EndBlockP->ID);
		//}
		AddString(strH,Str);
		//wnp->EndNodeP
		StrPrintF(Str,"ENDNID\n%d\n",wnp->EndNodeP->ID);
		AddString(strH,Str);
		wnp=wnp->NEXTNODE;
	}
	AddString(strH,"ENDWIRE\n");
}
void CountInputNode(int *nodes,BNP bnp)
{
	while(bnp!=NULL)
	{
		switch(bnp->bap->BITMAPID)
		{
			case CTRLU8BLOCKBitmap:
			case LOGICCTRLBitmap:
			(*nodes)++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				CountInputNode(nodes,((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	
	
}
void CountOutputNode(int *nodes,BNP bnp)
{
	while(bnp!=NULL)
	{
		switch(bnp->bap->BITMAPID)
		{
			case INDICATOR8BLOCKBitmap:
			case LOGICINDICATORBitmap:
			(*nodes)++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				CountOutputNode(nodes,((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	
	
}

HOOKP  FindInputNode(HOOKP hookp,BNP hook_block,BNP bnpLLHead, int *IOID)
{
	IONODEP ionode;
	int lattice_y;
	int nodes=0;
	BNP bnp = bnpLLHead;
	
	CountInputNode(&nodes,bnp);
	
	if(nodes!=0)
		lattice_y=32/nodes;
	else
		lattice_y=0;
		
	nodes=0;
	bnp = bnpLLHead;
	
	while(bnp!=NULL)
	{
		switch(bnp->bap->BITMAPID)
		{
			case CTRLU8BLOCKBitmap:
			case LOGICCTRLBitmap:
			
			ionode = allocate_a_new_IOnode();
			ionode->ID = (*IOID)++;
			ionode->TYPE = 1;
			ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
			ionode->BYTES=sizeof(int);
			ionode->NUMS=1;
			ionode->TL.x=0;
			ionode->TL.y=nodes*lattice_y;
			ionode->EXT.x=16;
			ionode->EXT.y=lattice_y;
			hook_block->bap->IONodeLLHead = 
				InsertIntoIONodesLL(hook_block->bap->IONodeLLHead,	ionode);
			// 加入線段 使用 StartStopAddIntoWLL()
			WireStart.BlockP = hook_block;
			WireStart.NodeP = ionode;
			WireStop.BlockP = bnp;
			WireStop.NodeP = bnp->bap->IONodeLLHead;
			//StartStopAddIntoWLL();
			hookp = AddWireLinkListNode(hookp,WireStart,WireStop);
			
			nodes++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				hookp = FindInputNode(hookp,hook_block,((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP,IOID);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	
	
	return hookp;
}
HOOKP  FindOutputNode(HOOKP hookp,BNP hook_block,BNP bnpLLHead, int *IOID)
{
	IONODEP ionode;
	int lattice_y;
	int nodes=0;
	BNP bnp = bnpLLHead;
	
	CountOutputNode(&nodes,bnp);
	
	if(nodes!=0)
		lattice_y=32/nodes;
	else
		lattice_y=0;

	nodes=0;
	bnp = bnpLLHead;
	
	while(bnp!=NULL)
	{
		switch(bnp->bap->BITMAPID)
		{
			case INDICATOR8BLOCKBitmap:
			case LOGICINDICATORBitmap:
			
			ionode = allocate_a_new_IOnode();
			ionode->ID = (*IOID)++;
			ionode->TYPE = 0;
			ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
			ionode->BYTES=sizeof(int);
			ionode->NUMS=1;
			ionode->TL.x=16;
			ionode->TL.y=nodes*lattice_y;
			ionode->EXT.x=16;
			ionode->EXT.y=lattice_y;
			hook_block->bap->IONodeLLHead = 
				InsertIntoIONodesLL(hook_block->bap->IONodeLLHead,	ionode);
			// 加入線段 使用 StartStopAddIntoWLL()
			WireStart.BlockP = hook_block;
			WireStart.NodeP = ionode;
			WireStop.BlockP = bnp;
			WireStop.NodeP = bnp->bap->IONodeLLHead;
			//StartStopAddIntoWLL();
			hookp = AddWireLinkListNode(hookp,WireStart,WireStop);
			
			nodes++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				hookp=FindOutputNode(hookp,hook_block,((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP,IOID);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	
	
	return hookp;
}

/***********************************************************************
 * 2003-4-12
 * 2003-05-24 改用file stream 的方式
 ***********************************************************************/
void SAVE(char *filename )
{
	VoidHand strH;
	char *strP;
	int i;
	FileHand savefile;
	
// 方塊資料結構
/*
	UInt16 ID;
	UInt16 TYPE;
	BAP bap;					// 指標
	PAP pap;					// 指標
	Ptr LoopBlockHOOKP;			// 指標
	Ptr CurrentHOOKP;			// 指標
	Ptr CaseBlockHOOKLLHeadP; 	// 指標
	Ptr hookP;					// 指標
	char *filename;				// 指標
	struct BlockNode *SelfBlockLLHead;		// 指標 不存
	struct BlockNode *BackgroundBlockLLHead;// 指標 不存
	struct BlockNode *PREVNODE;				// 指標 不存
	struct BlockNode *NEXTNODE;				// 指標 不存
*/
// bap 資料結構
/*
	UInt16 BITMAPID;			//
	struct Point XY;			//
	struct Point SIZE;			//
	IONODEP 	IONodeLLHead;	// 指標
	UInt16 		IONodeNUM;		// 不存
	CTRLNODEP 	CTRLNodeLLHead;	// 指標
	Boolean DrawBorder;			// 不存
	Boolean ReDraw;				// 不存
	Boolean Clean; 				// 不存
 
*/
// pap 資料結構
/*
	UInt16 BITMAPID;
	struct Point XY;
	struct Point SIZE;	
	CTRLNODEP CTRLNodeLLHead;	// 指標
*/
// IONODEP
/*
	UInt16 ID;					
	UInt16 TYPE; 				// 存
	Boolean NotReady;
	Ptr P;						// 內容值 
	UInt16 BYTES;
	UInt16 NUMS;
	struct Point TL; 			// 存
	struct Point EXT;			// 存
	struct IONODE *PREV;
	struct IONODE *NEXT;
*/
// CTRLNODEP
/*
	UInt16 ID;
	struct Point TL;			// 存
	struct Point EXT;			// 存
	struct CTRLNODE *PREV;
	struct CTRLNODE *NEXT;
*/
// wire
/*
	UInt16 WireID;
	BNP    StartBlockP;
	IONODEP StartNodeP;
	BNP    EndBlockP;
	IONODEP EndNodeP;
	Boolean DIRTY;
	Boolean STARTRUN;
	struct Point RUNPOINT;
	int RUNSEG;
	struct wList* PREVNODE;
	struct wList* NEXTNODE;
*/
	//開 db
	//OpenDB(filename);
	//
	// 5-25 使用file stream 方式存檔
	//
	savefile = FileOpen (0,filename,appDBType,appCreatorID,fileModeReadWrite,NULL);
	
	strH=MemHandleNew(i=sizeof('\0'));
	strP=MemHandleLock(strH);
	strP[0]='\0';
	MemHandleUnlock(strH);
	DataStruct2ASCII(strH,SYSHOOK->BlockLLHeadP,SYSHOOK->WireLLHeadP);

	strP=MemHandleLock(strH);
	//NewStrRecord(strP);
	FileWrite (savefile, strP,StrLen(strP), 1, NULL);

	MemHandleUnlock(strH);
	
	MemHandleFree(strH);
	//CloseDB();
	FileClose(savefile);
}
WNP RemoveDarkWire(WNP wnp,BNP hookblock)
{
	WNP wnphead=wnp;
	
	while(wnp!=NULL)
	{
		if((wnp->StartBlockP==hookblock)||(wnp->EndBlockP==hookblock))
		{
			wnphead=RemoveWireLinkList(wnphead,wnp);
			wnp=wnphead;
		}else
		{
			wnp=wnp->NEXTNODE;
		}
	}
	
	return wnphead;
}
/***********************************************************************
*	"FileNameInput" event process routine
*	
*	
*	
*	
*	
*	
************************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
Boolean SAVEFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;
	//char str[10];
	//Handle resH;
	//BitmapType *bitmap;
	//RectangleType Rtest;
	//int i;
	//FormPtr		frm;
	//ListPtr		lst;
	//Word 		CurrentRecord;
	FieldPtr	fldP;
	char filename[32];
	int i;
	int IOID=0;
	
	switch (eventP->eType) 
		{
		/*
		case menuEvent:
			return FrontPanelFormDoCommand(eventP->data.menu.itemID);
		*/
   		case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the main form.
	   	if (eventP->data.ctlEnter.controlID == SAVEOKButton)
	  		 	{
	   				fldP = GetObjectPtr(SAVEFileNameField);
					if(FldGetTextLength(fldP)!=0)
					{
	   				
	   				StrCopy(filename,FldGetTextPtr(fldP));
	  		 		for(i=FldGetTextLength(fldP)+1;i<32;i++)
	  		 			filename[i]='\0';
					//5-31 在hook block上產生IO點 與加入 暗線段
					// 原本 hook block 上的IO點 強制放掉
					SYSHOOK->BlockLLHeadP->bap->IONodeLLHead=NULL;
					// 還有連接的wire也要移去 不然會在第二次存檔時 發生線段重複 然後在 custom 載入時發生卡死
					SYSHOOK->WireLLHeadP=RemoveDarkWire(SYSHOOK->WireLLHeadP,SYSHOOK->BlockLLHeadP);
					// 建立新的 IO點 與 新的線段
					SYSHOOK=FindInputNode(SYSHOOK,SYSHOOK->BlockLLHeadP,SYSHOOK->BlockLLHeadP->NEXTNODE,&IOID);
					SYSHOOK=FindOutputNode(SYSHOOK,SYSHOOK->BlockLLHeadP,SYSHOOK->BlockLLHeadP->NEXTNODE,&IOID);
					
					// 公共變數用完後要記得擦屁股
					WireStart.BlockP = NULL;
					WireStart.NodeP = NULL;
					WireStop.BlockP = NULL;
					WireStop.NodeP = NULL;
					
	   				SAVE(filename);
					FrmReturnToForm(0);
					}
					handled = true;
				}
	   	if (eventP->data.ctlEnter.controlID == SAVECANCELButton)
	  		 	{
					FrmReturnToForm(0);
					handled = true;
				}
			break;
		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm ( frmP);
			FrmSetFocus	(frmP,FrmGetObjectIndex(frmP,SAVEFileNameField));
			handled = true;
			break;
		default:
			break;
		
		}
	
	return handled;
}
