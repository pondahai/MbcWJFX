#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"
#include "FileStream.h"

#include "vpl.h"


extern DmOpenRef   db;
extern UInt32      numRecords;
extern VoidHand	ChoicesHandle , ChoicesPtrsHandle;
extern HOOKP SYSHOOK;
extern Boolean CUSTOMLOAD;		// from function.c
extern struct HOOK CUSTUMHOOK;	// from function.c

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
extern WNP allocate_a_new_wire_node();
extern WNP InsertIntoWireLL(WNP Head,WNP node);

HOOKP LOAD(HOOKP sysHead,char *filename );

int ConvertFileName2RecordIndex(char *filename)
{
	char	str[20];
	int		itemIndex;
	Handle			recHandle;
	Ptr				recPtr;
	
	OpenDB("SAVETEST");

	numRecords = DmNumRecords(db);
	if (numRecords)
		{
		for (itemIndex = 0; itemIndex < numRecords; itemIndex++)
			{
			// Retrieve the record from the database and lock it down.
			recHandle = DmGetRecord(db, itemIndex);
			recPtr = MemHandleLock(recHandle);
			
			StrCopy(str,recPtr);
			if(StrCompare(str,filename) == 0)
			{
				MemHandleUnlock(recHandle);
				DmReleaseRecord(db, itemIndex, false);
				break; // jump out of the for loop
			}
			
			// Unlock the handle to the record.
			MemHandleUnlock(recHandle);
			
			// Release the record, not dirty.
			DmReleaseRecord(db, itemIndex, false);
			}
		
		}
	

	CloseDB();
	return itemIndex;
}

/***********************************************************************
 * 2003-4-12
 * 2003-5-25 使用 database 方式 建立表格 每一個database代表一個存檔
 *
 ***********************************************************************/

 void BuildLOADMenu(void )
{
	//int WLn,BLn;
	//BlockLink BL;
	//WireLink WL;
	//Handle			recHandle;
	Int				itemIndex, textLen, choicesOffset = 0;
	ListPtr			lst;
	FormPtr			frm;
	//Ptr				recPtr;
	CharPtr			choices;
	char 			str[50];
	char			filename[32];
	UInt16			i;
	Err				error;
	DmSearchStateType stateInfoP;
	UInt cardNoP;
	LocalID dbIDP;
	UInt16 numDBs=0;
	
	//OpenDB("SAVETEST");
	frm = FrmGetActiveForm();
	
		// Prepare to setup the contents of the list for the main form.
		
		// Get a pointer to the list object.
		itemIndex = FrmGetObjectIndex(frm, LOADFileListList);
		lst = FrmGetObjectPtr(frm, itemIndex);
	
		// Get the usable width of the list rectangle.
		//FrmGetObjectBounds (frm, itemIndex, &lstRect);
		//lstWidth = lstRect.extent.x - 2;
		
	// start a new search
	if(DmGetNextDatabaseByTypeCreator(true, 
	&stateInfoP,
	appDBType, 
	appCreatorID, 
	false,
	&cardNoP, 
	&dbIDP)==0)
	{
		numDBs=1;

		// Allocate an initial block for the list choices.
		ChoicesHandle = MemHandleNew(sizeof(char));
	
		// Lock down the block and set it's initial value to an empty string.
		choices = MemHandleLock(ChoicesHandle);
		*choices = 0;


		DmDatabaseInfo (0, dbIDP,filename, 
NULL,NULL, NULL,NULL, NULL,NULL, NULL,NULL, NULL,NULL);

			
			//////////////////////
			StrCopy(str,filename);
			textLen = StrLen(str);
			
			// Grow the choices buffer to accomodate the new string. We must unlock
			// the chunk so that the Memory Manager can move the chunk if neccessary to
			// grow it.
			MemHandleUnlock(ChoicesHandle);
			error = MemHandleResize(ChoicesHandle, textLen + choicesOffset + sizeof('\0'));
			choices = MemHandleLock(ChoicesHandle);
			
			// Check for fatal error.
			ErrFatalDisplayIf(error, "Could not grow choices for list.");
		
			// Copy the text from the record to the choices buffer.
			for (i = 0; i < textLen; i++)
				choices[choicesOffset + i] = str[i];
	
			// Update the end of choices offset and set a zero terminator 
			// on the string in the choices buffer.
			choicesOffset += textLen;
			choices[choicesOffset++] = 0;
	


	// continue search
	while(DmGetNextDatabaseByTypeCreator(false, 
	&stateInfoP,
	appDBType, 
	appCreatorID, 
	false,
	&cardNoP, 
	&dbIDP)==0)
	{
		DmDatabaseInfo (0, dbIDP,
filename, NULL,NULL, NULL,NULL, NULL,NULL, NULL,NULL, NULL,NULL);	

			//////////////////////
			StrCopy(str,filename);
			textLen = StrLen(str);
			
			// Grow the choices buffer to accomodate the new string. We must unlock
			// the chunk so that the Memory Manager can move the chunk if neccessary to
			// grow it.
			MemHandleUnlock(ChoicesHandle);
			error = MemHandleResize(ChoicesHandle, textLen + choicesOffset + sizeof('\0'));
			choices = MemHandleLock(ChoicesHandle);
			
			// Check for fatal error.
			ErrFatalDisplayIf(error, "Could not grow choices for list.");
		
			// Copy the text from the record to the choices buffer.
			for (i = 0; i < textLen; i++)
				choices[choicesOffset + i] = str[i];
	
			// Update the end of choices offset and set a zero terminator 
			// on the string in the choices buffer.
			choicesOffset += textLen;
			choices[choicesOffset++] = 0;

		numDBs++;
	}// while

		// Create an array of pointers from the choices strings.
		ChoicesPtrsHandle = SysFormPointerArrayToStrings(choices, numDBs);
		
		// Set the list choices from the array of pointers.
		LstSetListChoices(lst, MemHandleLock(ChoicesPtrsHandle), numDBs);


	}// if
	/*
	frm = FrmGetActiveForm();
	
	// Get the number of records currently in the application's database.
	numRecords = DmNumRecords(db);
	if (numRecords)
		{
		// Prepare to setup the contents of the list for the main form.
		
		// Get a pointer to the list object.
		itemIndex = FrmGetObjectIndex(frm, LOADFileListList);
		lst = FrmGetObjectPtr(frm, itemIndex);
	
		// Get the usable width of the list rectangle.
		//FrmGetObjectBounds (frm, itemIndex, &lstRect);
		//lstWidth = lstRect.extent.x - 2;
		
		// Allocate an initial block for the list choices.
		ChoicesHandle = MemHandleNew(sizeof(char));
	
		// Lock down the block and set it's initial value to an empty string.
		choices = MemHandleLock(ChoicesHandle);
		*choices = 0;
		
		// Build up the choices.  
		// A sequence of strings packed one after another, one for each record.
		for (itemIndex = 0; itemIndex < numRecords; itemIndex++)
			{
			// Retrieve the record from the database and lock it down.
			recHandle = DmGetRecord(db, itemIndex);
			recPtr = MemHandleLock(recHandle);
			
			// Determine the length of text that will fit within the list bounds.
			//i = lstWidth;
			//textLen = StrLen(recText);
			//FntCharsInWidth(recText, &i, &textLen, &fits);
			
			//temp = (recPtr[0] << 8) + recPtr[1];
			//StrPrintF(str,"%d",temp);
			StrCopy(str,recPtr);
			textLen = StrLen(str);
			
			// Grow the choices buffer to accomodate the new string. We must unlock
			// the chunk so that the Memory Manager can move the chunk if neccessary to
			// grow it.
			MemHandleUnlock(ChoicesHandle);
			error = MemHandleResize(ChoicesHandle, textLen + choicesOffset + sizeof('\0'));
			choices = MemHandleLock(ChoicesHandle);
			
			// Check for fatal error.
			ErrFatalDisplayIf(error, "Could not grow choices for list.");
		
			// Copy the text from the record to the choices buffer.
			for (i = 0; i < textLen; i++)
				choices[choicesOffset + i] = str[i];
	
			// Update the end of choices offset and set a zero terminator 
			// on the string in the choices buffer.
			choicesOffset += textLen;
			choices[choicesOffset++] = 0;
	
			// Unlock the handle to the record.
			MemHandleUnlock(recHandle);
			
			// Release the record, not dirty.
			DmReleaseRecord(db, itemIndex, false);
			}
		
		// Create an array of pointers from the choices strings.
		ChoicesPtrsHandle = SysFormPointerArrayToStrings(choices, numRecords);
		
		// Set the list choices from the array of pointers.
		LstSetListChoices(lst, MemHandleLock(ChoicesPtrsHandle), numRecords);
		}
	
	// Draw the form.
	//FrmDrawForm(frm);

	CloseDB();
	*/
}
/***********************************************************************
 * 2003-5-26 
 ***********************************************************************/
IONODEP FindIONodeByID(IONODEP ionodellhead,UInt16 ID)
{
	while(ionodellhead != NULL)
	{
		if(ionodellhead->ID == ID)
			break;
		ionodellhead = ionodellhead->NEXT;
	}
	return ionodellhead;
}
// 5-30 加入巢狀探尋
BNP FindNodeByID(BNP HeadP,UInt16 ID)
{
	BNP FindOutBNP;
	
	while(HeadP != NULL)
	{
		if(HeadP->ID == ID)
			break;
		if(HeadP->TYPE==LOOPBLOCK)
		{
				if((FindOutBNP=FindNodeByID(((HOOKP)HeadP->LoopBlockHOOKP)->BlockLLHeadP,ID))!=NULL)
				{
					HeadP=FindOutBNP;
					break;
				}//else{
				//HeadP = HeadP->NEXTNODE;
				//}
		}
		else if(HeadP->TYPE==CASEBLOCK)
		{
				// 未完成
		}
		else
		{
				//HeadP = HeadP->NEXTNODE;
		
		}
		HeadP = HeadP->NEXTNODE;

	}
	return HeadP;
}

/***********************************************************************
 * 2003-5-26 新版
 ***********************************************************************/
 
int read_a_str(char *str,FileHand file)
{
	int i;
	// 讀入一串字 '\n' 結尾
	for(i=0;;i++)
	{
		FileRead(file,&str[i],1,1,NULL);
		if((FileEOF(file)!=0)||(str[i]=='\n'))
		 break;
	}
	str[i]='\0';
	// 完成一串字
	
	return StrLen(str);
}
/***********************************************************************
 * hook 目前所用虎克 selfBlockLLHead 所屬背景串列 file 檔案handle
 * 2003-5-26 新版
 * 2003-5-31 要增加 自訂元件 的功能 遇到 HOOKBLOCK 除了本身BLOCK的建立外
 *           還要作的事就是 遞迴 LOAD 
 ***********************************************************************/
HOOKP parse_loop(HOOKP hook,BNP SelfBlockLLHead,FileHand file)
{
	char str[30];
	BNP newnode;
	IONODEP ionode;
	CTRLNODEP ctrlnode;
	WNP newwire;
	BNP hook_block;
	BNP Rbnp;//5-31 因為遞迴LOAD之後 而需要重設 串列背景 而設的
	
	str[0]='\0';
	
	// 讀入記憶體
	//read_a_str(str,file);
	while(1)
	{
		read_a_str(str,file);
		if(StrCompare(str,"STARTBLOCK") == 0)
		{
			break;
		}
	}
	while(1)
	{
		read_a_str(str,file); // NEWNODE or ENDBLOCK
		if(StrCompare(str,"ENDBLOCK") == 0)
		{
			break;
		}
		// 處理 STARTBLOCK 內部
		newnode=allocate_a_new_node();

		// 5-30 設定最高node
		// 不用設了 因為 這時的hook就是SYSHOOK 
		// 5-31 要將newnode 提早插入 為了讓 中途的虎克 例如 while loop 及 for loop 等
		/*
		if((SelfBlockLLHead==NULL) && (hook->BlockLLHeadP==NULL))
		{
			// 例外作法 因為下面wire載入時 需要作整個node串的探尋
			// 也會有機會 執行巢狀探尋 因此需要找出最高node
			SYSHOOK->BlockLLHeadP=newnode;
		}
		*/
		
		read_a_str(str,file); // ID
		read_a_str(str,file); // 
		newnode->ID=StrAToI(str);

		// 5-31  提早將 block 插入串列
		//       有個情況必須說明
		//       如果本次進來 是 loop 的遞迴 那麼
		//       在這裡設定 hook->的BLockLLHead 其實就是往 slefblockllhead 的 forloop->hook 
		//       內部設定 因為這時他們是一體的
		hook->BlockLLHeadP=InsertIntoBlockLLNoUID(hook->BlockLLHeadP,newnode);

		// 5-31 如果是第1次進來 Self 會是nuLL 所以要在這裡作上連接
		if((hook->BlockLLHeadP!=NULL) && (SelfBlockLLHead==NULL))
			SelfBlockLLHead=hook->BlockLLHeadP;

		
		read_a_str(str,file); // TYPE
		read_a_str(str,file); // 
		newnode->TYPE=StrAToI(str);
		read_a_str(str,file); // NEWBAP 一定有bap
		newnode->bap=allocate_a_new_bap();
			read_a_str(str,file); // BITMAPID
			read_a_str(str,file); //
			newnode->bap->BITMAPID=StrAToI(str);
			read_a_str(str,file); // XY
			read_a_str(str,file); //
			newnode->bap->XY.x=StrAToI(str);
			read_a_str(str,file); //
			newnode->bap->XY.y=StrAToI(str);
			read_a_str(str,file); // SIZE
			read_a_str(str,file); // 
			newnode->bap->SIZE.x=StrAToI(str);
			read_a_str(str,file); // 
			newnode->bap->SIZE.y=StrAToI(str);
			read_a_str(str,file); // NEWIONODEP or not
			if(StrCompare(str,"NEWIONODEP") == 0)
			{
				read_a_str(str,file); // 
				while(StrCompare(str,"ADDIONODE") == 0)
				{
					ionode=allocate_a_new_IOnode();
					read_a_str(str,file); // ID
					read_a_str(str,file); // 
					ionode->ID=StrAToI(str);
					read_a_str(str,file); // TYPE
					read_a_str(str,file); //
					ionode->TYPE=StrAToI(str);
					read_a_str(str,file); // BYTES
					read_a_str(str,file); //
					ionode->BYTES=StrAToI(str);
					read_a_str(str,file); // VALUE
					read_a_str(str,file); // 
					ionode->P=MemHandleLock(MemHandleNew(ionode->BYTES));
					switch(ionode->BYTES)
					{
						case 2:
						*((int *)ionode->P)=StrAToI(str);
						break;
						case 4:
						*((long *)ionode->P)=StrAToI(str);
						break;
						default:
						*((int *)ionode->P)=0;
						break;
					}
					read_a_str(str,file); // TL
					read_a_str(str,file); // 
					ionode->TL.x=StrAToI(str);
					read_a_str(str,file); // 
					ionode->TL.y=StrAToI(str);
					read_a_str(str,file); // SIZE
					read_a_str(str,file); // 
					ionode->EXT.x=StrAToI(str);
					read_a_str(str,file); // 
					ionode->EXT.y=StrAToI(str);
					read_a_str(str,file); // next str
					newnode->bap->IONodeLLHead=InsertIntoIONodesLL(newnode->bap->IONodeLLHead,ionode);
				}
			}
			if(StrCompare(str,"NEWCTRLNODEP") == 0)
			{
				read_a_str(str,file); // 
				while(StrCompare(str,"ADDCTRLNODE") == 0)
				{
					ctrlnode=allocate_a_new_CTRLnode();
					read_a_str(str,file); // ID
					read_a_str(str,file); // 
					ctrlnode->ID=StrAToI(str);
					read_a_str(str,file); // TL
					read_a_str(str,file); // 
					ctrlnode->TL.x=StrAToI(str);
					read_a_str(str,file); // 
					ctrlnode->TL.y=StrAToI(str);
					read_a_str(str,file); // SIZE
					read_a_str(str,file); // 
					ctrlnode->EXT.x=StrAToI(str);
					read_a_str(str,file); // 
					ctrlnode->EXT.y=StrAToI(str);
					read_a_str(str,file); // next str
					newnode->bap->CTRLNodeLLHead=InsertIntoCTRLNodesLL(newnode->bap->CTRLNodeLLHead,ctrlnode);
				}
			}
		// BAP完成
		while(1)
		{
			if(StrCompare(str,"ENDBAP") == 0)
				break;
		}
		// NEWPAP or not
		read_a_str(str,file);
		if(StrCompare(str,"NEWPAP") == 0)
		{
			newnode->pap=allocate_a_new_pap();
				read_a_str(str,file); // BITMAPID
				read_a_str(str,file); //
				newnode->pap->BITMAPID=StrAToI(str);
				read_a_str(str,file); // XY
				read_a_str(str,file); //
				newnode->pap->XY.x=StrAToI(str);
				read_a_str(str,file); //
				newnode->pap->XY.y=StrAToI(str);
				read_a_str(str,file); // SZIE
				read_a_str(str,file); //
				newnode->pap->SIZE.x=StrAToI(str);
				read_a_str(str,file); //
				newnode->pap->SIZE.y=StrAToI(str);
				read_a_str(str,file); // NEWCTRLNODEP or not
				if(StrCompare(str,"NEWCTRLNODEP") == 0)
				{
					read_a_str(str,file); // 
					while(StrCompare(str,"ADDCTRLNODE") == 0)
					{
						ctrlnode=allocate_a_new_CTRLnode();
						read_a_str(str,file); // ID
						read_a_str(str,file); // 
						ctrlnode->ID=StrAToI(str);
						read_a_str(str,file); // TL
						read_a_str(str,file); // 
						ctrlnode->TL.x=StrAToI(str);
						read_a_str(str,file); // 
						ctrlnode->TL.y=StrAToI(str);
						read_a_str(str,file); // SIZE
						read_a_str(str,file); // 
						ctrlnode->EXT.x=StrAToI(str);
						read_a_str(str,file); // 
						ctrlnode->EXT.y=StrAToI(str);
						read_a_str(str,file); // next str
						newnode->pap->CTRLNodeLLHead=InsertIntoCTRLNodesLL(newnode->pap->CTRLNodeLLHead,ctrlnode);
					}
				}
			// 跳過 ENDPAP
			read_a_str(str,file); // next str
		}
		// PAP 完成
		// NEWLOOPHOOK or not
		if(StrCompare(str,"NEWLOOPHOOK") == 0)
		{
			read_a_str(str,file); // NEWHOOK
			(HOOKP)newnode->LoopBlockHOOKP=allocate_a_new_hook();
			/////////////////////////////// 遞迴
			// 說明
			// 
			(HOOKP)newnode->LoopBlockHOOKP=parse_loop((HOOKP)newnode->LoopBlockHOOKP,newnode,file);
			read_a_str(str,file); // next str
		}
		// NEWCASEHOOK or not
		if(StrCompare(str,"NEWCASEHOOK") == 0)
		{
			/////////////////////////////////
			// 5-26 未完成
			//
			read_a_str(str,file); // next str
		}
		// filename or not
		if(StrCompare(str,"FILENAME") == 0)
		{
			read_a_str(str,file); // 
			newnode->filename=MemHandleLock(MemHandleNew(StrLen(str)+1));
			StrCopy(newnode->filename,str);
			// 5-31 遞迴 載入 LOAD()
			if(newnode != SelfBlockLLHead)
			{// 跳過第1個 HOOKBLOCK 執行載入
				//  這裡與 custom load 類似 
				// 不同之處是 這裡載入後的串列第1個block 會被捨棄掉
				// 以 newnode 代替
				(HOOKP)newnode->hookP=allocate_a_new_hook();
				(HOOKP)newnode->hookP=LOAD((HOOKP)newnode->hookP,str);
				((HOOKP)newnode->hookP)->BlockLLHeadP->NEXTNODE->PREVNODE=NULL;
				// 重設背景
				Rbnp=((HOOKP)newnode->hookP)->BlockLLHeadP->NEXTNODE;
				while(Rbnp!=NULL)
				{
					Rbnp->SelfBlockLLHead=newnode;
					Rbnp->BackgroundBlockLLHead=newnode;
					Rbnp=Rbnp->NEXTNODE;
				}
				// newnode 在這裡要把 剛剛 LOAD進來的串列頭一個 的IOLLHead搶過來
				// 因為暗線段已經建好了
				newnode->bap->IONodeLLHead=((HOOKP)newnode->hookP)->BlockLLHeadP->bap->IONodeLLHead;
				// 走一遍 wireLL 把 ID==1 的換成 newnode
				newwire=((HOOKP)newnode->hookP)->WireLLHeadP;
				while(newwire!=NULL)
				{
					if(newwire->StartBlockP->ID == 1)
					{
						newwire->StartBlockP=newnode;
					}
					if(newwire->EndBlockP->ID == 1)
					{
						newwire->EndBlockP=newnode;
					}
						newwire=newwire->NEXTNODE;
				}
				((HOOKP)newnode->hookP)->BlockLLHeadP=((HOOKP)newnode->hookP)->BlockLLHeadP->NEXTNODE;
				((HOOKP)newnode->hookP)->WireLLHeadP=((HOOKP)newnode->hookP)->WireLLHeadP;
			}
			
			read_a_str(str,file); // next str
		}
		
		
		// 設定self
		newnode->SelfBlockLLHead=SelfBlockLLHead;
		// 設定background
		newnode->BackgroundBlockLLHead=SelfBlockLLHead;
		
		// 完成一個block 插入串列
		// 5-31 搬到前面 以便遞迴 parse_loop 的時候 wire 會找不到 那個ID 的block
		//hook->BlockLLHeadP=InsertIntoBlockLLNoUID(hook->BlockLLHeadP,newnode);
		
		// ENDBLOCK or ENDNODE
		if(StrCompare(str,"ENDBLOCK") == 0)
		{
			break;
		}
	}// while 
	while(1)
	{
		read_a_str(str,file);
		if(StrCompare(str,"STARTWIRE") == 0)
		{
			break;
		}
	}
	while(1)
	{
		read_a_str(str,file);	// StartBlockP?
		// 處理 STARTWIRE 內部
		// 找出這個ID目前的ptr
		if(StrCompare(str,"STARTBID") == 0)
		{
			newwire=allocate_a_new_wire_node();

			read_a_str(str,file); // 
			//newwire->StartBlockP=FindNodeByID(SYSHOOK->BlockLLHeadP,StrAToI(str));
			// 5-30 要找出哪個node
			// 5-31 改 以從頭 向下 探尋 的方式 因為這時的串列可以串接起來了
			//      也就是說 如果這時候是 第2次 進來這裡 那麼必是迴圈結構
			//      原本無法 從頭找出 這個ID的BLOCK 是因為 前面 將newnode 插入
			//      串列的時機是在 進來這裡之後 因此 在這裡 以探尋法 有時會找不到
			//      這個 ID 的 BLOCK 所以　現在改成 在進來這裡之前 就把 newnode 插入
			//      如此一來由 頭 探尋下來 就可以通通找到
			//      更正一點 如果要探尋 ID 變成 block 那麼只要 從 目前的 selfblockllhead 開始就可以
			//      了 因為 目前的 wire 不可能超過 目前 selfblockllhead 的範圍
			//      wire 所屬範圍 都被限制住了 
			//if((newwire->StartBlockP=FindNodeByID(SYSHOOK->BlockLLHeadP,StrAToI(str)))==NULL)
			//if((newwire->StartBlockP=FindNodeByID(hook->BlockLLHeadP,StrAToI(str)))==NULL)
			//	newwire->StartBlockP=SelfBlockLLHead;
			// 所以改成這樣就好了 
			// 預測兩種情況
			// 1. IO點位在 迴圈結構上 那麼會傳回 SelfBlockLLHead 的指標
			// 2. IO點位在 迴圈結構內 那麼 FindNodeByID 會向內探尋 也就是 newnode->LoopBlockHOOKP 裡面
			//    此時 內部方塊已經佈建完畢
			if(StrCompare(str,"1") == 0)
			{// 如果 BlockID 是 0 則必定存在一個最高 blockLLHead 是 hookblock
				hook_block=SelfBlockLLHead;
				while(hook_block->TYPE != HOOKBLOCK)
				{
					hook_block=hook_block->SelfBlockLLHead;
				}

				newwire->StartBlockP=hook_block;
			}else{
			newwire->StartBlockP=FindNodeByID(SelfBlockLLHead,StrAToI(str));
			}
			read_a_str(str,file); // StartNodeP
			read_a_str(str,file); // 
			newwire->StartNodeP=FindIONodeByID(newwire->StartBlockP->bap->IONodeLLHead,StrAToI(str));
			read_a_str(str,file); // EndBlockP
			read_a_str(str,file); // 
			//newwire->EndBlockP=FindNodeByID(SYSHOOK->BlockLLHeadP,StrAToI(str));
			//if((newwire->EndBlockP=FindNodeByID(SYSHOOK->BlockLLHeadP,StrAToI(str)))==NULL)
			//if((newwire->EndBlockP=FindNodeByID(hook->BlockLLHeadP,StrAToI(str)))==NULL)
			//	newwire->EndBlockP=SelfBlockLLHead;
			if(StrCompare(str,"1") == 0)
			{// 如果 BlockID 是 0 表示該block是 hookblock
			 // 則串列中必定存在一個最高 blockLLHead 是 hookblock
				hook_block=SelfBlockLLHead;
				while(hook_block->TYPE != HOOKBLOCK)
				{
					hook_block=hook_block->SelfBlockLLHead;
				}

				newwire->EndBlockP=hook_block;
			}else{
			newwire->EndBlockP=FindNodeByID(SelfBlockLLHead,StrAToI(str));
			}
			
			read_a_str(str,file); // EndNodeP
			read_a_str(str,file); // 
			newwire->EndNodeP=FindIONodeByID(newwire->EndBlockP->bap->IONodeLLHead,StrAToI(str));
			
			hook->WireLLHeadP=InsertIntoWireLL(hook->WireLLHeadP,newwire);
		}
		//
		// 完成一個wire 插入串列
		//
		if(StrCompare(str,"ENDWIRE") == 0)
		{
			break;
		}
	}

	return hook;
}
/***********************************************************************
 * 2003-4-12
 * 2003-4-13 修改recursionHead變數改以向系統索取空間
 * 2003-5-26 新版
 ***********************************************************************/
HOOKP LOAD(HOOKP sysHead,char *filename )
{
	FileHand 	loadfile;
	
	// 取得檔案 handle
	loadfile = FileOpen (0,filename,appDBType,appCreatorID,fileModeReadOnly,NULL);

	sysHead=parse_loop(sysHead,NULL, loadfile);
	// 將 HOOKBLOCK 中的 hookp 指標建立
	(HOOKP)sysHead->BlockLLHeadP->hookP=allocate_a_new_hook();
	// 將檔名輸入
	sysHead->BlockLLHeadP->filename=MemHandleLock(MemHandleNew(StrLen(filename)+1));
	StrCopy(sysHead->BlockLLHeadP->filename,filename);
	// 設定 自屬 與背景
	sysHead->BlockLLHeadP->SelfBlockLLHead=sysHead->BlockLLHeadP;
	sysHead->BlockLLHeadP->BackgroundBlockLLHead=sysHead->BlockLLHeadP;

	FileClose(loadfile);
		

	return sysHead;
}
void ProcessCUSTOMLoad(char *filename )
{

	LOAD(&CUSTUMHOOK,filename);
	// 設定 hookp 時需注意 需指向 下一個 block 因為本身 已經是別人的 block了
	// 而 hookp 中的串列 需將頭拿掉 以免遇到 回溯機制 而出問題
	CUSTUMHOOK.BlockLLHeadP->NEXTNODE->PREVNODE=NULL;
	((HOOKP)CUSTUMHOOK.BlockLLHeadP->hookP)->BlockLLHeadP=CUSTUMHOOK.BlockLLHeadP->NEXTNODE;
	((HOOKP)CUSTUMHOOK.BlockLLHeadP->hookP)->WireLLHeadP=CUSTUMHOOK.WireLLHeadP;
	
}
/***********************************************************************
*	"Files" event process routine
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
Boolean LOADFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;
	//char str[10];
	//Handle resH;
	//BitmapType *bitmap;
	//RectangleType Rtest;
	//int i;
	FormPtr		frm;
	ListPtr		lst;
	Word 		CurrentRecord;
	char 		*filename;
	
	switch (eventP->eType) 
		{
		/*
		case menuEvent:
			return FrontPanelFormDoCommand(eventP->data.menu.itemID);
		*/
		case frmOpenEvent:
			BuildLOADMenu();
			// 產生檔案列表

			//WinEraseWindow();
			frmP = FrmGetActiveForm();
			FrmDrawForm ( frmP);
			
			handled = true;
			break;
   		case ctlSelectEvent:  // A control button was pressed and released.
			
	   	// If the done button is pressed, go back to the main form.
	   	if (eventP->data.ctlEnter.controlID == LOADDELETEButton)
		{
			frm = FrmGetActiveForm();
			lst = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, LOADFileListList));
			CurrentRecord = LstGetSelection(lst);

			if(ChoicesHandle)
			if(CurrentRecord != noListSelection)
			{
				filename = LstGetSelectionText(lst,CurrentRecord);
				if(FrmCustomAlert(DeleteFileComfirAlert,filename,"","")==0)
				{//顯示確認視窗
					//DmDeleteDatabase(0,DmFindDatabase(0,filename));
					FileDelete(0,filename);
				}
				FrmGotoForm(LOADForm);
			}
			handled = true;
		}
	   	if (eventP->data.ctlEnter.controlID == LOADLOADButton)
	  		 	{
					frm = FrmGetActiveForm();
					lst = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, LOADFileListList));
					CurrentRecord = LstGetSelection(lst);

					if(ChoicesHandle)
					if(CurrentRecord != noListSelection)
					{
						filename = LstGetSelectionText(lst,CurrentRecord);

					if(CUSTOMLOAD==false)
					{
					
				    ReleaseLLMemPtr(SYSHOOK->BlockLLHeadP->NEXTNODE);
				    ReleaseHOOKNode(SYSHOOK);
			    	SYSHOOK->BlockLLHeadP = NULL;
					SYSHOOK->WireLLHeadP = NULL;
					
					SYSHOOK=LOAD(SYSHOOK,filename);
					((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->BlockLLHeadP=SYSHOOK->BlockLLHeadP;
					((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP=SYSHOOK->WireLLHeadP;
					}else{
						ProcessCUSTOMLoad(filename);
						FrmGotoForm(FunctionsForm);
					}
						/*
					 if(WhoLoad)
					 {			// menuLOAD
						
						if(filename != NULL)
						{
	   						SYSHOOK = LOAD(SYSHOOK,filename);
	   					}
	   					else
	   					{
	   						SYSHOOK->BlockLLHeadP = NULL;
	   						SYSHOOK->WireLLHeadP = NULL;
	   					}
						FrmReturnToForm(0);
	   				 }else{		// functionsLOAD
	   					
						if(filename != NULL)
						{
						
	   					SecondHead = LOAD(SecondHead,filename);
	   					
	   					// estabilish connection
	   					SecondHead.BlockLLHead->BlockLLHeadP = SecondHead.BlockLLHead;
	   					SecondHead.BlockLLHead->WireLLHeadP = SecondHead.WireLLHead;
	   					// give the icon ID
	   					SecondHead.BlockLLHead->bap->BITMAPID = CUSTOMBLOCKBitmap;
	   					// set the IO nodes
	   					AutoFindIOnodesSetIntoICONnode(SecondHead.BlockLLHead);
						SecondHead.BlockLLHead->bap->XY.x = 10;
						SecondHead.BlockLLHead->bap->XY.y = 10;
						SecondHead.BlockLLHead->bap->SIZE.x = 32;
						SecondHead.BlockLLHead->bap->SIZE.y = 32;
	   					}
	   					else
	   					{
	   					SecondHead.BlockLLHead = NULL;
	   					SecondHead.WireLLHead = NULL;
	   					}
						FrmGotoForm(FunctionsForm);
						
	   				 }
	   				 	*/
						FrmReturnToForm(0);
	   				} // no selection?
	   				
					handled = true;
					if (ChoicesHandle)
					{
						MemHandleFree(ChoicesHandle);
						ChoicesHandle = 0;
						MemHandleFree(ChoicesPtrsHandle);
					}
				}
	   	if (eventP->data.ctlEnter.controlID == LOADCANCELButton)
	  		 	{
					/*
					if(WhoLoad)
					{
						// menuLOAD
	   					SYSHOOK->BlockLLHeadP = NULL;
	   					SYSHOOK->WireLLHeadP = NULL;
						FrmReturnToForm(0);
	   				}else{
	   					// functionsLOAD
	   					SecondHead.BlockLLHead = NULL;
	   					SecondHead.WireLLHead = NULL;
						FrmGotoForm(FunctionsForm);
	   				}
					*/
					handled = true;
					FrmReturnToForm(0);
					if (ChoicesHandle)
					{
						MemHandleFree(ChoicesHandle);
						ChoicesHandle = 0;
						MemHandleFree(ChoicesPtrsHandle);
					}
				}
			break;
			/*
  		case lstSelectEvent:  // P7. An entry in the list was selected.
			// Get the current selection from the list and set the current record to edit.
			frm = FrmGetActiveForm();
			lst = FrmGetObjectPtr(frm, FrmGetObjectIndex(frm, FilesFileListList));
			CurrentRecord = LstGetSelection(lst);
			
			// Go to the edit form.	
			//FrmGotoForm(MemoPadEditForm);
			LOAD(CurrentRecord);
			FrmGotoForm(BlockDiagramForm);
			handled = true;
			break;
		*/
		case frmCloseEvent:	// P7. The form was told to close.
			// Free the memory blocks allocated for the list.
			if (ChoicesHandle)
				{
				MemHandleFree(ChoicesHandle);
				ChoicesHandle = 0;
				MemHandleFree(ChoicesPtrsHandle);
				}
			
			break;
		case penDownEvent:
			break;
		case penMoveEvent:
			break;
		case penUpEvent:
			break;
		case keyDownEvent:
			break;
		default:
			break;
		
		}
	
	return handled;
}
