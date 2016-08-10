#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"

#include "vpl.h"
// extern function
extern InAreaInfType IsInArea(InAreaInfType ,BNP ,UInt16 now_x,UInt16 now_y,UInt16,UInt16);
// extern variable
extern BNP BlockToolsLLHeadP;
extern UInt16 BlockPointerState;// = HANDBitmap; // block : HAND , THREAD


Boolean BlockToolsFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;
	BNP bnp;
	UInt16 x,y;
	InAreaInfType InAreaInf;
	
	switch (eventP->eType) 
		{
		case frmOpenEvent:
			//WinEraseWindow();
			frmP = FrmGetActiveForm();
			FrmDrawForm ( frmP);
			
			// 畫出工具們
			bnp = BlockToolsLLHeadP;
			x=1;y=13;
			while(bnp != NULL)
			{
				DrawIcon(bnp->bap->BITMAPID,x,y);
				
				bnp->bap->XY.x = x;
				bnp->bap->XY.y = y;
				
				// 計算下一個位置
				if(x == 1)
					x = 19;
				else if(x == 19)
				{
					x = 1;
					y += 19;
				}
				
				bnp = bnp->NEXTNODE;
			}
			
			handled = true;
			break;
		case penDownEvent:
			InAreaInf = IsInArea(InAreaInf,BlockToolsLLHeadP,eventP->screenX,eventP->screenY,0,0); //LLP will set
			if(InAreaInf.InArea)
			{
				BlockPointerState = InAreaInf.llp->bap->BITMAPID;
			}
			FrmReturnToForm(0);
			handled = true;
			break;
		case penMoveEvent:
			
			handled = true;
			break;
		case penUpEvent:
			
			handled = true;
			break;
		case keyDownEvent:
			
		break;
		default:
			break;
		
		}
	
	return handled;
}