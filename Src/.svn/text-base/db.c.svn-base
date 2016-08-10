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
extern DmOpenRef   db;
extern UInt32      numRecords;
//extern SysHead SYSHOOK;	// 程式主串列的頭


/***********************************************************************
 *
 *   Internal Functions
 *
 ***********************************************************************/

/***********************************************************************
 * Database rotuine
 ***********************************************************************/
 void    OpenTestDB(void )
{
    UInt16      error;
	LocalID		dbID;
	
    error=DmCreateDatabase(0,"test0520",appCreatorID,appDBType,false);

	if(error==dmErrAlreadyExists)
	{//已存在，殺掉
		dbID=DmFindDatabase(0,"test0520");
		DmDeleteDatabase(0,dbID);
	    error=DmCreateDatabase(0,"test0520",appCreatorID,appDBType,false);
	}
    if((error!=dmErrAlreadyExists)&&(error!=0)) {
        /* Error Occurs!! */
    }

    db=DmOpenDatabaseByTypeCreator(appDBType,appCreatorID,
        dmModeReadWrite);

    numRecords=DmNumRecords(db);
}
 void    OpenDB(char *DBname)
{
    UInt16      error;

    error=DmCreateDatabase(0,DBname,appCreatorID,appDBType,false);

    if((error!=dmErrAlreadyExists)&&(error!=0)) {
        /* Error Occurs!! */
    }

    db=DmOpenDatabaseByTypeCreator(appDBType,appCreatorID,
        dmModeReadWrite);

    numRecords=DmNumRecords(db);
}

 void    CloseDB(void)
{
    DmCloseDatabase(db);
}
 UInt16  NewStrRecord(char *str)
{
    Handle   hrecord;
    void        *precord;
    UInt16      cursor;
    UInt32      size;

    cursor=0;
    size=StrLen(str);
    hrecord=DmNewRecord(db,&cursor,size);
    precord=MemHandleLock(hrecord);

    DmWrite(precord,0,str,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);

    return cursor;
}

/*
 UInt16  NewIntRecord(int i)
{
    Handle   hrecord;
    void        *precord;
    UInt16      cursor;
    UInt32      size;

    cursor=dmMaxRecordIndex;
    size=sizeof(int);
    hrecord=DmNewRecord(db,&cursor,size);
    precord=MemHandleLock(hrecord);

    DmWrite(precord,0,&i,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);

    return cursor;
}
 UInt16  NewBlockRecord(BlockLink BL)
{
    Handle   hrecord;
    void        *precord;
    UInt16      cursor;
    UInt32      size;

    cursor=dmMaxRecordIndex;
    size=sizeof(BlockLLNode);
    hrecord=DmNewRecord(db,&cursor,size);
    precord=MemHandleLock(hrecord);

    DmWrite(precord,0,BL,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);

    return cursor;
}
 UInt16  NewWireRecord(WireLink WL)
{
    Handle   hrecord;
    void        *precord;
    UInt16      cursor;
    UInt32      size;

    cursor=dmMaxRecordIndex;
    size=sizeof(WireLLNode);
    hrecord=DmNewRecord(db,&cursor,size);
    precord=MemHandleLock(hrecord);

    DmWrite(precord,0,WL,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);

    return cursor;
}
*/
/*
void    ModifyRecord(UInt16 cursor,struct MyRecord *r)
{
    Handle   hrecord;
    void        *precord;
    UInt32      size;

    size=sizeof(struct MyRecord);
    hrecord=DmGetRecord(db,cursor);
    precord=MemHandleLock(hrecord);

    DmWrite(precord,0,r,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);
}
*/
/*
 void    GetIntRecord(UInt16 cursor,struct MyRecord *r)
{
    Handle   hrecord;
    void        *precord;
    UInt32      size;

    size=sizeof(int);
    hrecord=DmGetRecord(db,cursor);
    precord=MemHandleLock(hrecord);

    MemMove(r,precord,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);
}
 void    GetBlockRecord(UInt16 cursor,struct MyRecord *r)
{
    Handle   hrecord;
    void        *precord;
    UInt32      size;

    size=sizeof(BlockLLNode);
    hrecord=DmGetRecord(db,cursor);
    precord=MemHandleLock(hrecord);

    MemMove(r,precord,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);
}
 void    GetWireRecord(UInt16 cursor,struct MyRecord *r)
{
    Handle   hrecord;
    void        *precord;
    UInt32      size;

    size=sizeof(WireLLNode);
    hrecord=DmGetRecord(db,cursor);
    precord=MemHandleLock(hrecord);

    MemMove(r,precord,size);

    MemHandleUnlock(hrecord);
    DmReleaseRecord(db,cursor,true);
}
*/
extern void    RemoveRecord(UInt16 cursor)
{
    DmRemoveRecord(db,cursor);
}
/*
static  Int16 compare(void* rec1, void* rec2,
    Int16 sortBy,
    SortRecordInfoPtr rec1SortInfo,
    SortRecordInfoPtr rec2SortInfo,
    Handle appInfoH)
{
    struct MyRecord *r1,*r2;
    Int16  result;

    r1=(struct MyRecord *)rec1;
    r2=(struct MyRecord *)rec2;

    switch (sortBy) {
        case MY_SORT_BY_NAME:
            result=StrCompare(r1->name,r2->name);

            break;
        case MY_SORT_BY_SEX:
            result=(r1->sex)-(r2->sex);
            break;
        case MY_SORT_BY_AGE:
            result=(r1->age)-(r2->age);
            break;
        case MY_SORT_BY_TEL:
            result=StrCompare(r1->tel,r2->tel);
            break;
    }

    return result;
}
*/
/*
void    SortDB(void)
{
    DmQuickSort(db,compare, MY_SORT_BY_NAME);
}
*/
/*
extern int CountItemN(void )
{
	int i=0;
	BlockLink BL;
	BL = SYSHOOK.BlockLLHead;
	while(BL != NULL)
	{
		i++;
		BL=BL->NEXTNODE;
	}
	return i;
}
extern int CountWireN(void )
{
	int i=0;
	WireLink WL;
	WL = SYSHOOK.WireLLHead;
	while(WL != NULL)
	{
		i++;
		WL=WL->NEXTNODE;
	}
	return i;
}
UInt16 CalculateNeededSize(void )
{
	int WLn,BLn;
//	BlockLink BL;
//	WireLink WL;
	
	BLn=CountItemN();
	WLn=CountWireN();
	return (BLn+WLn+4);	// total size
}
*/