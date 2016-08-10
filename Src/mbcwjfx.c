/***********************************************************************
 *
 * Copyright (c) 1994-1999 3Com Corporation or its subsidiaries.
 * All rights reserved.
 *
 * PROJECT:  Pilot
 * FILE:     Starter.c
 * AUTHOR:   Roger Flores: May 20, 1997
 *
 * DECLARER: Starter
 *
 * DESCRIPTION:
 *	  
 *
 **********************************************************************/
#include <Pilot.h>
#include <SysEvtMgr.h>
#include <KeyMgr.h>
//#include <FloatMgr.h>
#include "StarterRsc.h"
#include "ScrDriverNEW.h"
//#include "Window.h"

#include "vpl.h"

/***********************************************************************
 *
 *   Entry Points
 *
 ***********************************************************************/


/***********************************************************************
 *
 *   Internal Structures
 *
 ***********************************************************************/
typedef struct 
	{
	Byte replaceme;
	} StarterPreferenceType;

typedef struct 
	{
	Byte replaceme;
	} StarterAppInfoType;

typedef StarterAppInfoType* StarterAppInfoPtr;


/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
//static Boolean HideSecretRecords;
DmOpenRef   db;
UInt32      numRecords;
VoidHand 		ChoicesHandle = 0, ChoicesPtrsHandle;

extern  Boolean DataLoaded=false;
  Boolean CanvasState=true;
  int STOP=0;	// 0=�L�� 1=�i�� 2=����
  Boolean LAMP=false;
Boolean RUNFOREVER=false;

int prev_x,prev_y;
  int penMoveAverage=_PenMovAver;
 UInt16 BlockPointerState = HANDBitmap; // block : HAND , THREAD
 UInt16 PanelPointerState = HANDBitmap; // panel : HAND , FINGER
//BNP BlockLLHead = NULL;
//
// LLP �Φb InArea �P CheckOnNode ��Ƥ������޼ƶǻ�
// ���ΤF 4-14
// BNP LLP;

//extern BNP FunctionsLLHead = NULL;
//BNP FLLP;

 HOOKP SYSHOOK;	// �{���D��C���Y
 EndPoint WireStart,WireStop;
 struct hand HandPointer;

UInt16 UID=0;// ��Block�Ϊ�

BNP BlockToolsLLHeadP;
BNP PanelToolsLLHeadP;
BNP FunctionCatagoryLLHead;

// for ScrDisplayMode
	DWordPtr MYwidthP;
	DWordPtr MYheightP;
	DWordPtr MYdepthP;
	BooleanPtr MYenableColorP;
//
// 5-15 �ʵe
// ����4�ܼ�
 int RUN=0;	// 0=�R�� 1=���� 2=�Ȱ�
//Boolean Animation=false;
//Boolean RunLock=false;
//Boolean RunWho=false;

Word TicksPerSecend;
/***********************************************************************
 *
 *   Internal Constants
 *
 ***********************************************************************/


/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/

extern Boolean BlockDiagramFormHandleEvent(EventPtr eventP);
extern Boolean BlockToolsFormHandleEvent(EventPtr eventP);
extern Boolean PanelToolsFormHandleEvent(EventPtr eventP);
extern Boolean FrontPanelFormHandleEvent(EventPtr eventP);
extern Boolean FunctionsFormHandleEvent(EventPtr eventP);
extern Boolean SAVEFormHandleEvent(EventPtr eventP);
extern Boolean LOADFormHandleEvent(EventPtr eventP);
extern Boolean AboutFormHandleEvent(EventPtr eventP);
//extern HOOKP NEW(HOOKP );
extern HOOKP InsertHOOKNode(HOOKP );
extern HOOKP allocate_a_new_hook();
extern void set_block_tools_palette(void );
extern void	set_panel_tools_palette();
extern BNP set_function_palette(void );
extern BNP allocate_a_new_node();
extern BAP allocate_a_new_bap();
extern void ReleaseLLMemPtr(BNP llhead);
extern void ReleaseHOOKNode(HOOKP syshook);


////////////////////////////////////////////////////////////////////////
/***********************************************************************
 *
 * FUNCTION:    RomVersionCompatible
 *
 * DESCRIPTION: This routine checks that a ROM version is meet your
 *              minimum requirement.
 *
 * PARAMETERS:  requiredVersion - minimum rom version required
 *                                (see sysFtrNumROMVersion in SystemMgr.h 
 *                                for format)
 *              launchFlags     - flags that indicate if the application 
 *                                UI is initialized.
 *
 * RETURNED:    error code or zero if rom is compatible
 *
 * REVISION HISTORY:
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
static Err RomVersionCompatible(DWord requiredVersion, Word launchFlags)
{
	DWord romVersion;

	// See if we're on in minimum required version of the ROM or later.
	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);
	if (romVersion < requiredVersion)
		{
		if ((launchFlags & (sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp)) ==
			(sysAppLaunchFlagNewGlobals | sysAppLaunchFlagUIApp))
			{
			FrmAlert (RomIncompatibleAlert);
		
			// Pilot 1.0 will continuously relaunch this app unless we switch to 
			// another safe one.
			if (romVersion < sysMakeROMVersion(2,0,0,sysROMStageRelease,0))
				AppLaunchWithCommand(sysFileCDefaultApp, sysAppLaunchCmdNormalLaunch, NULL);
			}
		
		return (sysErrRomIncompatible);
		}

	return (0);
}



/***********************************************************************
 *
 * FUNCTION:    AppHandleEvent
 *
 * DESCRIPTION: This routine loads form resources and set the event
 *              handler for the form loaded.
 *
 * PARAMETERS:  event  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
static Boolean AppHandleEvent( EventPtr eventP)
{
	Word formId;
	FormPtr frmP;


	if (eventP->eType == frmLoadEvent)
		{
		// Load the form resource.
		formId = eventP->data.frmLoad.formID;
		frmP = FrmInitForm(formId);
		FrmSetActiveForm(frmP);

		// Set the event handler for the form.  The handler of the currently
		// active form is called by FrmHandleEvent each time is receives an
		// event.
		switch (formId)
			{
			case BlockDiagramForm:
			//case FrontPanelForm:
				FrmSetEventHandler(frmP, BlockDiagramFormHandleEvent);
				break;
			case BlockToolsForm:
				FrmSetEventHandler(frmP, BlockToolsFormHandleEvent);
				break;
			case PanelToolsForm:
				FrmSetEventHandler(frmP, PanelToolsFormHandleEvent);
				break;
			case FrontPanelForm:
				FrmSetEventHandler(frmP, FrontPanelFormHandleEvent);
				break;
			case FunctionsForm:
				FrmSetEventHandler(frmP, FunctionsFormHandleEvent);
				break;
			case LOADForm:
				FrmSetEventHandler(frmP, LOADFormHandleEvent);
				break;
			case SAVEForm:
				FrmSetEventHandler(frmP, SAVEFormHandleEvent);
				break;
			case AboutForm:
				FrmSetEventHandler(frmP, AboutFormHandleEvent);
				break;
				
			default:
//				ErrFatalDisplay("Invalid Form Load Event");
				break;

			}
		return true;
		}
	
	return false;
}


/***********************************************************************
 *
 * FUNCTION:    AppEventLoop
 *
 * DESCRIPTION: This routine is the event loop for the application.  
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
static void AppEventLoop(void)
{
	Word error;
	EventType event;


	do {
		//EvtGetEvent(&event, evtWaitForever);
		EvtGetEvent(&event, ((RUN)?1:100));
		
		//5-8 ����o�O���̥��s HOOK ���󤤪� hookp->wirellhead �|����
		// �ҥH�b�o�̧�s
		// ���D�F
		// �]��Hook->Wirellhead �@�����S���Q��s
		// �{���̭��u���� hook ���󤤪� hookp->wirellheadp
		// �]���b�o�̧⥦�ɰ_��
		// �n�Ҽ{��SYSHOOK ���@�ܼƼo���F
		SYSHOOK->WireLLHeadP = ((HOOKP)SYSHOOK->BlockLLHeadP->hookP)->WireLLHeadP;
		
		if (! SysHandleEvent(&event))
			if (! MenuHandleEvent(0, &event, &error))
				if (! AppHandleEvent(&event))
					FrmDispatchEvent(&event);

	} while (event.eType != appStopEvent);
}


/***********************************************************************
 *
 * FUNCTION:     AppStart
 *
 * DESCRIPTION:  Get the current application's preferences.
 *
 * PARAMETERS:   nothing
 *
 * RETURNED:     Err value 0 if nothing went wrong
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
static Err AppStart(void)
{
    StarterPreferenceType prefs;
    Word prefsSize;
	//char text[32];
	
	DWord MYdepthp;

	// add by peng
	// set the "functions"
	//
	//FUNCTIONSLLDATA.LLHead = NULL;
	//FUNCTIONSLLDATA = SetFunctionsLL(FUNCTIONSLLDATA);
	//
	// estabilish the ICON node.
	//
	SYSHOOK = allocate_a_new_hook();
	SYSHOOK->BlockLLHeadP = NULL;
	SYSHOOK->WireLLHeadP = NULL;
	SYSHOOK = InsertHOOKNode(SYSHOOK);
	// ���յ{��
	//set_test_data();
	// �إߤu��L�ϥܦ�C
	set_block_tools_palette();
	set_panel_tools_palette();
	//
	//HandPointer.Clone=allocate_a_new_node();
		
	//
	FunctionCatagoryLLHead = set_function_palette();
	////
	// �������
	ScrDisplayMode(scrDisplayModeGet,MYwidthP,MYheightP,MYdepthP,MYenableColorP);
	// �]�w ���}�Ƕ����
	MYdepthp=2;
	ScrDisplayMode(scrDisplayModeSet,MYwidthP,MYheightP,&MYdepthp,MYenableColorP);
	TicksPerSecend=SysTicksPerSecond();
	/*
	StrPrintF(text,"%l",widthP);
	WinDrawChars(text,StrLen(text),0,0);
	StrPrintF(text,"%l",heightP);
	WinDrawChars(text,StrLen(text),0,15);
	StrPrintF(text,"%l",depthP);
	WinDrawChars(text,StrLen(text),0,30);
	StrPrintF(text,"%l",enableColorP);
	WinDrawChars(text,StrLen(text),0,45);
	*/

	//while(1){}
	
	////
	// Read the saved preferences / saved-state information.
	prefsSize = sizeof(StarterPreferenceType);
	if (PrefGetAppPreferences(appFileCreator, appPrefID, &prefs, &prefsSize, true) != 
		noPreferenceFound)
		{
		}
	
   return 0;
}


/***********************************************************************
 *
 * FUNCTION:    AppStop
 *
 * DESCRIPTION: Save the current state of the application.
 *
 * PARAMETERS:  nothing
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
static void AppStop(void)
{
   StarterPreferenceType prefs;
   
   //����
   ReleaseLLMemPtr(BlockToolsLLHeadP);
   ReleaseLLMemPtr(PanelToolsLLHeadP);
   ReleaseLLMemPtr(FunctionCatagoryLLHead);
   ReleaseLLMemPtr(SYSHOOK->BlockLLHeadP->NEXTNODE);
   ReleaseHOOKNode(SYSHOOK);
	MemHandleUnlock(MemPtrRecoverHandle(SYSHOOK));
	MemHandleFree(MemPtrRecoverHandle(SYSHOOK));
   
	// �^�_���
	ScrDisplayMode(scrDisplayModeSet,MYwidthP,MYheightP,MYdepthP,MYenableColorP);

	// Write the saved preferences / saved-state information.  This data 
	// will be backed up during a HotSync.
	PrefSetAppPreferences (appFileCreator, appPrefID, appPrefVersionNum, 
		&prefs, sizeof (prefs), true);
}



/***********************************************************************
 *
 * FUNCTION:    StarterPilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 *
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
static DWord StarterPilotMain(Word cmd, Ptr cmdPBP, Word launchFlags)
{
	Err error;


	error = RomVersionCompatible (ourMinVersion, launchFlags);
	if (error) return (error);


	switch (cmd)
		{
		case sysAppLaunchCmdNormalLaunch:
			error = AppStart();
			if (error) 
				return error;
				
			FrmGotoForm(BlockDiagramForm);
			AppEventLoop();
			AppStop();
			break;

		default:
			break;

		}
	
	return 0;
}


/***********************************************************************
 *
 * FUNCTION:    PilotMain
 *
 * DESCRIPTION: This is the main entry point for the application.
 *
 * PARAMETERS:  cmd - word value specifying the launch code. 
 *              cmdPB - pointer to a structure that is associated with the launch code. 
 *              launchFlags -  word value providing extra information about the launch.
 * RETURNED:    Result of launch
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
/***********************************************************************
 * 2003-4-12
 * 
 ***********************************************************************/
DWord PilotMain( Word cmd, Ptr cmdPBP, Word launchFlags)
{
    return StarterPilotMain(cmd, cmdPBP, launchFlags);
}

