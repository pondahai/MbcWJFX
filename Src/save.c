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

	MemHandleResize(strH,MemHandleSize(strH)+StrLen(str));			//�վ�j�p
	strP=MemHandleLock(strH);								// ��
	StrNCat(strP,str,MemHandleSize(strH));					//�걵
	MemHandleUnlock(strH);									// ����
}
/***********************************************************************
 * 2003-5-20
 * 2003-5-31 �p�G�O HOOKBLOCK ���ܨä��|�h�B�z �䤺����C
 *           �ӬO�ȶȰO�� �o�� HOOKBLOCK �Ӥw ���� LOAD ���ɭ�
 *           �~�|�h�B�z
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
			
			//LoopBlockHOOKP �u�|���@�� ����end
			if(bnp->TYPE==LOOPBLOCK)
			{
				AddString(strH,"NEWLOOPHOOK\n");
				AddString(strH,"NEWHOOK\n");
				//���j�I�s
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
				//���j�I�s
				DataStruct2ASCII(strH,hookp->BlockLLHeadP,hookp->WireLLHeadP);
				hookp=hookp->NEXT;
				if(hookp == (HOOKP)bnp->CaseBlockHOOKLLHeadP)//�]�� case��loop�O�`����
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
		// 5-31 �M�w HOOKBLOCK ��ID��0
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
			// �[�J�u�q �ϥ� StartStopAddIntoWLL()
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
			// �[�J�u�q �ϥ� StartStopAddIntoWLL()
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
 * 2003-05-24 ���file stream ���覡
 ***********************************************************************/
void SAVE(char *filename )
{
	VoidHand strH;
	char *strP;
	int i;
	FileHand savefile;
	
// �����Ƶ��c
/*
	UInt16 ID;
	UInt16 TYPE;
	BAP bap;					// ����
	PAP pap;					// ����
	Ptr LoopBlockHOOKP;			// ����
	Ptr CurrentHOOKP;			// ����
	Ptr CaseBlockHOOKLLHeadP; 	// ����
	Ptr hookP;					// ����
	char *filename;				// ����
	struct BlockNode *SelfBlockLLHead;		// ���� ���s
	struct BlockNode *BackgroundBlockLLHead;// ���� ���s
	struct BlockNode *PREVNODE;				// ���� ���s
	struct BlockNode *NEXTNODE;				// ���� ���s
*/
// bap ��Ƶ��c
/*
	UInt16 BITMAPID;			//
	struct Point XY;			//
	struct Point SIZE;			//
	IONODEP 	IONodeLLHead;	// ����
	UInt16 		IONodeNUM;		// ���s
	CTRLNODEP 	CTRLNodeLLHead;	// ����
	Boolean DrawBorder;			// ���s
	Boolean ReDraw;				// ���s
	Boolean Clean; 				// ���s
 
*/
// pap ��Ƶ��c
/*
	UInt16 BITMAPID;
	struct Point XY;
	struct Point SIZE;	
	CTRLNODEP CTRLNodeLLHead;	// ����
*/
// IONODEP
/*
	UInt16 ID;					
	UInt16 TYPE; 				// �s
	Boolean NotReady;
	Ptr P;						// ���e�� 
	UInt16 BYTES;
	UInt16 NUMS;
	struct Point TL; 			// �s
	struct Point EXT;			// �s
	struct IONODE *PREV;
	struct IONODE *NEXT;
*/
// CTRLNODEP
/*
	UInt16 ID;
	struct Point TL;			// �s
	struct Point EXT;			// �s
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
	//�} db
	//OpenDB(filename);
	//
	// 5-25 �ϥ�file stream �覡�s��
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
					//5-31 �bhook block�W����IO�I �P�[�J �t�u�q
					// �쥻 hook block �W��IO�I �j���
					SYSHOOK->BlockLLHeadP->bap->IONodeLLHead=NULL;
					// �٦��s����wire�]�n���h ���M�|�b�ĤG���s�ɮ� �o�ͽu�q���� �M��b custom ���J�ɵo�ͥd��
					SYSHOOK->WireLLHeadP=RemoveDarkWire(SYSHOOK->WireLLHeadP,SYSHOOK->BlockLLHeadP);
					// �إ߷s�� IO�I �P �s���u�q
					SYSHOOK=FindInputNode(SYSHOOK,SYSHOOK->BlockLLHeadP,SYSHOOK->BlockLLHeadP->NEXTNODE,&IOID);
					SYSHOOK=FindOutputNode(SYSHOOK,SYSHOOK->BlockLLHeadP,SYSHOOK->BlockLLHeadP->NEXTNODE,&IOID);
					
					// ���@�ܼƥΧ���n�O�o������
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
