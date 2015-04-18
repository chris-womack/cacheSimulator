#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"


//#define DEBUG

//**********************************************************//
//**************** Notes for next time *********************//
//**********************************************************//

// Also note: For cache flush use loadL1i and loadL1d and fill with 0 entries. 
// This way the load logic can handle all of the dirty stuff since load is going to 
// return whether we wrote back/ how many cycles it took, this should work for timing 
// the cache flush. The cache flush also is going to return how many cycles it took.

int main(int argc, char *argv[]){
    char op;
    unsigned long long address;
    unsigned int bytesize;

    int i = 0;
    int numwords = 0;
    unsigned long long wordstart;

    //** Counters init **//
    refCount = 0;
    invalidCount = 0;
    totalCycle = 0;

    // flush
    flushTime = 0;
    flushCount = 0;

    // instructions
    instCount = 0;
    instCycle = 0;
    avgInstCycle = 0;

    // data
    dataCount = 0;
    dataCycle = 0;

    // reads
    readCount = 0;
    readCycle = 0;
    avgReadCycle = 0;

    // writes
    writeCount = 0;
    writeCycle = 0;
    avgWriteCycle = 0;

    // L1i
    l1iMissCount = 0;
    l1iHitCount = 0;
    l1iRefCount = 0;
    l1iKickouts = 0;
    l1iDirtyKickouts = 0;
    l1iTransfers = 0;
    l1iFlushKickouts = 0;

    // L1d
    l1dMissCount = 0;
    l1dHitCount = 0;
    l1dRefCount = 0;
    l1dKickouts = 0;
    l1dDirtyKickouts = 0;
    l1dTransfers = 0;
    l1dFlushKickouts = 0;

    // L2
    l2MissCount = 0;
    l2HitCount = 0;
    l2RefCount = 0;
    l2Kickouts = 0;
    l2DirtyKickouts = 0;
    l2Transfers = 0;
    l2FlushKickouts = 0;
    
    if(argv[1])
        configRead(argv[1]);
    else
        configRead(NULL);

    blockNumL1 = 0;
    blockNumL2 = 0;
    numBitsL1 = 0;
    numBitsL2 = 0;

    // get number of bits for blocks
    blockNumL1 = (L1_cache_size/L1_block_size);
    blockNumL2 = (L2_cache_size/L2_block_size);
    numBitsL1 = log2(blockNumL1);
    numBitsL2 = log2(blockNumL2);
    numByteBounL1 = log2(L1_block_size);
    numByteBounL2 = log2(L2_block_size);

    #ifdef DEBUG
        printf("# of blocks in L1 %llu\n", blockNumL1);
        printf("# of blocks in L2 %llu\n", blockNumL2);
        printf("# of bits for L1 %llu\n", numBitsL1);
        printf("# of bits for L2 %llu\n", numBitsL2);
        printf("\n");
    #endif /* DEBUG */

    // initialize cache arrays from config
    dmL1i = calloc(blockNumL1,sizeof(dmEntry));
    dmL1d = calloc(blockNumL1,sizeof(dmEntry));
    dmL2  = calloc(blockNumL2,sizeof(dmEntry));

    saL1i = calloc(blockNumL1,sizeof(saEntry));
    saL1d = calloc(blockNumL1,sizeof(saEntry));
    saL2  = calloc(blockNumL2,sizeof(saEntry));

    // 
    while(scanf("%c %Lx %d\n",&op,&address,&bytesize)==3){
        if(op=='I')
            instCount++;
        else if (op=='R')
            readCount++;
        else 
            writeCount++;

        numwords =(bytesize+(address%4))/4; 
        if((bytesize+(address%4))%4 != 0){ //this logic seems right, but may be questionable. Double check if it doens't work.
            numwords ++;
        }

        #ifdef DEBUG
            printf("Reference address: %Lx\n", address);
        #endif /* DEBUG */

        wordstart = (address-(address%4));
        for (i = 0; i<numwords; i++){
            int temp;

            #ifdef DEBUG
                printf("\n");
                printf("Wordstart from reference %Lx\n", wordstart+(4*i));
                printf("Index from address for L1: %Lx\n", ((wordstart+(4*i)) >> numByteBounL1) % blockNumL1);
                printf("Tag from address for L1: %Lx\n", wordstart+(4*i) >> numBitsL1);
                printf("Index from address for L2: %Lx\n", ((wordstart+(4*i)) >> numByteBounL2) % blockNumL2);
                printf("Tag from address for L2: %Lx\n", wordstart+(4*i) >> numBitsL2);
            #endif /* DEBUG */

            temp = access(op,wordstart+(4*i));
            totalCycle+=temp; //access each necessary word.
          //  printf("Access time: %d \n", temp);
        }

        #ifdef DEBUG
            printf("Number of words: %d\n",numwords);
            printf("Size of ref in bytes %u\n", bytesize);
            printf("\n");
        #endif /* DEBUG */
        //if((totalexec % 380000) == 0 && totalexec != 0 )
        //    cache_flush(); 
    }

    printOutput();
    printValid();
    printL1();
    printL2();

    // dealloc cache arrays
    free(dmL1i);
    free(dmL1d);
    free(dmL2);

    free(saL1i);
    free(saL1d);
    free(saL2);

    return 0;
}
