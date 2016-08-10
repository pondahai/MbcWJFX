/*
void set_test_data()
{
	BAP bap;
	PAP pap;
	HOOKP hook;
	BNP newp;
	IONODEP ionode;
	CTRLNODEP ctrlnode;
	char str_true[5]={"true"};
	char str_false[6]={"false"};


	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = INDICATOR8BLOCKBitmap;
	bap->XY.x = 0;
	bap->XY.y = 40;
	bap->SIZE.x = 32;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	pap = allocate_a_new_pap();
	pap->BITMAPID = INDICATOR8Bitmap;
	pap->XY.x = 100;
	pap->XY.y = 50;
	pap->SIZE.x = 32;
	pap->SIZE.y = 16;
	newp->pap = pap;
	
	// 插入IO 節點
	ionode = allocate_a_new_IOnode();
	ionode->ID = 0;
	ionode->TYPE = 1;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=0;
	ionode->TL.y=0;
	ionode->EXT.x=32;
	ionode->EXT.y=16;
	newp->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newp->bap->IONodeLLHead,
				ionode);
	
	newp->TYPE=FUNCBLOCK;
	newp->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
	newp->BackgroundBlockLLHead=SYSHOOK->BlockLLHeadP;
	//insert into linked list
	SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,newp);
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = CTRLU8BLOCKBitmap;
	bap->XY.x = 0;
	bap->XY.y = 40;
	bap->SIZE.x = 32;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	pap = allocate_a_new_pap();
	pap->BITMAPID = CTRLU8Bitmap;
	pap->XY.x = 100;
	pap->XY.y = 50;
	pap->SIZE.x = 32;
	pap->SIZE.y = 16;
	newp->pap = pap;
	
	// 插入IO 節點
	ionode = allocate_a_new_IOnode();
	ionode->ID = 0;
	ionode->TYPE = 0;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=0;
	ionode->TL.y=0;
	ionode->EXT.x=32;
	ionode->EXT.y=16;
	newp->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newp->bap->IONodeLLHead,
				ionode);
	
	newp->TYPE=FUNCBLOCK;
	newp->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
	newp->BackgroundBlockLLHead=SYSHOOK->BlockLLHeadP;
	//insert into linked list
	SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,newp);
	//allocate a new node                                   	
	newp = allocate_a_new_node();

	bap = allocate_a_new_bap();
	bap->BITMAPID = FORLOOPBitmap;
	bap->XY.x = 40;
	bap->XY.y = 100;
	bap->SIZE.x = 32;
	bap->SIZE.y = 32; 
	newp->bap = bap;
	
	ctrlnode = allocate_a_new_CTRLnode();
	ctrlnode->ID=DRAGNODE;
	ctrlnode->TL.x=25;
	ctrlnode->TL.y=25;
	ctrlnode->EXT.x=7;
	ctrlnode->EXT.y=7;
	newp->bap->CTRLNodeLLHead=ctrlnode;
	
	hook = allocate_a_new_hook();
	hook->BlockLLHeadP = NULL;
	hook->WireLLHeadP = NULL;
	(HOOKP)newp->LoopBlockHOOKP = hook;
	
	newp->TYPE=LOOPBLOCK;
	newp->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
	newp->BackgroundBlockLLHead=SYSHOOK->BlockLLHeadP;
	//insert into linked list
	SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,newp);

	//allocate a new node                                   	
	newp = allocate_a_new_node();

	bap = allocate_a_new_bap();
	bap->BITMAPID = SWITCHCASEBitmap;
	bap->XY.x = 40;
	bap->XY.y = 100;
	bap->SIZE.x = 32;
	bap->SIZE.y = 32; 
	newp->bap = bap;
	// 拖拉點
	ctrlnode = allocate_a_new_CTRLnode();
	ctrlnode->ID=DRAGNODE;
	ctrlnode->TL.x=25;
	ctrlnode->TL.y=25;
	ctrlnode->EXT.x=7;
	ctrlnode->EXT.y=7;
	newp->bap->CTRLNodeLLHead=ctrlnode;
	// 上選點 
	ctrlnode = allocate_a_new_CTRLnode();
	ctrlnode->ID=SCPAGEUP;
	ctrlnode->TL.x=25;
	ctrlnode->TL.y=1;
	ctrlnode->EXT.x=6;
	ctrlnode->EXT.y=13;
	newp->bap->CTRLNodeLLHead->NEXT=ctrlnode;
	// 下選點
	ctrlnode = allocate_a_new_CTRLnode();
	ctrlnode->ID=SCPAGEDOWN;
	ctrlnode->TL.x=1;
	ctrlnode->TL.y=1;
	ctrlnode->EXT.x=6;
	ctrlnode->EXT.y=13;
	newp->bap->CTRLNodeLLHead->NEXT->NEXT=ctrlnode;
	// 第一頁
	hook = allocate_a_new_hook();
	hook->name = MemHandleLock(MemHandleNew(StrLen(str_true)+1));
	MemMove(hook->name,str_true,StrLen(str_true)+1);
	
	hook->BlockLLHeadP = NULL;
	hook->WireLLHeadP = NULL;
	(HOOKP)newp->CaseBlockHOOKLLHeadP = hook;
	// 第二頁
	hook = allocate_a_new_hook();
	hook->name = MemHandleLock(MemHandleNew(StrLen(str_false)+1));
	MemMove(hook->name,str_false,StrLen(str_false)+1);
	
	hook->BlockLLHeadP = NULL;
	hook->WireLLHeadP = NULL;
	(HOOKP)((HOOKP)newp->CaseBlockHOOKLLHeadP)->NEXT = hook;
	hook->PREV = ((HOOKP)newp->CaseBlockHOOKLLHeadP);
	// 形成迴圈
	(HOOKP)hook->NEXT = ((HOOKP)newp->CaseBlockHOOKLLHeadP);
	(HOOKP)((HOOKP)newp->CaseBlockHOOKLLHeadP)->PREV = hook;
	// 指向目前頁
	(HOOKP)newp->CurrentHOOKP=(HOOKP)newp->CaseBlockHOOKLLHeadP;
	
	newp->TYPE=CASEBLOCK;
	newp->SelfBlockLLHead=SYSHOOK->BlockLLHeadP;
	newp->BackgroundBlockLLHead=SYSHOOK->BlockLLHeadP;
	//insert into linked list
	SYSHOOK->BlockLLHeadP = InsertIntoBlockLLwithUID(SYSHOOK->BlockLLHeadP,newp);



}
*/

FUNCTIONSLLDATAtype InsertIntoFunctionsLL(FUNCTIONSLLDATAtype Fdata,BNP node )
{
		//BNP LLP = BlockLLHead;
	//FLLP = FunctionsLLHead;
	Fdata.llp = Fdata.LLHead;
	
	if(Fdata.llp == NULL)
	{
		Fdata.LLHead = node ;
		return Fdata;
	}
	
	while(true)
	{
		if(Fdata.llp->NEXTNODE == NULL)
			break;
		Fdata.llp = Fdata.llp->NEXTNODE;
	}
	
	Fdata.llp->NEXTNODE = node;
	
	return Fdata;
}


FUNCTIONSLLDATAtype SetFunctionsLL(FUNCTIONSLLDATAtype  Fdata)
{
	BAP bap;
	PAP pap;
	BNP newp;
	IONODEP ionode;
	
	newp = allocate_a_new_node();

	//establish the new node's data
	bap = allocate_a_new_bap();
	bap->BITMAPID = INDICATOR8BLOCKBitmap;
	bap->XY.x = 0;
	bap->XY.y = 40;
	bap->SIZE.x = 32;
	bap->SIZE.y = 16; 
	newp->bap = bap;
	
	pap = allocate_a_new_pap();
	pap->BITMAPID = INDICATOR8Bitmap;
	pap->XY.x = 100;
	pap->XY.y = 50;
	pap->SIZE.x = 32;
	pap->SIZE.y = 16;
	newp->pap = pap;
	
	// 插入IO 節點
	ionode = allocate_a_new_IOnode();
	ionode->ID = 0;
	ionode->TYPE = 1;
	ionode->P = MemHandleLock(MemHandleNew(sizeof(int)));
	ionode->BYTES=sizeof(int);
	ionode->NUMS=1;
	ionode->TL.x=0;
	ionode->TL.y=0;
	ionode->EXT.x=32;
	ionode->EXT.y=16;
	newp->bap->IONodeLLHead = 
		InsertIntoIONodesLL(
			newp->bap->IONodeLLHead,
				ionode);
	
	newp->TYPE=FUNCBLOCK;
	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

/*	
	newp = allocate_a_new_node(newp);
	//establish the new node's data


	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);
	
	
	newp = allocate_a_new_node(newp);
	//establish the new node's data
	newp->bap->UID = 0x0005;                          
	newp->bap->PanelBitmapID = SWOFFBitmap;          
	newp->bap->BITMAPID = LOGICCTRLBitmap;     
	newp->bap->PanelXY.x = 50;                       
	newp->bap->PanelXY.y = 70;                        
	newp->bap->PanelSize.x = 16;                      
	newp->bap->PanelSize.y = 32;                      
	newp->bap->XY.x = 40;                        
	newp->bap->XY.y = 80;                       
	newp->bap->SIZE.x = 32;                      
	newp->bap->SIZE.y = 16;                      
	newp->bap->IONodeN = 1;                           
	newp->bap->IONodes[0].NodeID = 0x0001;            
	newp->bap->IONodes[0].IO = false;           //output       
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0; 
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0; 
	newp->bap->IONodes[0].PieceExtent.x = 32;         
	newp->bap->IONodes[0].PieceExtent.y = 16;
	newp->bap->IONodes[0].value = 0;
	newp->bap->PanelControlNodeN = 1;
	newp->bap->PanelNodes[0].NodeID = 0x0001;
	newp->bap->PanelNodes[0].ControlNodeAreaTopLeft.x=0;
	newp->bap->PanelNodes[0].ControlNodeAreaTopLeft.y=0;
	newp->bap->PanelNodes[0].ControlNodeAreaExtent.x=16;
	newp->bap->PanelNodes[0].ControlNodeAreaExtent.y=32;
	
    	newp->NEXTNODE = NULL;


	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	newp = allocate_a_new_node(newp);
	//establish the new node's data
	newp->bap->UID = 0x0005;                          
	newp->bap->PanelBitmapID = LEDOFFBitmap;          
	newp->bap->BITMAPID = LOGICINDICATORBitmap;     
	newp->bap->PanelXY.x = 50;                       
	newp->bap->PanelXY.y = 90;                        
	newp->bap->PanelSize.x = 16;                      
	newp->bap->PanelSize.y = 16;                      
	newp->bap->XY.x = 0;                        
	newp->bap->XY.y = 80;                       
	newp->bap->SIZE.x = 32;                      
	newp->bap->SIZE.y = 16;                      
	newp->bap->IONodeN = 1;                           
	newp->bap->IONodes[0].NodeID = 0x0001;            
	newp->bap->IONodes[0].IO = true;           //input       
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0; 
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0; 
	newp->bap->IONodes[0].PieceExtent.x = 32;         
	newp->bap->IONodes[0].PieceExtent.y = 16;
	newp->bap->IONodes[0].value = 0;
	newp->bap->PanelControlNodeN = 0;
	
    	newp->NEXTNODE = NULL;


	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node
	newp = allocate_a_new_node();
	//establish the new node's data
	newp->bap->UID = 0x0001;                                   
	newp->bap->PanelBitmapID = NULL;                           
	newp->bap->BITMAPID = ANDBitmap;                      
	newp->bap->PanelXY.x = NULL;                               
	newp->bap->PanelXY.y = NULL;                               
	newp->bap->PanelSize.x = NULL;                             
	newp->bap->PanelSize.y = NULL;                             
	newp->bap->XY.x = 0;                                
	newp->bap->XY.y = 60;                                 
	newp->bap->SIZE.x = 16;                               
	newp->bap->SIZE.y = 16;                               
	newp->bap->IONodeN = 3;                                    
	newp->bap->IONodes[0].NodeID = 0x0001;                     
	newp->bap->IONodes[0].IO = true;                           
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0;          
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0;          
	newp->bap->IONodes[0].PieceExtent.x = 8;                   
	newp->bap->IONodes[0].PieceExtent.y = 8;                   
    	newp->bap->IONodes[1].NodeID = 0x0002;                     
        newp->bap->IONodes[1].IO = true;                           
        newp->bap->IONodes[1].PieceRelativeTopLeft.x = 0;          
        newp->bap->IONodes[1].PieceRelativeTopLeft.y = 8;          
        newp->bap->IONodes[1].PieceExtent.x = 8;                   
        newp->bap->IONodes[1].PieceExtent.y = 8;                   
        newp->bap->IONodes[2].NodeID = 0x0003;                     
        newp->bap->IONodes[2].IO = false;                          
        newp->bap->IONodes[2].PieceRelativeTopLeft.x = 8;          
        newp->bap->IONodes[2].PieceRelativeTopLeft.y = 0;          
        newp->bap->IONodes[2].PieceExtent.x = 8;                   
        newp->bap->IONodes[2].PieceExtent.y = 16;                  
    	newp->NEXTNODE = NULL;

	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node
	newp = allocate_a_new_node();
	//establish the new node's data
	newp->bap->UID = 0x0001;                                   
	newp->bap->PanelBitmapID = NULL;                           
	newp->bap->BITMAPID = ORBitmap;                      
	newp->bap->PanelXY.x = NULL;                               
	newp->bap->PanelXY.y = NULL;                               
	newp->bap->PanelSize.x = NULL;                             
	newp->bap->PanelSize.y = NULL;                             
	newp->bap->XY.x = 20;                                
	newp->bap->XY.y = 60;                                 
	newp->bap->SIZE.x = 16;                               
	newp->bap->SIZE.y = 16;                               
	newp->bap->IONodeN = 3;                                    
	newp->bap->IONodes[0].NodeID = 0x0001;                     
	newp->bap->IONodes[0].IO = true;                           
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0;          
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0;          
	newp->bap->IONodes[0].PieceExtent.x = 8;                   
	newp->bap->IONodes[0].PieceExtent.y = 8;                   
    	newp->bap->IONodes[1].NodeID = 0x0002;                     
        newp->bap->IONodes[1].IO = true;                           
        newp->bap->IONodes[1].PieceRelativeTopLeft.x = 0;          
        newp->bap->IONodes[1].PieceRelativeTopLeft.y = 8;          
        newp->bap->IONodes[1].PieceExtent.x = 8;                   
        newp->bap->IONodes[1].PieceExtent.y = 8;                   
        newp->bap->IONodes[2].NodeID = 0x0003;                     
        newp->bap->IONodes[2].IO = false;                          
        newp->bap->IONodes[2].PieceRelativeTopLeft.x = 8;          
        newp->bap->IONodes[2].PieceRelativeTopLeft.y = 0;          
        newp->bap->IONodes[2].PieceExtent.x = 8;                   
        newp->bap->IONodes[2].PieceExtent.y = 16;                  
    	newp->NEXTNODE = NULL;

	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node
	newp = allocate_a_new_node();
	//establish the new node's data
	newp->bap->UID = 0x0001;                                   
	newp->bap->PanelBitmapID = NULL;                           
	newp->bap->BITMAPID = NOTBitmap;                      
	newp->bap->PanelXY.x = NULL;                               
	newp->bap->PanelXY.y = NULL;                               
	newp->bap->PanelSize.x = NULL;                             
	newp->bap->PanelSize.y = NULL;                             
	newp->bap->XY.x = 40;                                
	newp->bap->XY.y = 60;                                 
	newp->bap->SIZE.x = 16;                               
	newp->bap->SIZE.y = 16;                               
	newp->bap->IONodeN = 2;                                    
	newp->bap->IONodes[0].NodeID = 0x0001;                     
	newp->bap->IONodes[0].IO = true;                           
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0;          
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0;          
	newp->bap->IONodes[0].PieceExtent.x = 8;                   
	newp->bap->IONodes[0].PieceExtent.y = 16;                   
    	newp->bap->IONodes[1].NodeID = 0x0002;                     
        newp->bap->IONodes[1].IO = false;                           
        newp->bap->IONodes[1].PieceRelativeTopLeft.x = 8;          
        newp->bap->IONodes[1].PieceRelativeTopLeft.y = 0;          
        newp->bap->IONodes[1].PieceExtent.x = 8;                   
        newp->bap->IONodes[1].PieceExtent.y = 16;                   
    	newp->NEXTNODE = NULL;

	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);


	//allocate a new node
	newp = allocate_a_new_node();
	//establish the new node's data
	newp->bap->UID = 0x0001;                                   
	newp->bap->PanelBitmapID = NULL;                           
	newp->bap->BITMAPID = ADDBitmap;                      
	newp->bap->PanelXY.x = NULL;                               
	newp->bap->PanelXY.y = NULL;                               
	newp->bap->PanelSize.x = NULL;                             
	newp->bap->PanelSize.y = NULL;                             
	newp->bap->XY.x = 0;                                
	newp->bap->XY.y = 20;                                 
	newp->bap->SIZE.x = 16;                               
	newp->bap->SIZE.y = 16;                               
	newp->bap->IONodeN = 3;                                    
	newp->bap->IONodes[0].NodeID = 0x0001;                     
	newp->bap->IONodes[0].IO = true;                           
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0;          
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0;          
	newp->bap->IONodes[0].PieceExtent.x = 8;                   
	newp->bap->IONodes[0].PieceExtent.y = 8;                   
    	newp->bap->IONodes[1].NodeID = 0x0002;                     
        newp->bap->IONodes[1].IO = true;                           
        newp->bap->IONodes[1].PieceRelativeTopLeft.x = 0;          
        newp->bap->IONodes[1].PieceRelativeTopLeft.y = 8;          
        newp->bap->IONodes[1].PieceExtent.x = 8;                   
        newp->bap->IONodes[1].PieceExtent.y = 8;                   
        newp->bap->IONodes[2].NodeID = 0x0003;                     
        newp->bap->IONodes[2].IO = false;                          
        newp->bap->IONodes[2].PieceRelativeTopLeft.x = 8;          
        newp->bap->IONodes[2].PieceRelativeTopLeft.y = 0;          
        newp->bap->IONodes[2].PieceExtent.x = 8;                   
        newp->bap->IONodes[2].PieceExtent.y = 16;                  
    	newp->NEXTNODE = NULL;

	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node
	newp = allocate_a_new_node();
	//establish the new node's data
	newp->bap->UID = 0x0002;                                      
	newp->bap->PanelBitmapID = NULL;                              
	newp->bap->BITMAPID = SUBBitmap;                         
	newp->bap->PanelXY.x = NULL;                                  
	newp->bap->PanelXY.y = NULL;                                  
	newp->bap->PanelSize.x = NULL;                                
	newp->bap->PanelSize.y = NULL;                                
	newp->bap->XY.x = 20;                                    
	newp->bap->XY.y = 20;                                    
	newp->bap->SIZE.x = 16;                                  
	newp->bap->SIZE.y = 16;                                  
	newp->bap->IONodeN = 3;                                       
	newp->bap->IONodes[0].NodeID = 0x0001;                        
	newp->bap->IONodes[0].IO = true;                              
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0;             
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0;             
	newp->bap->IONodes[0].PieceExtent.x = 8;                      
	newp->bap->IONodes[0].PieceExtent.y = 8;                      
    	newp->bap->IONodes[1].NodeID = 0x0002;                        
        newp->bap->IONodes[1].IO = true;                              
        newp->bap->IONodes[1].PieceRelativeTopLeft.x = 0;             
        newp->bap->IONodes[1].PieceRelativeTopLeft.y = 8;             
        newp->bap->IONodes[1].PieceExtent.x = 8;                      
        newp->bap->IONodes[1].PieceExtent.y = 8;                      
        newp->bap->IONodes[2].NodeID = 0x0003;                        
        newp->bap->IONodes[2].IO = false;                             
        newp->bap->IONodes[2].PieceRelativeTopLeft.x = 8;             
        newp->bap->IONodes[2].PieceRelativeTopLeft.y = 0;             
        newp->bap->IONodes[2].PieceExtent.x = 8;                      
        newp->bap->IONodes[2].PieceExtent.y = 16;                     
    	newp->NEXTNODE = NULL;

	//insert into linked list
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node                                   	
	newp = allocate_a_new_node();                     
	//establish the new node's data                         
	newp->bap->UID = 0x0003;                                  
	newp->bap->PanelBitmapID = NULL;                          
	newp->bap->BITMAPID = MULBitmap;                     
	newp->bap->PanelXY.x = NULL;                              
	newp->bap->PanelXY.y = NULL;                              
	newp->bap->PanelSize.x = NULL;                            
	newp->bap->PanelSize.y = NULL;                            
	newp->bap->XY.x = 40;                                
	newp->bap->XY.y = 20;                                
	newp->bap->SIZE.x = 16;                              
	newp->bap->SIZE.y = 16;                              
	newp->bap->IONodeN = 3;                                   
	newp->bap->IONodes[0].NodeID = 0x0001;                    
	newp->bap->IONodes[0].IO = true;                          
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0;         
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0;         
	newp->bap->IONodes[0].PieceExtent.x = 8;                  
	newp->bap->IONodes[0].PieceExtent.y = 8;                  
    	newp->bap->IONodes[1].NodeID = 0x0002;                    
        newp->bap->IONodes[1].IO = true;                          
        newp->bap->IONodes[1].PieceRelativeTopLeft.x = 0;         
        newp->bap->IONodes[1].PieceRelativeTopLeft.y = 8;         
        newp->bap->IONodes[1].PieceExtent.x = 8;                  
        newp->bap->IONodes[1].PieceExtent.y = 8;                  
        newp->bap->IONodes[2].NodeID = 0x0003;                    
        newp->bap->IONodes[2].IO = false;                         
        newp->bap->IONodes[2].PieceRelativeTopLeft.x = 8;         
        newp->bap->IONodes[2].PieceRelativeTopLeft.y = 0;         
        newp->bap->IONodes[2].PieceExtent.x = 8;                  
        newp->bap->IONodes[2].PieceExtent.y = 16;                 
    	newp->NEXTNODE = NULL;
                                                                
	//insert into linked list                               
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node                                   	
	newp = allocate_a_new_node();                     
	//establish the new node's data                         
	newp->bap->UID = 0x0003;                                  
	newp->bap->PanelBitmapID = NULL;                          
	newp->bap->BITMAPID = DIVBitmap;                     
	newp->bap->PanelXY.x = NULL;                              
	newp->bap->PanelXY.y = NULL;                              
	newp->bap->PanelSize.x = NULL;                            
	newp->bap->PanelSize.y = NULL;                            
	newp->bap->XY.x = 60;                                
	newp->bap->XY.y = 20;                                
	newp->bap->SIZE.x = 16;                              
	newp->bap->SIZE.y = 16;                              
	newp->bap->IONodeN = 3;                                   
	newp->bap->IONodes[0].NodeID = 0x0001;                    
	newp->bap->IONodes[0].IO = true;                          
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 0;         
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 0;         
	newp->bap->IONodes[0].PieceExtent.x = 8;                  
	newp->bap->IONodes[0].PieceExtent.y = 8;                  
    	newp->bap->IONodes[1].NodeID = 0x0002;                    
        newp->bap->IONodes[1].IO = true;                          
        newp->bap->IONodes[1].PieceRelativeTopLeft.x = 0;         
        newp->bap->IONodes[1].PieceRelativeTopLeft.y = 8;         
        newp->bap->IONodes[1].PieceExtent.x = 8;                  
        newp->bap->IONodes[1].PieceExtent.y = 8;                  
        newp->bap->IONodes[2].NodeID = 0x0003;                    
        newp->bap->IONodes[2].IO = false;                         
        newp->bap->IONodes[2].PieceRelativeTopLeft.x = 8;         
        newp->bap->IONodes[2].PieceRelativeTopLeft.y = 0;         
        newp->bap->IONodes[2].PieceExtent.x = 8;                  
        newp->bap->IONodes[2].PieceExtent.y = 16;                 
    	newp->NEXTNODE = NULL;
                                                                
	//insert into linked list                               
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node                                   	
	newp = allocate_a_new_node();                     
	//establish the new node's data                         
	newp->bap->UID = NULL;                                  
	newp->bap->PanelBitmapID = NULL;                          
	newp->bap->BITMAPID = CUSTOMBLOCKBitmap;                     
	newp->bap->PanelXY.x = NULL;                              
	newp->bap->PanelXY.y = NULL;                              
	newp->bap->PanelSize.x = NULL;                            
	newp->bap->PanelSize.y = NULL;                            
	newp->bap->XY.x = 0;                                
	newp->bap->XY.y = 100;                                
	newp->bap->SIZE.x = 32;                              
	newp->bap->SIZE.y = 32;                              
    	newp->NEXTNODE = NULL;
                                                                
	//insert into linked list                               
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node                                   	
	newp = allocate_a_new_node();                     
	//establish the new node's data                         
	newp->bap->UID = NULL;                                  
	newp->bap->PanelBitmapID = NULL;                          
	newp->bap->BITMAPID = FORLOOPBitmap;                     
	newp->bap->PanelXY.x = NULL;                              
	newp->bap->PanelXY.y = NULL;                              
	newp->bap->PanelSize.x = NULL;                            
	newp->bap->PanelSize.y = NULL;                            
	newp->bap->XY.x = 40;                                
	newp->bap->XY.y = 100;                                
	newp->bap->SIZE.x = 32;                              
	newp->bap->SIZE.y = 32;                              
	newp->bap->IONodeN = 1;
	newp->bap->IONodes[0].NodeID = 0x0001;
	newp->bap->IONodes[0].IO = 2; // mean the drag point
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 25;
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 25;
	newp->bap->IONodes[0].PieceExtent.x = 7;
	newp->bap->IONodes[0].PieceExtent.y = 7;
    	newp->NEXTNODE = NULL;
                                                                
	//insert into linked list                               
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node                                   	
	newp = allocate_a_new_node();                     
	//establish the new node's data                         
	newp->bap->UID = NULL;                                  
	newp->bap->PanelBitmapID = NULL;                          
	newp->bap->BITMAPID = WHILELOOPBitmap;                     
	newp->bap->PanelXY.x = NULL;                              
	newp->bap->PanelXY.y = NULL;                              
	newp->bap->PanelSize.x = NULL;                            
	newp->bap->PanelSize.y = NULL;                            
	newp->bap->XY.x = 80;                                
	newp->bap->XY.y = 100;                                
	newp->bap->SIZE.x = 32;                              
	newp->bap->SIZE.y = 32;
	newp->bap->IONodeN = 1;
	newp->bap->IONodes[0].NodeID = 0x0001;
	newp->bap->IONodes[0].IO = 2; // mean the drag point
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 25;
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 25;
	newp->bap->IONodes[0].PieceExtent.x = 7;
	newp->bap->IONodes[0].PieceExtent.y = 7;

    	newp->NEXTNODE = NULL;
                                                                
	//insert into linked list                               
	Fdata = InsertIntoFunctionsLL(Fdata,newp);

	//allocate a new node                                   	
	newp = allocate_a_new_node();                     
	//establish the new node's data                         
	newp->bap->UID = NULL;                                  
	newp->bap->PanelBitmapID = NULL;                          
	newp->bap->BITMAPID = SWITCHCASEBitmap;                     
	newp->bap->PanelXY.x = NULL;                              
	newp->bap->PanelXY.y = NULL;                              
	newp->bap->PanelSize.x = NULL;                            
	newp->bap->PanelSize.y = NULL;                            
	newp->bap->XY.x = 120;                                
	newp->bap->XY.y = 100;                                
	newp->bap->SIZE.x = 32;                              
	newp->bap->SIZE.y = 32;                              
	newp->bap->IONodeN = 1;
	newp->bap->IONodes[0].NodeID = 0x0001;
	newp->bap->IONodes[0].IO = 2; // mean the drag point
	newp->bap->IONodes[0].PieceRelativeTopLeft.x = 25;
	newp->bap->IONodes[0].PieceRelativeTopLeft.y = 25;
	newp->bap->IONodes[0].PieceExtent.x = 7;
	newp->bap->IONodes[0].PieceExtent.y = 7;
    	newp->NEXTNODE = NULL;
                                                                
	//insert into linked list                               
	Fdata = InsertIntoFunctionsLL(Fdata,newp);
*/
	return Fdata;
}

void SetTestData(void )
{
	//UInt16 i;
	//struct list *ptr;
	BlockLink newnodeptr;
	//WireLink  newwireptr;
	
	WireStart.BlockP = NULL;
	WireStop.BlockP = NULL;


	//allocate a new node
	newnodeptr = allocate_a_new_node();
	//establish the new node's data
	newnodeptr->item.UID = 0x0004;                         
	newnodeptr->item.PanelBitmapID = INDICATOR8Bitmap;     
	newnodeptr->item.BlockBitmapID = INDICATOR8BLOCKBitmap;
	newnodeptr->item.PanelXY.x = 100;                      
	newnodeptr->item.PanelXY.y = 50;                       
	newnodeptr->item.PanelSize.x = 32;                     
	newnodeptr->item.PanelSize.y = 16;                     
	newnodeptr->item.BlockXY.x = 40;                       
	newnodeptr->item.BlockXY.y = 100;                      
	newnodeptr->item.BlockSize.x = 32;                     
	newnodeptr->item.BlockSize.y = 16;                     
	newnodeptr->item.IONodeN = 1;                          
	newnodeptr->item.IONodes[0].NodeID = 0x0001;           
	newnodeptr->item.IONodes[0].IO = false;                
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.x = 0;
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.y = 0;
	newnodeptr->item.IONodes[0].PieceExtent.x = 32;        
	newnodeptr->item.IONodes[0].PieceExtent.y = 16;        
	newnodeptr->item.PanelControlNodeN = 0;
    	newnodeptr->NEXTNODE = NULL;

	//insert into linked list
	InsertIntoBlockLL(newnodeptr);
	
	newnodeptr = allocate_a_new_node(newnodeptr);
	//establish the new node's data
	newnodeptr->item.UID = 0x0005;                          
	newnodeptr->item.PanelBitmapID = CTRLU8Bitmap;          
	newnodeptr->item.BlockBitmapID = CTRLU8BLOCKBitmap;     
	newnodeptr->item.PanelXY.x = 100;                       
	newnodeptr->item.PanelXY.y = 70;                        
	newnodeptr->item.PanelSize.x = 32;                      
	newnodeptr->item.PanelSize.y = 16;                      
	newnodeptr->item.BlockXY.x = 40;                        
	newnodeptr->item.BlockXY.y = 120;                       
	newnodeptr->item.BlockSize.x = 32;                      
	newnodeptr->item.BlockSize.y = 16;                      
	newnodeptr->item.IONodeN = 1;                           
	newnodeptr->item.IONodes[0].NodeID = 0x0001;            
	newnodeptr->item.IONodes[0].IO = true;                  
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.x = 0; 
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.y = 0; 
	newnodeptr->item.IONodes[0].PieceExtent.x = 32;         
	newnodeptr->item.IONodes[0].PieceExtent.y = 16;
	newnodeptr->item.PanelControlNodeN = 2;
	newnodeptr->item.PanelNodes[0].NodeID = 0x0001;
	newnodeptr->item.PanelNodes[0].ControlNodeAreaTopLeft.x=27;
	newnodeptr->item.PanelNodes[0].ControlNodeAreaTopLeft.y=0;
	newnodeptr->item.PanelNodes[0].ControlNodeAreaExtent.x=5;
	newnodeptr->item.PanelNodes[0].ControlNodeAreaExtent.y=8;
	newnodeptr->item.PanelNodes[1].NodeID = 0x0002;
	newnodeptr->item.PanelNodes[1].ControlNodeAreaTopLeft.x=27;
	newnodeptr->item.PanelNodes[1].ControlNodeAreaTopLeft.y=8;
	newnodeptr->item.PanelNodes[1].ControlNodeAreaExtent.x=5;
	newnodeptr->item.PanelNodes[1].ControlNodeAreaExtent.y=8;
	
    	newnodeptr->NEXTNODE = NULL;


	//insert into linked list
	InsertIntoBlockLL(newnodeptr);
	

	//allocate a new node
	newnodeptr = allocate_a_new_node();
	//establish the new node's data
	newnodeptr->item.UID = 0x0001;                                   
	newnodeptr->item.PanelBitmapID = NULL;                           
	newnodeptr->item.BlockBitmapID = ADDBitmap;                      
	newnodeptr->item.PanelXY.x = NULL;                               
	newnodeptr->item.PanelXY.y = NULL;                               
	newnodeptr->item.PanelSize.x = NULL;                             
	newnodeptr->item.PanelSize.y = NULL;                             
	newnodeptr->item.BlockXY.x = 100;                                
	newnodeptr->item.BlockXY.y = 50;                                 
	newnodeptr->item.BlockSize.x = 16;                               
	newnodeptr->item.BlockSize.y = 16;                               
	newnodeptr->item.IONodeN = 3;                                    
	newnodeptr->item.IONodes[0].NodeID = 0x0001;                     
	newnodeptr->item.IONodes[0].IO = true;                           
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.x = 0;          
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.y = 0;          
	newnodeptr->item.IONodes[0].PieceExtent.x = 8;                   
	newnodeptr->item.IONodes[0].PieceExtent.y = 8;                   
    	newnodeptr->item.IONodes[1].NodeID = 0x0002;                     
        newnodeptr->item.IONodes[1].IO = true;                           
        newnodeptr->item.IONodes[1].PieceRelativeTopLeft.x = 0;          
        newnodeptr->item.IONodes[1].PieceRelativeTopLeft.y = 8;          
        newnodeptr->item.IONodes[1].PieceExtent.x = 8;                   
        newnodeptr->item.IONodes[1].PieceExtent.y = 8;                   
        newnodeptr->item.IONodes[2].NodeID = 0x0003;                     
        newnodeptr->item.IONodes[2].IO = false;                          
        newnodeptr->item.IONodes[2].PieceRelativeTopLeft.x = 8;          
        newnodeptr->item.IONodes[2].PieceRelativeTopLeft.y = 0;          
        newnodeptr->item.IONodes[2].PieceExtent.x = 8;                   
        newnodeptr->item.IONodes[2].PieceExtent.y = 16;                  
    	newnodeptr->NEXTNODE = NULL;

	//insert into linked list
	InsertIntoBlockLL(newnodeptr);

	//allocate a new node
	newnodeptr = allocate_a_new_node();
	//establish the new node's data
	newnodeptr->item.UID = 0x0002;                                      
	newnodeptr->item.PanelBitmapID = NULL;                              
	newnodeptr->item.BlockBitmapID = SUBBitmap;                         
	newnodeptr->item.PanelXY.x = NULL;                                  
	newnodeptr->item.PanelXY.y = NULL;                                  
	newnodeptr->item.PanelSize.x = NULL;                                
	newnodeptr->item.PanelSize.y = NULL;                                
	newnodeptr->item.BlockXY.x = 20;                                    
	newnodeptr->item.BlockXY.y = 20;                                    
	newnodeptr->item.BlockSize.x = 16;                                  
	newnodeptr->item.BlockSize.y = 16;                                  
	newnodeptr->item.IONodeN = 3;                                       
	newnodeptr->item.IONodes[0].NodeID = 0x0001;                        
	newnodeptr->item.IONodes[0].IO = true;                              
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.x = 0;             
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.y = 0;             
	newnodeptr->item.IONodes[0].PieceExtent.x = 8;                      
	newnodeptr->item.IONodes[0].PieceExtent.y = 8;                      
    	newnodeptr->item.IONodes[1].NodeID = 0x0002;                        
        newnodeptr->item.IONodes[1].IO = true;                              
        newnodeptr->item.IONodes[1].PieceRelativeTopLeft.x = 0;             
        newnodeptr->item.IONodes[1].PieceRelativeTopLeft.y = 8;             
        newnodeptr->item.IONodes[1].PieceExtent.x = 8;                      
        newnodeptr->item.IONodes[1].PieceExtent.y = 8;                      
        newnodeptr->item.IONodes[2].NodeID = 0x0003;                        
        newnodeptr->item.IONodes[2].IO = false;                             
        newnodeptr->item.IONodes[2].PieceRelativeTopLeft.x = 8;             
        newnodeptr->item.IONodes[2].PieceRelativeTopLeft.y = 0;             
        newnodeptr->item.IONodes[2].PieceExtent.x = 8;                      
        newnodeptr->item.IONodes[2].PieceExtent.y = 16;                     
    	newnodeptr->NEXTNODE = NULL;

	//insert into linked list
	InsertIntoBlockLL(newnodeptr);


	//allocate a new node                                   	
	newnodeptr = allocate_a_new_node();                     
	//establish the new node's data                         
	newnodeptr->item.UID = 0x0003;                                  
	newnodeptr->item.PanelBitmapID = NULL;                          
	newnodeptr->item.BlockBitmapID = DIVBitmap;                     
	newnodeptr->item.PanelXY.x = NULL;                              
	newnodeptr->item.PanelXY.y = NULL;                              
	newnodeptr->item.PanelSize.x = NULL;                            
	newnodeptr->item.PanelSize.y = NULL;                            
	newnodeptr->item.BlockXY.x = 30;                                
	newnodeptr->item.BlockXY.y = 80;                                
	newnodeptr->item.BlockSize.x = 16;                              
	newnodeptr->item.BlockSize.y = 16;                              
	newnodeptr->item.IONodeN = 3;                                   
	newnodeptr->item.IONodes[0].NodeID = 0x0001;                    
	newnodeptr->item.IONodes[0].IO = true;                          
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.x = 0;         
	newnodeptr->item.IONodes[0].PieceRelativeTopLeft.y = 0;         
	newnodeptr->item.IONodes[0].PieceExtent.x = 8;                  
	newnodeptr->item.IONodes[0].PieceExtent.y = 8;                  
    	newnodeptr->item.IONodes[1].NodeID = 0x0002;                    
        newnodeptr->item.IONodes[1].IO = true;                          
        newnodeptr->item.IONodes[1].PieceRelativeTopLeft.x = 0;         
        newnodeptr->item.IONodes[1].PieceRelativeTopLeft.y = 8;         
        newnodeptr->item.IONodes[1].PieceExtent.x = 8;                  
        newnodeptr->item.IONodes[1].PieceExtent.y = 8;                  
        newnodeptr->item.IONodes[2].NodeID = 0x0003;                    
        newnodeptr->item.IONodes[2].IO = false;                         
        newnodeptr->item.IONodes[2].PieceRelativeTopLeft.x = 8;         
        newnodeptr->item.IONodes[2].PieceRelativeTopLeft.y = 0;         
        newnodeptr->item.IONodes[2].PieceExtent.x = 8;                  
        newnodeptr->item.IONodes[2].PieceExtent.y = 16;                 
    	newnodeptr->NEXTNODE = NULL;
                                                                
	//insert into linked list                               
	InsertIntoBlockLL(newnodeptr);                          


	newwireptr = allocate_a_new_wire_node();
	newwireptr->wire.WireID = 0x0001;
	newwireptr->wire.StartBlockID = 1;
	newwireptr->wire.StartNodeID = 2;
	newwireptr->wire.EndBlockID = 3;
	newwireptr->wire.EndNodeID = 0;
	newwireptr->NEXTNODE = NULL;
	InsertIntoWireLL(newwireptr);
	newwireptr = allocate_a_new_wire_node();
	newwireptr->wire.WireID = 0x0002;
	newwireptr->wire.StartBlockID = 2;
	newwireptr->wire.StartNodeID = 2;
	newwireptr->wire.EndBlockID = 3;
	newwireptr->wire.EndNodeID = 1;
	newwireptr->NEXTNODE = NULL;
	InsertIntoWireLL(newwireptr);

}
