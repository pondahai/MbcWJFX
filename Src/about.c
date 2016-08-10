#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"

#include "vpl.h"



 Boolean AboutFormHandleEvent(EventPtr eventP)
{
    Boolean handled = false;
    FormPtr frmP;
	char str[80];
	//Handle resH;
	//BitmapType *bitmap;
	//RectangleType Rtest;
	//int i;
	
	switch (eventP->eType) 
		{

		case frmOpenEvent:
			frmP = FrmGetActiveForm();
			FrmDrawForm ( frmP);
			
			StrPrintF(str,"Last compiled:%s %s",__DATE__,__TIME__);
			WinDrawChars(str,StrLen(str),0,150);
			
			handled = true;
			break;
		case penDownEvent:
			FrmReturnToForm(0);
			handled = true;
			break;
		case penMoveEvent:
				
			
			handled = true;
			break;
		case penUpEvent:
		
			
			handled = true;
			break;
		case nilEvent:
		
			handled = true;
		break;

		default:
			break;
		
		}
	
	return handled;
}
