/* Configure Parameters */

// cache parameters
int L1_block_size;
int L1_cache_size;
int L1_assoc;
int L1_hit_time;
int L1_miss_time;
int L2_block_size;
int L2_cache_size;
int L2_assoc;
int L2_hit_time;
int L2_miss_time;
int L2_transfer_time;
int L2_bus_width;

int DIRECT_MAPPED;
int FULLY_ASSOC;
int SET_ASSOC;
int mem_sendaddr;
int mem_ready;
int mem_chunktime;
int mem_chunksize;

// cache structs
typedef struct {
    unsigned long long int addr;
    int dirty; 
 } dmEntry; //Direct Mapped

typedef struct {
    unsigned long long int addr;
    int dirty;
    unsigned long long int time; 
} faEntry; //Fully Associative	

typedef struct {
    unsigned long long int addr;
    int dirty;
    unsigned long long int time; 
} saEntry; //Set Associative


// cache arrays
dmEntry* dmL1i;
dmEntry* dmL1d;
dmEntry* dmL2;

faEntry* faL1i;
faEntry* faL1d;
faEntry* faL2;

saEntry* saL1i;
saEntry* saL1d;
saEntry* saL2;

// initializes cache parameters
int configRead(char *file);