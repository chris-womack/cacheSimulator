/* Configure Parameters */

// cache parameters
unsigned long long L1_block_size;
unsigned long long L1_cache_size;
unsigned long long L1_assoc;
unsigned long long L1_hit_time;
unsigned long long L1_miss_time;
unsigned long long L2_block_size;
unsigned long long L2_cache_size;
unsigned long long L2_assoc;
unsigned long long L2_hit_time;
unsigned long long L2_miss_time;
unsigned long long L2_transfer_time;
unsigned long long L2_bus_width;

int DIRECT_MAPPED;
int FULLY_ASSOC;
int SET_ASSOC;
int mem_sendaddr;
int mem_ready;
int mem_chunktime;
int mem_chunksize;

unsigned long long blockNumL1;
unsigned long long blockNumL2;
unsigned long long numBitsL1;
unsigned long long numBitsL2;

unsigned long long numByteBounL1;
unsigned long long numByteBounL2;

unsigned long long refCount;
unsigned long long invalidCount;
unsigned long long totalCycle;

// flush
unsigned long long flushTime;
unsigned long long flushCount;

// instructions
unsigned long long instCount;
unsigned long long instCycle;
unsigned long long avgInstCycle;

// data
unsigned long long dataCount;
unsigned long long dataCycle;

// reads
unsigned long long readCount;
unsigned long long readCycle;
unsigned long long avgReadCycle;

// writes
unsigned long long writeCount;
unsigned long long writeCycle;
unsigned long long avgWriteCycle;

// L1i
unsigned long long l1iMissCount;
unsigned long long l1iHitCount;
unsigned long long l1iRefCount;
unsigned long long l1iKickouts;
unsigned long long l1iDirtyKickouts;
unsigned long long l1iTransfers;
unsigned long long l1iFlushKickouts;

// L1d
unsigned long long l1dMissCount;
unsigned long long l1dHitCount;
unsigned long long l1dRefCount;
unsigned long long l1dKickouts;
unsigned long long l1dDirtyKickouts;
unsigned long long l1dTransfers;
unsigned long long l1dFlushKickouts;

// L2
unsigned long long l2MissCount;
unsigned long long l2HitCount;
unsigned long long l2RefCount;
unsigned long long l2Kickouts;
unsigned long long l2DirtyKickouts;
unsigned long long l2Transfers;
unsigned long long l2FlushKickouts;

// cache structs
typedef struct {
    unsigned long long addr;
    unsigned long long tag;
    int dirty;
    int valid; 
 } dmEntry; //Direct Mapped

struct node {
    unsigned long long addr;
    unsigned long long tag;
    int dirty;
    int valid; 
    struct node *next;
    struct node *prev;
}; //Fully Associative	
typedef struct node faEntry;

typedef struct {
    unsigned long long addr;
    unsigned long long tag;
    int dirty;
    int valid; 
} saEntry; //Set Associative


// cache arrays
dmEntry* dmL1i;
dmEntry* dmL1d;
dmEntry* dmL2;

faEntry* faL1iHead;
faEntry* faL1iCurr;
faEntry* faL1iTail;
faEntry* faL1dHead;
faEntry* faL1dCurr;
faEntry* faL1dTail;
faEntry* faL2Head;
faEntry* faL2Curr;
faEntry* faL2Tail;

saEntry* saL1i;
saEntry* saL1d;
saEntry* saL2;

// initializes cache parameters
int configRead(char *file);