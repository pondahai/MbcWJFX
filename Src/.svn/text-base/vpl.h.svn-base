/***********************************************************************
 *
 *   Global variables
 *
 ***********************************************************************/
//static Boolean HideSecretRecords;


#define offset_x 0
#define offset_y -16
#define _PenMovAver 3

// BlockType
#define HOOKBLOCK 1
#define FUNCBLOCK 2
#define LOOPBLOCK 3
#define CASEBLOCK 4
#define CTRLBLOCK 5

// 5-3 
// 歸納出5種需要作Link List的資料
// 1. 基本元件
// 2. 結構元件
// 3. 虎克元件
// 4. 線段
// 5. I/O節點
// 6. 控制點
// 
// 這些資料結構有些是其他資料結構的內部資料
// 例如I/O節點都會被包含在 基本元件 結構元件 以及 虎克元件中
//Aspect 外觀
struct Point{
	UInt16 x;
	UInt16 y;
};


struct IONODE{
	UInt16 ID;
	UInt16 TYPE; 					//1 = input , 0 = output ,2 = drag point
	Boolean NotReady;
	void *P;						// 需要被調整大小
	UInt16 BYTES;
	UInt16 NUMS;
	struct Point TL; // 接點外觀
	struct Point EXT;			//
	struct IONODE *PREV;
	struct IONODE *NEXT;
};
typedef struct IONODE *IONODEP;

struct CTRLNODE{						// Panel 元件控制點
	UInt16 ID;						// 或是 Block 元件外觀改變點
	struct Point TL;
	struct Point EXT;
	struct CTRLNODE *PREV;
	struct CTRLNODE *NEXT;
};
typedef struct CTRLNODE *CTRLNODEP;

typedef struct {
	UInt16 BITMAPID;
	struct Point XY;
	struct Point SIZE;
	IONODEP IONodeLLHead;		// 元件的IO點鏈結串列
	//UInt16 IONodeNUM;
	int INodeNUM;
	int ONodeNUM;
	CTRLNODEP CTRLNodeLLHead;	// 給結構元件的外觀控制點用
	Boolean DrawBorder;// 
	Boolean ReDraw;
	Boolean Clean; // 5-13 新增清除指標 讓 畫 與 清除命令分開
}BA;
typedef BA *BAP;
typedef struct {
	UInt16 BITMAPID;
	struct Point XY;
	struct Point SIZE;	
	CTRLNODEP CTRLNodeLLHead;//
	Boolean ReDraw;
}PA;
typedef PA *PAP;

// HOOKBLOCK 1
// FUNCBLOCK 2
// LOOPBLOCK 3
// CASEBLOCK 4

// 5-5 最後還是決定結構合併，這樣程式比較好寫也比較好看
struct BlockNode{
	UInt16 ID;
	UInt16 TYPE;
	BAP bap;	// 方塊外觀
	PAP pap;	// 面板外觀
	Ptr LoopBlockHOOKP;		//迴圈用的
	Ptr CaseBlockHOOKLLHeadP; //Switch case 用
	Ptr CurrentHOOKP;			//Switch case 用
	Ptr hookP;		// 虎克元件
	char *filename;	// 虎克元件
	struct BlockNode *SelfBlockLLHead;		// 本身所屬串列頭
	struct BlockNode *BackgroundBlockLLHead;// 目前背景所處
	struct BlockNode *PREVNODE;				// 前串列元件
	struct BlockNode *NEXTNODE;				// 後串列元件
};

typedef struct BlockNode BN;
typedef BN *BNP;

//BlockType Items[5];
//LineType WireItem[5];
/*
typedef struct{
	UInt16 WireID;
	//Uint16 LineStyle;
	UInt16 StartBlockID;
	Ptr    StartBlockP;
	UInt16 StartNodeID;
	UInt16 EndBlockID;
	Ptr    EndBlockP;
	UInt16 EndNodeID;
}LineType;
*/
struct wList{
	//LineType wire;
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
};

typedef struct wList WN;
typedef WN *WNP;



//
/*
#define HAND 1
#define THREAD 2
#define ARROW 3
#define FINGER 4
#define SCISSOR 5
#define KILL 6
*/

//int WhichTool=HAND;			// now is which tool?1=hand 2=thread


struct hand{
UInt16 HandState ;
BNP CatchWhat;
//BNP Clone;
struct Point RelativePoint;
//BlockLink StartbllpH;
};

// ver. 3
/*
struct SysStruct{
BlockLink BlockLLHead;
WireLink WireLLHead;
};

typedef struct SysStruct SysHead;
*/
#define menuLoad true
#define functionLoad false

// 那個元件的那個節點
typedef struct{
	BNP BlockP;
	IONODEP NodeP;
}EndPoint;

typedef struct {
	UInt16 BitMapIDInArea;
	Boolean InArea;
	BNP LLHead;
	BNP llp;
}FUNCTIONSLLDATAtype;

typedef struct {
	Boolean InArea;
	BNP llp;
}InAreaInfType;

struct HOOK{
	char *name; 				// switch case用
	BNP BlockLLHeadP;
	WNP WireLLHeadP;
	struct HOOK *PREV;	// switch case用
	struct HOOK *NEXT;	// switch case用
};
typedef struct HOOK *HOOKP;

// 控制點的定義值
#define DRAGNODE 1
#define SCPAGEUP 2
#define SCPAGEDOWN 3
#define CONTROLUP 4
#define CONTROLDOWN 5
#define CONTROLPUSH 6

/***********************************************************************
 *
 *   Internal Constants
 *
 ***********************************************************************/

#define appFileCreator	'wjfx'
#define appVersionNum              0x01
#define appPrefID                  0x00
#define appPrefVersionNum          0x01

#define appCreatorID    'wjfx'
#define appDBType       'save'

// Define the minimum OS version we support
#define ourMinVersion	sysMakeROMVersion(2,0,0,sysROMStageRelease,0)
