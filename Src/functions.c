#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"
#include "FileStream.h"

#include "vpl.h"

/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
extern HOOKP SYSHOOK;	// 程式主串列的頭
extern EndPoint WireStart,WireStop;
//extern Boolean WhoLoad; 
//extern Boolean WaitFileLoad;
//extern HOOKP	SecondHead; // for 自訂方塊載入時用
//extern  int penMove Average;
//extern FUNCTIONSLLDATAtype FUNCTIONSLLDATA;
extern struct hand HandPointer;
extern BNP FunctionCatagoryLLHead;
extern DmOpenRef   db;
extern UInt32      numRecords;

extern void DrawIcon(UInt16 BitMapID,UInt16 x,UInt16 y);
extern Boolean CheckBoundary(int x,int y,int XX,int YY,int X,int Y);
extern InAreaInfType IsInArea(InAreaInfType InAreaInf,BNP llp,UInt16 now_x,UInt16 now_y,UInt16 window_x,UInt16 window_y);
extern BNP allocate_a_new_node();
extern BAP allocate_a_new_bap();
extern BNP InsertIntoBlockLLwithUID(BNP bLLP,BNP node);
extern IONODEP allocate_a_new_IOnode();
extern IONODEP InsertIntoIONodesLL(IONODEP IONodeH,IONODEP node );
extern HOOKP allocate_a_new_hook();
extern CTRLNODEP allocate_a_new_CTRLnode();
extern PAP allocate_a_new_pap();
extern CTRLNODEP InsertIntoCTRLNodesLL(CTRLNODEP ctrlnodeH,CTRLNODEP ctrlnode );
extern BNP InsertIntoBlockLLNoUID(BNP bLLP,BNP node);

Boolean CatagoryIn;
BNP CatagoryBNP;
UInt16 Catx;
UInt16 Caty;
UInt16 Catextx;
UInt16 Catexty;
Boolean CUSTOMLOAD=false;
struct HOOK CUSTUMHOOK;
/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/
// 5-11 這裡是從文字資料中建立圖示
BNP new_node(char **k,BNP LLHead,int *i)
{
	BNP newnode;
	BAP bap;
	PAP pap;
	CTRLNODEP ctrlnode;
	IONODEP ionode;
	//HOOKP hookp;
	//int i=0;
				newnode = allocate_a_new_node();
				while(StrCompare(*k,"ENDNODE") != 0)
				{
					//*k += *i;
					if(StrCompare(*k,"NEWBAP") == 0){
						*i += StrLen(*k) + 1;
						bap = allocate_a_new_bap();
						*k += StrLen(*k) + 1;
						while(StrCompare(*k,"ENDBAP") != 0)
						{
							if(StrCompare(*k,"BITMAPID") == 0){
								*i += StrLen(*k) + 1;
								*k += StrLen(*k) + 1;
								bap->BITMAPID= (UInt16)StrAToI(*k);
							}else if(StrCompare(*k,"EXTX") == 0){
								*i += StrLen(*k) + 1;
								*k += StrLen(*k) + 1;
								bap->SIZE.x= (UInt16)StrAToI(*k);
							}else if(StrCompare(*k,"EXTY") == 0){
								*i += StrLen(*k) + 1;
								*k += StrLen(*k) + 1;
								bap->SIZE.y= (UInt16)StrAToI(*k);
							}else if(StrCompare(*k,"NEWIONODE") == 0){
								*i += StrLen(*k) + 1;
								ionode= allocate_a_new_IOnode();
								*k += StrLen(*k) + 1;
								while(StrCompare(*k,"ENDIONODE") != 0)
								{
									if(StrCompare(*k,"TYPE") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ionode->ID=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"X") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ionode->TL.x=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"Y") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ionode->TL.y=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"EXTX") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ionode->EXT.x=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"EXTY") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ionode->EXT.y=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"NEWVALUESPACE") == 0){
										ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
									}
									*i += StrLen(*k) + 1;
									*k += StrLen(*k) + 1;
								}// while IOnode
								bap->IONodeLLHead=InsertIntoIONodesLL(bap->IONodeLLHead,ionode);
							}else if(StrCompare(*k,"NEWCTRLNODE") == 0){
								*i += StrLen(*k) + 1;
								ctrlnode= allocate_a_new_CTRLnode();
								*k += StrLen(*k) + 1;
								while(StrCompare(*k,"ENDCTRLNODE") != 0)
								{
									if(StrCompare(*k,"ID") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->ID=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"X") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->TL.x=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"Y") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->TL.y=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"EXTX") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->EXT.x=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"EXTY") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->EXT.y=(UInt16)StrAToI(*k);
									}
									*i += StrLen(*k) + 1;
									*k += StrLen(*k) + 1;
								}// while CTRLnode
								bap->CTRLNodeLLHead=InsertIntoCTRLNodesLL(bap->CTRLNodeLLHead,ctrlnode);
							}// end if
							*i += StrLen(*k) + 1;
							*k += StrLen(*k) + 1;
						}//while endbap
						newnode->bap=bap;
					}else if(StrCompare(*k,"NEWPAP") == 0){
						*i += StrLen(*k) + 1;
						pap = allocate_a_new_pap();
						*k += StrLen(*k) + 1;
						while(StrCompare(*k,"ENDPAP") != 0)
						{
							if(StrCompare(*k,"BITMAPID") == 0){
								*i += StrLen(*k) + 1;
								*k += StrLen(*k) + 1;
								pap->BITMAPID= (UInt16)StrAToI(*k);
							}else if(StrCompare(*k,"EXTX") == 0){
								*i += StrLen(*k) + 1;
								*k += StrLen(*k) + 1;
								pap->SIZE.x= (UInt16)StrAToI(*k);
							}else if(StrCompare(*k,"EXTY") == 0){
								*i += StrLen(*k) + 1;
								*k += StrLen(*k) + 1;
								pap->SIZE.y= (UInt16)StrAToI(*k);
							}else if(StrCompare(*k,"NEWCTRLNODE") == 0){
								*i += StrLen(*k) + 1;
								ctrlnode= allocate_a_new_CTRLnode();
								*k += StrLen(*k) + 1;
								while(StrCompare(*k,"ENDCTRLNODE") != 0)
								{
									if(StrCompare(*k,"ID") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->ID=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"X") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->TL.x=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"Y") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->TL.y=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"EXTX") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->EXT.x=(UInt16)StrAToI(*k);
									}else if(StrCompare(*k,"EXTY") == 0){
										*i += StrLen(*k) + 1;
										*k += StrLen(*k) + 1;
										ctrlnode->EXT.y=(UInt16)StrAToI(*k);
									}
									*i += StrLen(*k) + 1;
									*k += StrLen(*k) + 1;
								}// while CTRLnode
								pap->CTRLNodeLLHead=InsertIntoCTRLNodesLL(pap->CTRLNodeLLHead,ctrlnode);
							}
							*i += StrLen(*k) + 1;
							*k += StrLen(*k) + 1;
						}
						newnode->pap=pap;
					}else if(StrCompare(*k,"NEWHOOKP") == 0){
						*i += StrLen(*k) + 1;
						(HOOKP)newnode->hookP = allocate_a_new_hook();
						*k += StrLen(*k) + 1;
						while(StrCompare(*k,"ENDHOOKP") != 0)
						{
							if(StrCompare(*k,"NEWNODE") == 0){
								*i += StrLen(*k) + 1;
								*k += StrLen(*k) + 1;
								((HOOKP)newnode->hookP)->BlockLLHeadP = new_node(&(*k),((HOOKP)newnode->hookP)->BlockLLHeadP,i);
							}
							*i += StrLen(*k) + 1;
							*k += StrLen(*k) + 1;

						}
					}else if(StrCompare(*k,"TYPE") == 0){
						*i += StrLen(*k) + 1;
						*k += StrLen(*k) + 1;
						newnode->TYPE= (UInt16)StrAToI(*k);
					}else if(StrCompare(*k,"NEWLOOPHOOK") == 0){
						(HOOKP)newnode->LoopBlockHOOKP=allocate_a_new_hook();
					}
					//i += StrLen(k) + 1;
					*i += StrLen(*k) + 1;
					*k += StrLen(*k) + 1;

				}// while NEWNODE
				LLHead = InsertIntoBlockLLNoUID(LLHead,newnode);

	return LLHead;	
}
/*
	5-25 改成 fopen 方式 外部產生function 版面內容
*/
BNP set_function_palette(void )
{
	// 9種功能分類
	// 
	//BNP newnode;
	BNP LLHead=NULL;
	//BAP bap;
	//int atoi;
	//char functxt[]="NEWNODE\nTYPE\n9000\nNEWBAP\nBITMAPID\n6100\nEXTX\n32\nEXTY\n32\nENDBAP\nNEWHOOKP\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n1000\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n1100\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n1200\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n1300\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n3100\nEXTX\n32\nEXTY\n16\nENDBAP\nNEWPAP\nBITMAPID\n1400\nEXTX\n32\nEXTY\n16\nENDPAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n3200\nEXTX\n32\nEXTY\n16\nENDBAP\nNEWPAP\nBITMAPID\n1500\nEXTX\n32\nEXTY\n16\nENDPAP\nENDNODE\nENDHOOKP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6200\nEXTX\n32\nEXTY\n32\nENDBAP\nNEWHOOKP\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n3900\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n4000\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n4100\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n4800\nEXTX\n32\nEXTY\n16\nENDBAP\nNEWPAP\nBITMAPID\n4400\nEXTX\n16\nEXTY\n32\nENDPAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n4700\nEXTX\n32\nEXTY\n16\nENDBAP\nNEWPAP\nBITMAPID\n4600\nEXTX\n16\nEXTY\n32\nENDPAP\nENDNODE\nENDHOOKP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6300\nX\n10\nY\n10\nEXTX\n32\nEXTY\n32\nENDBAP\nNEWHOOKP\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n4900\nEXTX\n32\nEXTY\n32\nNEWCTRLNODE\nID\n1\nX\n25\nY\n25\nEXTX\n7\nEXTY\n7\nENDCTRLNODE\nENDBAP\nNEWLOOPHOOK\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n5000\nEXTX\n32\nEXTY\n32\nNEWCTRLNODE\nID\n1\nX\n25\nY\n25\nEXTX\n7\nEXTY\n7\nENDCTRLNODE\nENDBAP\nNEWLOOPHOOK\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n5100\nEXTX\n32\nEXTY\n32\nENDBAP\nENDNODE\nENDHOOKP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6400\nX\n10\nY\n10\nEXTX\n32\nEXTY\n32\nENDBAP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6500\nX\n10\nY\n10\nEXTX\n32\nEXTY\n32\nENDBAP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6600\nX\n10\nY\n10\nEXTX\n32\nEXTY\n32\nENDBAP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6700\nX\n10\nY\n10\nEXTX\n32\nEXTY\n32\nENDBAP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6800\nX\n10\nY\n10\nEXTX\n32\nEXTY\n32\nENDBAP\nNEWHOOKP\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n7000\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n7100\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n7200\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n7300\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nNEWNODE\nTYPE\n8000\nNEWBAP\nBITMAPID\n7400\nEXTX\n16\nEXTY\n16\nENDBAP\nENDNODE\nENDHOOKP\nENDNODE\nNEWNODE\nNEWBAP\nBITMAPID\n6900\nX\n10\nY\n10\nEXTX\n32\nEXTY\n32\nENDBAP\nENDNODE\nENDEND";
	char *functxt;
	int i,j;
	char *k;
	// 5-20
    //UInt16      error;
    Handle   hrecord;
    void        *precord;
    UInt32      size;
    //FileHand func;
	// 5-20 使用pdb 方式讀入
	// 開 db
    //error=DmCreateDatabase(0,"func",appCreatorID,'func',false);

    //if((error!=dmErrAlreadyExists)&&(error!=0)) {
        /* Error Occurs!! */
    //}
	//
	// 5-25
	// 因為無法得知 file stream 的檔案格式
	// 所以無法外部產生資料檔 然後用file stream 方式讀取
	// 因此使用database方式讀取 正常
	//
	if(DmFindDatabase(0,"func")!=0)
	{
    
    db=DmOpenDatabaseByTypeCreator('func',appCreatorID,
        dmModeReadOnly);

	// 檢查這個 db 有幾個 rec
	numRecords=DmNumRecords(db);

	// 讀 rec
    //size=sizeof(int);
    hrecord=DmQueryRecord(db,0); //Query 不用release
    precord=MemHandleLock(hrecord);
	
	size=StrLen(precord);
	functxt=MemHandleLock(MemHandleNew(size+1));
    MemMove(functxt,precord,size+1);
//    functxt[size+1]='\0';
    MemHandleUnlock(hrecord);
	//
    DmCloseDatabase(db);

	// 5-25
	//functxt=MemHandleLock(MemHandleNew(4096));
	//if((func=FileOpen(0,"func",'func','wjfx',fileModeReadOnly,NULL))!=0)
	//{
	//	for(i=0;(FileEOF(func)!=0);i++)
	//	{
	//		FileRead(func, functxt+i,1, 1, NULL);
	//	}
	//
	j=StrLen(functxt);
	for(i=0;i<j;i++)
	{
		if(functxt[i] == '\n')
			functxt[i]='\0';
	}
	
	j=0;
	i=0;
	while(StrCompare((k=&functxt[i]),"ENDEND") != 0)
	{
		k=&functxt[i];
		if(StrCompare(k,"NEWNODE") == 0){
			i += StrLen(k) + 1;
			k=&functxt[i];
			LLHead = new_node(&k,LLHead,&i);
				
		}
		i += StrLen(k) + 1;
	}// while ENDEND
	
	//5-20
	MemHandleUnlock(MemPtrRecoverHandle(functxt));
	MemHandleFree(MemPtrRecoverHandle(functxt));
	
	}// if
	return LLHead;
}
void DrawToolIcon(BNP bnp)
{
	int x=1;
	int y=16;
	while(bnp != NULL)
	{
		DrawIcon(bnp->bap->BITMAPID,x,y);
		bnp->bap->XY.x = x;
		bnp->bap->XY.y = y;
		
		if(x == 1)
			x = 34;
		else if(x == 34)
			x = 67;
		else if(x == 67)
		{
			y +=35;
			x = 1;
		}	
		bnp = bnp->NEXTNODE;
	}
}
void DrawToolCatagory(void )
{
	//FunctionCatagoryLLHead 公共變數
	BNP bnp;
	
	bnp = FunctionCatagoryLLHead;
	DrawToolIcon(bnp);
}

/***********************************************************************
 *
 *
 *
 ***********************************************************************/

void FunctionspenUpProcess(EventPtr eventP)
{
}
void FunctionspenDownProcess(EventPtr eventP)
{
}
void FunctionspenMoveProcess(EventPtr eventP)
{
}
// 5-31 
// 輸入
// bnpLLHead 是內部串列hookP的block串列的頭一個
// IOID IO點的計數器
/*
void  FindInputNode(BNP bnpLLHead, int *IOID)
{
	IONODEP ionode;
	int lattice_y;
	int nodes=0;
	BNP bnp = bnpLLHead;
	
	while(bnp!=NULL)
	{
		switch(bnp->bap->BITMAPID)
		{
			case CTRLU8BLOCKBitmap:
			case LOGICCTRLBitmap:
			nodes++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				FindInputNode(((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP,IOID);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	
	
	lattice_y=32/nodes;
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
			CUSTUMHOOK.BlockLLHeadP->bap->IONodeLLHead = 
				InsertIntoIONodesLL(CUSTUMHOOK.BlockLLHeadP->bap->IONodeLLHead,	ionode);
			// 加入線段 使用 StartStopAddIntoWLL()
			WireStart.BlockP = CUSTUMHOOK.BlockLLHeadP;
			WireStart.NodeP = ionode;
			WireStop.BlockP = bnp;
			WireStop.NodeP = bnp->bap->IONodeLLHead;
			StartStopAddIntoWLL();
			
			nodes++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				FindInputNode(((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP,IOID);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	
	
	
}
void  FindOutputNode(BNP bnpLLHead, int *IOID)
{
	IONODEP ionode;
	int lattice_y;
	int nodes=0;
	BNP bnp = bnpLLHead;
	
	while(bnp!=NULL)
	{
		switch(bnp->bap->BITMAPID)
		{
			case INDICATOR8BLOCKBitmap:
			case LOGICINDICATORBitmap:
			nodes++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				FindOutputNode(((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP,IOID);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	

	lattice_y=32/nodes;
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
			CUSTUMHOOK.BlockLLHeadP->bap->IONodeLLHead = 
				InsertIntoIONodesLL(CUSTUMHOOK.BlockLLHeadP->bap->IONodeLLHead,	ionode);
			// 加入線段 使用 StartStopAddIntoWLL()
			WireStart.BlockP = bnp;
			WireStart.NodeP = bnp->bap->IONodeLLHead;
			WireStop.BlockP = CUSTUMHOOK.BlockLLHeadP;
			WireStop.NodeP = ionode;
			StartStopAddIntoWLL();
				
			nodes++;
			break;
			case FORLOOPBitmap:
			case WHILELOOPBitmap:
				FindOutputNode(((HOOKP)bnp->LoopBlockHOOKP)->BlockLLHeadP,IOID);
			break;
		}
		bnp=bnp->NEXTNODE;	
	}	
	
}
*/
// 5-31
void AddCUSTtoSYSHOOK(void )
{
	//IONODEP ionode;
	//BNP bnp;
	
	CUSTUMHOOK.BlockLLHeadP->bap->BITMAPID = CUSTOMBLOCKBitmap;
	CUSTUMHOOK.BlockLLHeadP->bap->XY.x=10;
	CUSTUMHOOK.BlockLLHeadP->bap->XY.y=20;
	CUSTUMHOOK.BlockLLHeadP->bap->SIZE.x=32;
	CUSTUMHOOK.BlockLLHeadP->bap->SIZE.y=32;
	CUSTUMHOOK.BlockLLHeadP->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
	CUSTUMHOOK.BlockLLHeadP->BackgroundBlockLLHead=SYSHOOK->BlockLLHeadP;

	// 製造IO點
	// 5-31 這樣產生是空的IO節點
	//      接著要在裡面 加入線段 該線段係 連接 此IO與內部IO方塊之用
	//      將會利用到 跨結構產生線段 副程式
	// 5-31 製造IO點 與 產生 暗線線段 改成 存檔時產生
	// 先找 input
	//FindInputNode(((HOOKP)CUSTUMHOOK.BlockLLHeadP->hookP)->BlockLLHeadP,&IOID);
	// 再找 output
	//FindOutputNode(((HOOKP)CUSTUMHOOK.BlockLLHeadP->hookP)->BlockLLHeadP,&IOID);
	
	CUSTUMHOOK.BlockLLHeadP->NEXTNODE=NULL;
	CUSTUMHOOK.BlockLLHeadP->PREVNODE=NULL;
	CUSTUMHOOK.BlockLLHeadP->bap->ReDraw=true;
	CUSTUMHOOK.BlockLLHeadP->bap->Clean=true;
	SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,CUSTUMHOOK.BlockLLHeadP);
}
// 5-11 這裡是負責建立元件
void AddToSYSHOOK(BNP bnp)
{
	BNP newnode;
	BNP ctrlblock1,ctrlblock2;// 預設元件1號 預設元件2號
	IONODEP ionode;
	char str_true[5]={"true"};
	char str_false[6]={"false"};

	//BAP newbap;
   switch(bnp->bap->BITMAPID)
   {
   	case FORLOOPBitmap:
   	case WHILELOOPBitmap:
		newnode = allocate_a_new_node();
		newnode->TYPE = LOOPBLOCK;
		newnode->bap = allocate_a_new_bap();
		newnode->bap->BITMAPID = bnp->bap->BITMAPID;
		newnode->bap->XY.x=10;
		newnode->bap->XY.y=20;
		newnode->bap->SIZE.x=bnp->bap->SIZE.x;
		newnode->bap->SIZE.y=bnp->bap->SIZE.y;
		newnode->bap->CTRLNodeLLHead=allocate_a_new_CTRLnode();
		newnode->bap->CTRLNodeLLHead->ID=DRAGNODE;
		newnode->bap->CTRLNodeLLHead->TL.x=25;
		newnode->bap->CTRLNodeLLHead->TL.y=25;
		newnode->bap->CTRLNodeLLHead->EXT.x=7;
		newnode->bap->CTRLNodeLLHead->EXT.y=7;
		(HOOKP)newnode->LoopBlockHOOKP = allocate_a_new_hook();
   	break;
   	case SWITCHCASEBitmap:
		newnode = allocate_a_new_node();
		newnode->TYPE = CASEBLOCK;
		newnode->bap = allocate_a_new_bap();
		newnode->bap->BITMAPID = bnp->bap->BITMAPID;
		newnode->bap->XY.x=10;
		newnode->bap->XY.y=20;
		newnode->bap->SIZE.x=bnp->bap->SIZE.x;
		newnode->bap->SIZE.y=bnp->bap->SIZE.y;
		newnode->bap->CTRLNodeLLHead = allocate_a_new_CTRLnode();
		newnode->bap->CTRLNodeLLHead->ID=DRAGNODE;
		newnode->bap->CTRLNodeLLHead->TL.x=25;
		newnode->bap->CTRLNodeLLHead->TL.y=25;
		newnode->bap->CTRLNodeLLHead->EXT.x=7;
		newnode->bap->CTRLNodeLLHead->EXT.y=7;
		newnode->bap->CTRLNodeLLHead->NEXT=allocate_a_new_CTRLnode();
		newnode->bap->CTRLNodeLLHead->NEXT->ID=SCPAGEUP;
		newnode->bap->CTRLNodeLLHead->NEXT->TL.x=25;
		newnode->bap->CTRLNodeLLHead->NEXT->TL.y=1;
		newnode->bap->CTRLNodeLLHead->NEXT->EXT.x=6;
		newnode->bap->CTRLNodeLLHead->NEXT->EXT.y=13;
		newnode->bap->CTRLNodeLLHead->NEXT->NEXT=allocate_a_new_CTRLnode();
		newnode->bap->CTRLNodeLLHead->NEXT->NEXT->ID=SCPAGEDOWN;
		newnode->bap->CTRLNodeLLHead->NEXT->NEXT->TL.x=1;
		newnode->bap->CTRLNodeLLHead->NEXT->NEXT->TL.y=1;
		newnode->bap->CTRLNodeLLHead->NEXT->NEXT->EXT.x=6;
		newnode->bap->CTRLNodeLLHead->NEXT->NEXT->EXT.y=13;
		
		(HOOKP)newnode->CaseBlockHOOKLLHeadP = allocate_a_new_hook();
		((HOOKP)newnode->CaseBlockHOOKLLHeadP)->name=MemHandleLock(MemHandleNew(StrLen(str_true)+1));
		MemMove(((HOOKP)newnode->CaseBlockHOOKLLHeadP)->name,str_true,StrLen(str_true)+1);
		(HOOKP)((HOOKP)newnode->CaseBlockHOOKLLHeadP)->NEXT = allocate_a_new_hook();
		(HOOKP)((HOOKP)((HOOKP)newnode->CaseBlockHOOKLLHeadP)->NEXT)->PREV = (HOOKP)newnode->CaseBlockHOOKLLHeadP;
		((HOOKP)((HOOKP)newnode->CaseBlockHOOKLLHeadP)->NEXT)->name=MemHandleLock(MemHandleNew(StrLen(str_false)+1));
		MemMove(((HOOKP)((HOOKP)newnode->CaseBlockHOOKLLHeadP)->NEXT)->name,str_false,StrLen(str_false)+1);

		(HOOKP)((HOOKP)((HOOKP)newnode->CaseBlockHOOKLLHeadP)->NEXT)->NEXT = (HOOKP)newnode->CaseBlockHOOKLLHeadP;
		((HOOKP)newnode->CaseBlockHOOKLLHeadP)->PREV = (HOOKP)((HOOKP)newnode->CaseBlockHOOKLLHeadP)->NEXT;
		(HOOKP)newnode->CurrentHOOKP=(HOOKP)newnode->CaseBlockHOOKLLHeadP;
   	break;
   	default:
		newnode = allocate_a_new_node();
		newnode->TYPE = FUNCBLOCK;
		newnode->bap = allocate_a_new_bap();
		newnode->bap->BITMAPID = bnp->bap->BITMAPID;
		newnode->bap->XY.x=10;
		newnode->bap->XY.y=20;
		newnode->bap->SIZE.x=bnp->bap->SIZE.x;
		newnode->bap->SIZE.y=bnp->bap->SIZE.y;
		switch(bnp->bap->BITMAPID)
		{
		   	case INDICATOR8BLOCKBitmap:
				newnode->pap = allocate_a_new_pap();
				newnode->pap->BITMAPID = bnp->pap->BITMAPID;
				newnode->pap->XY.x=10;
				newnode->pap->XY.y=20;
				newnode->pap->SIZE.x=bnp->pap->SIZE.x;
				newnode->pap->SIZE.y=bnp->pap->SIZE.y;
		   	break;
		   	case CTRLU8BLOCKBitmap:
				newnode->pap = allocate_a_new_pap();
				newnode->pap->BITMAPID = bnp->pap->BITMAPID;
				newnode->pap->XY.x=10;
				newnode->pap->XY.y=20;
				newnode->pap->SIZE.x=bnp->pap->SIZE.x;
				newnode->pap->SIZE.y=bnp->pap->SIZE.y;
				newnode->pap->CTRLNodeLLHead=allocate_a_new_CTRLnode();
				newnode->pap->CTRLNodeLLHead->ID=CONTROLUP;
				newnode->pap->CTRLNodeLLHead->TL.x=28;
				newnode->pap->CTRLNodeLLHead->TL.y=0;
				newnode->pap->CTRLNodeLLHead->EXT.x=4;
				newnode->pap->CTRLNodeLLHead->EXT.y=8;
				newnode->pap->CTRLNodeLLHead->NEXT=allocate_a_new_CTRLnode();
				newnode->pap->CTRLNodeLLHead->NEXT->ID=CONTROLDOWN;
				newnode->pap->CTRLNodeLLHead->NEXT->TL.x=28;
				newnode->pap->CTRLNodeLLHead->NEXT->TL.y=8;
				newnode->pap->CTRLNodeLLHead->NEXT->EXT.x=4;
				newnode->pap->CTRLNodeLLHead->NEXT->EXT.y=8;
			break;
		   	case LOGICINDICATORBitmap:
				newnode->pap = allocate_a_new_pap();
				newnode->pap->BITMAPID = bnp->pap->BITMAPID;
				newnode->pap->XY.x=10;
				newnode->pap->XY.y=20;
				newnode->pap->SIZE.x=bnp->pap->SIZE.x;
				newnode->pap->SIZE.y=bnp->pap->SIZE.y;
		   	break;
		   	case LOGICCTRLBitmap:
				newnode->pap = allocate_a_new_pap();
				newnode->pap->BITMAPID = bnp->pap->BITMAPID;
				newnode->pap->XY.x=10;
				newnode->pap->XY.y=20;
				newnode->pap->SIZE.x=bnp->pap->SIZE.x;
				newnode->pap->SIZE.y=bnp->pap->SIZE.y;
				newnode->pap->CTRLNodeLLHead=allocate_a_new_CTRLnode();
				newnode->pap->CTRLNodeLLHead->ID=CONTROLPUSH;
				newnode->pap->CTRLNodeLLHead->TL.x=0;
				newnode->pap->CTRLNodeLLHead->TL.y=0;
				newnode->pap->CTRLNodeLLHead->EXT.x=16;
				newnode->pap->CTRLNodeLLHead->EXT.y=32;
			break;

		}
	break;
   }// switch

// 插入IO 節點

   switch(bnp->bap->BITMAPID)
   {
   	case INDICATOR8BLOCKBitmap:
		newnode->bap->IONodeLLHead = allocate_a_new_IOnode();
		newnode->bap->IONodeLLHead->ID=0;
		newnode->bap->IONodeLLHead->TYPE=1;
		newnode->bap->IONodeLLHead->P = MemHandleLock(MemHandleNew(sizeof(int)));
		(*(int *)newnode->bap->IONodeLLHead->P)=0;
		newnode->bap->IONodeLLHead->BYTES=sizeof(int);
		newnode->bap->IONodeLLHead->NUMS=1;
		newnode->bap->IONodeLLHead->TL.x=0;
		newnode->bap->IONodeLLHead->TL.y=0;
		newnode->bap->IONodeLLHead->EXT.x=32;
		newnode->bap->IONodeLLHead->EXT.y=16;
   	break;
   	case CTRLU8BLOCKBitmap:
		newnode->bap->IONodeLLHead = allocate_a_new_IOnode();
		newnode->bap->IONodeLLHead->ID=0;
		newnode->bap->IONodeLLHead->TYPE=0;
		newnode->bap->IONodeLLHead->P = MemHandleLock(MemHandleNew(sizeof(int)));
		(*(int *)newnode->bap->IONodeLLHead->P)=0;
		newnode->bap->IONodeLLHead->BYTES=sizeof(int);
		newnode->bap->IONodeLLHead->NUMS=1;
		newnode->bap->IONodeLLHead->TL.x=0;
		newnode->bap->IONodeLLHead->TL.y=0;
		newnode->bap->IONodeLLHead->EXT.x=32;
		newnode->bap->IONodeLLHead->EXT.y=16;
   	break;
   	case LOGICINDICATORBitmap:
		newnode->bap->IONodeLLHead = allocate_a_new_IOnode();
		newnode->bap->IONodeLLHead->ID=0;
		newnode->bap->IONodeLLHead->TYPE=1;
		newnode->bap->IONodeLLHead->P = MemHandleLock(MemHandleNew(sizeof(int)));
		(*(int *)newnode->bap->IONodeLLHead->P)=0;
		newnode->bap->IONodeLLHead->BYTES=sizeof(int);
		newnode->bap->IONodeLLHead->NUMS=1;
		newnode->bap->IONodeLLHead->TL.x=0;
		newnode->bap->IONodeLLHead->TL.y=0;
		newnode->bap->IONodeLLHead->EXT.x=32;
		newnode->bap->IONodeLLHead->EXT.y=16;
   	break;
   	case LOGICCTRLBitmap:
		newnode->bap->IONodeLLHead = allocate_a_new_IOnode();
		newnode->bap->IONodeLLHead->ID=0;
		newnode->bap->IONodeLLHead->TYPE=0;
		newnode->bap->IONodeLLHead->P = MemHandleLock(MemHandleNew(sizeof(int)));
		(*(int *)newnode->bap->IONodeLLHead->P)=0;
		newnode->bap->IONodeLLHead->BYTES=sizeof(int);
		newnode->bap->IONodeLLHead->NUMS=1;
		newnode->bap->IONodeLLHead->TL.x=0;
		newnode->bap->IONodeLLHead->TL.y=0;
		newnode->bap->IONodeLLHead->EXT.x=32;
		newnode->bap->IONodeLLHead->EXT.y=16;
   	break;
   	
   	case ADDBitmap:
   	case SUBBitmap:
   	case MULBitmap:
   	case DIVBitmap:
   	case ANDBitmap:
   	case ORBitmap:
   	case ABOVEBitmap:
   	case LESSBitmap:
   	case EQUBitmap:
   	case ABOVEEQUBitmap:
   	case LESSEQUBitmap:
	ionode = allocate_a_new_IOnode();
	ionode->ID = 0;
	ionode->TYPE = 1;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=0;
	ionode->TL.y=0;
	ionode->EXT.x=8;
	ionode->EXT.y=8;
	newnode->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newnode->bap->IONodeLLHead,
				ionode);
	
	ionode = allocate_a_new_IOnode();
	ionode->ID = 1;
	ionode->TYPE = 1;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=0;
	ionode->TL.y=8;
	ionode->EXT.x=8;
	ionode->EXT.y=8;
	newnode->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newnode->bap->IONodeLLHead,
				ionode);
	
	ionode = allocate_a_new_IOnode();
	ionode->ID = 2;
	ionode->TYPE = 0;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=8;
	ionode->TL.y=0;
	ionode->EXT.x=8;
	ionode->EXT.y=16;
	newnode->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newnode->bap->IONodeLLHead,
				ionode);
	
	break;
	case NOTBitmap:
	ionode = allocate_a_new_IOnode();
	ionode->ID = 0;
	ionode->TYPE = 1;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=0;
	ionode->TL.y=0;
	ionode->EXT.x=8;
	ionode->EXT.y=16;
	newnode->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newnode->bap->IONodeLLHead,
				ionode);
	
	ionode = allocate_a_new_IOnode();
	ionode->ID = 1;
	ionode->TYPE = 0;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=8;
	ionode->TL.y=0;
	ionode->EXT.x=8;
	ionode->EXT.y=16;
	newnode->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newnode->bap->IONodeLLHead,
				ionode);
	
	break;
   }
   //插入預設元件
   switch(bnp->bap->BITMAPID)
   {
   	case FORLOOPBitmap:
   		ctrlblock1=allocate_a_new_node();// N
   		ctrlblock2=allocate_a_new_node();// i
   		ctrlblock1->TYPE=CTRLBLOCK;
   		ctrlblock1->bap=allocate_a_new_bap();
   		ctrlblock1->bap->BITMAPID=FORLOOPNBitmap;
   		ctrlblock1->bap->XY.x=0;
   		ctrlblock1->bap->XY.y=5;
   		ctrlblock1->bap->SIZE.x=10;
   		ctrlblock1->bap->SIZE.y=10;
   		ctrlblock1->bap->IONodeLLHead=allocate_a_new_IOnode();
   		ctrlblock1->bap->IONodeLLHead->ID=0;
   		ctrlblock1->bap->IONodeLLHead->TYPE=1;
   		ctrlblock1->bap->IONodeLLHead->P= MemHandleLock(MemHandleNew(sizeof(long)));
   		(*(long *)ctrlblock1->bap->IONodeLLHead->P)=0 ;
   		ctrlblock1->bap->IONodeLLHead->BYTES=sizeof(long);
   		ctrlblock1->bap->IONodeLLHead->TL.x=0;
   		ctrlblock1->bap->IONodeLLHead->TL.y=0;
   		ctrlblock1->bap->IONodeLLHead->EXT.x=10;
   		ctrlblock1->bap->IONodeLLHead->EXT.y=10;
   		ctrlblock1->bap->ReDraw=true;
   		ctrlblock1->bap->Clean=true;
   		ctrlblock1->SelfBlockLLHead=newnode;
   		ctrlblock1->BackgroundBlockLLHead=newnode;
   		//((HOOKP)newnode->LoopBlockHOOKP)->BlockLLHeadP
   		((HOOKP)newnode->LoopBlockHOOKP)->BlockLLHeadP=
   		InsertIntoBlockLLwithUID(((HOOKP)newnode->LoopBlockHOOKP)->BlockLLHeadP,ctrlblock1);
   		ctrlblock2->TYPE=CTRLBLOCK;
   		ctrlblock2->bap=allocate_a_new_bap();
   		ctrlblock2->bap->BITMAPID=FORLOOPIBitmap;
   		ctrlblock2->bap->XY.x=0;
   		ctrlblock2->bap->XY.y=17;
   		ctrlblock2->bap->SIZE.x=10;
   		ctrlblock2->bap->SIZE.y=10;
   		ctrlblock2->bap->IONodeLLHead=allocate_a_new_IOnode();
   		ctrlblock2->bap->IONodeLLHead->ID=0;
   		ctrlblock2->bap->IONodeLLHead->TYPE=0;
   		ctrlblock2->bap->IONodeLLHead->P= MemHandleLock(MemHandleNew(sizeof(long)));
   		(*(long *)ctrlblock2->bap->IONodeLLHead->P)=0 ;
   		ctrlblock2->bap->IONodeLLHead->BYTES=sizeof(long);
   		ctrlblock2->bap->IONodeLLHead->TL.x=0;
   		ctrlblock2->bap->IONodeLLHead->TL.y=0;
   		ctrlblock2->bap->IONodeLLHead->EXT.x=10;
   		ctrlblock2->bap->IONodeLLHead->EXT.y=10;
   		ctrlblock2->bap->ReDraw=true;
   		ctrlblock2->bap->Clean=true;
   		ctrlblock2->SelfBlockLLHead=newnode;
   		ctrlblock2->BackgroundBlockLLHead=newnode;
   		((HOOKP)newnode->LoopBlockHOOKP)->BlockLLHeadP=
   		InsertIntoBlockLLwithUID(((HOOKP)newnode->LoopBlockHOOKP)->BlockLLHeadP,ctrlblock2);
   	break;
   	case WHILELOOPBitmap:
ctrlblock1=allocate_a_new_node();
ctrlblock1->TYPE=CTRLBLOCK;
ctrlblock1->bap=allocate_a_new_bap();
ctrlblock1->bap->BITMAPID=WHILELOOPLOOPBitmap;
ctrlblock1->bap->XY.x=newnode->bap->SIZE.x-10;
ctrlblock1->bap->XY.y=newnode->bap->SIZE.y-18;
ctrlblock1->bap->SIZE.x=10;
ctrlblock1->bap->SIZE.y=10;
ctrlblock1->bap->IONodeLLHead = allocate_a_new_IOnode();
ctrlblock1->bap->IONodeLLHead->ID=0;
ctrlblock1->bap->IONodeLLHead->TYPE=1;
ctrlblock1->bap->IONodeLLHead->P = MemHandleLock(MemHandleNew(sizeof(int)));
ctrlblock1->bap->IONodeLLHead->BYTES=sizeof(int);
ctrlblock1->bap->IONodeLLHead->NUMS=1;
ctrlblock1->bap->IONodeLLHead->TL.x=0;
ctrlblock1->bap->IONodeLLHead->TL.y=0;
ctrlblock1->bap->IONodeLLHead->EXT.x=10;
ctrlblock1->bap->IONodeLLHead->EXT.y=10;

ctrlblock1->SelfBlockLLHead=newnode;
ctrlblock1->BackgroundBlockLLHead=newnode;
ctrlblock1->bap->ReDraw=true;
ctrlblock1->bap->Clean=true;
((HOOKP)newnode->LoopBlockHOOKP)->BlockLLHeadP=
InsertIntoBlockLLwithUID(((HOOKP)newnode->LoopBlockHOOKP)->BlockLLHeadP,ctrlblock1);

   	break;
   	case SWITCHCASEBitmap:
   	
   	// 未完成
   	
   	break;
   }
   
	
	newnode->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
	newnode->BackgroundBlockLLHead=SYSHOOK->BlockLLHeadP;
	newnode->bap->ReDraw=true;
	newnode->bap->Clean=true;
	SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,newnode);
}
/***********************************************************************
*	"Functions" event process routine
*	
*	
*	
*	
*	
*	
************************************************************************/
Boolean FunctionsFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;
    
	//UInt16 x,y;
	//int i;
	//RectangleType Rtest;
	InAreaInfType InAreaInf;
	
	switch (eventP->eType) 
		{
		case frmOpenEvent:
			//WinEraseWindow();
			frmP = FrmGetActiveForm();
			FrmDrawForm ( frmP);
			/*
			Rtest.topLeft.x=0;
			Rtest.topLeft.y=10;
			Rtest.extent.x=160;
			Rtest.extent.y=16;
			WinDrawRectangle(&Rtest,0);
			*/
			// 畫出 function palette
			DrawToolCatagory();
			CatagoryIn = false;
			CatagoryBNP = NULL;
			handled = true;
			break;
		case penDownEvent:
			if(CatagoryIn)
				InAreaInf = IsInArea(InAreaInf,((HOOKP)CatagoryBNP->hookP)->BlockLLHeadP,eventP->screenX,eventP->screenY,0,0);
			else
				InAreaInf = IsInArea(InAreaInf,FunctionCatagoryLLHead,eventP->screenX,eventP->screenY,0,0);
			if(InAreaInf.InArea)
			{
				if(CatagoryIn) 
				{// 進入類別中
				 if(InAreaInf.llp->TYPE == 8000)
				 {
					AddToSYSHOOK(InAreaInf.llp);
					
					FrmGotoForm(BlockDiagramForm);
				 }
					CatagoryBNP->bap->XY.x = Catx;
					CatagoryBNP->bap->XY.y = Caty;
					CatagoryBNP->bap->SIZE.x = Catextx;
					CatagoryBNP->bap->SIZE.y = Catexty;
					CatagoryIn = false;
					CatagoryBNP = NULL;
				}
				else if(((HOOKP)InAreaInf.llp->hookP) != NULL)
				{
					// 改變catagory ICON的大小
					Catx	= InAreaInf.llp->bap->XY.x;
					Caty	= InAreaInf.llp->bap->XY.y;
					Catextx= InAreaInf.llp->bap->SIZE.x;
					Catexty= InAreaInf.llp->bap->SIZE.y;
					InAreaInf.llp->bap->XY.x = 0;
					InAreaInf.llp->bap->XY.y = 0;
					InAreaInf.llp->bap->SIZE.x = 160;
					InAreaInf.llp->bap->SIZE.y = 160;
					WinEraseWindow();
					frmP = FrmGetActiveForm();
					FrmDrawForm ( frmP);
					DrawToolIcon(((HOOKP)InAreaInf.llp->hookP)->BlockLLHeadP);
					CatagoryIn = true;
					CatagoryBNP = InAreaInf.llp;
				}
				/////////////////////////////////////////
				//
				// 5-31 處理 自訂元件
				//
				if(InAreaInf.llp->bap->BITMAPID == FUNCCUSTBitmap)
				{
					CUSTOMLOAD=true;
					CUSTUMHOOK.BlockLLHeadP=NULL;
					CUSTUMHOOK.WireLLHeadP=NULL;
					FrmGotoForm(LOADForm);
				}
			}else{
				if(CatagoryBNP != NULL)
				{
					CatagoryBNP->bap->XY.x = Catx;
					CatagoryBNP->bap->XY.y = Caty;
					CatagoryBNP->bap->SIZE.x = Catextx;
					CatagoryBNP->bap->SIZE.y = Catexty;
					CatagoryIn = false;
					CatagoryBNP = NULL;
				}
				FrmReturnToForm(0);
			}
		
			handled = true;
			break;
		case penMoveEvent:

			handled = true;
			break;
		case penUpEvent:
		
			handled = true;
			break;
		case nilEvent:
			if(CUSTOMLOAD)
			{
				CUSTOMLOAD=false;
				// 加入串列
				// CUSTUMHOOK
				if(CUSTUMHOOK.BlockLLHeadP!=NULL)
					AddCUSTtoSYSHOOK();
				FrmGotoForm(BlockDiagramForm);
			}
			handled = true;
			break;
		default:
			break;
		
		}
	
	return handled;
}
