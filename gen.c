#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"

// #define DEBUG

unsigned long long log2(unsigned long long num){
    unsigned long long count = 0;
    while(num >>= 1)
        count++;
    return count;
}

int cacheFlush(){
    int i;
    for(i =0; i < (L2_cache_size/L1_block_size); i++){

    }
    return 0; 
}; 

int access(char op, unsigned long long int address){
    unsigned long long int cycles = 0;
    
    if(op == 'R'){ // read
        if(inL1d(address)){
            cycles+=L1_hit_time;
            l1dHitCount++;
        }
        else{
            cycles+=L1_miss_time;
            l1dMissCount++;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL1d(address); // put it in the L1 cache after accessing it from teh L2 cache. 
                l2HitCount++;
            }
            else{
                cycles+=L2_miss_time+mem_sendaddr+mem_ready + (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL2(address);
                cycles+=loadL1d(address);
                l2MissCount++;
            }
        }
        readCycle += cycles;
    }
    else if(op == 'I'){ // instruction
        #ifdef DEBUG
            printf("inL1i returns %d\n", inL1i(address));
        #endif /* DEBUG */
        if(inL1i(address)){   
            cycles+=L1_hit_time;
            l1iHitCount++;
        }
        else{
            cycles+=L1_miss_time;
            l1iMissCount++;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                loadL1i(address); // put it in the L1 cache after accessing it from the L2 cache. Note: don't care about writebacks since instructions won't.
                l2HitCount++;
            }
            else{
                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL2(address);
                loadL1i(address);
                l2MissCount++;
            }
        }
        instCycle += cycles;
    }
    else if(op == 'W'){ // write
        if(inL1d(address)){
            cycles+=L1_hit_time;
            l1dHitCount++;
        }
        else{
            cycles+=L1_miss_time;
            l1dMissCount++;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL1d(address); // put it in the L1 cache after accessing it from teh L2 cache.
                l2HitCount++;
            }
            else{
                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL2(address);
                cycles+=loadL1d(address);
                l2MissCount++;
            }
        }
        writeCycle += cycles;
        dirtyL1d(1, address); // we mark the L1 block dirty no matter what. L2 is left clean as per the instructions.
    }
    else{
        return -1; 
    }
    return cycles;
}

int printOutput(){
    dataCount = readCount + writeCount;
    refCount = instCount + dataCount;
    l1iRefCount = l1iMissCount+l1iHitCount;
    l1dRefCount = l1dMissCount+l1dHitCount;
    l2RefCount = l2MissCount+l2HitCount;


    printf("Memory system:\n");
    printf("Dcache size = %llu : ways = %llu : block size = %llu\n", L1_cache_size, L1_assoc, L1_block_size);
    printf("Icache size = %llu : ways = %llu : block size = %llu\n", L1_cache_size, L1_assoc, L1_block_size);
    printf("L2-cache size = %llu : ways = %llu : block size = %llu\n", L2_cache_size, L2_assoc, L2_block_size);
    printf("Memory ready time = %d : chunksize = %d : chunktime = %d\n", mem_ready, mem_chunksize, mem_chunktime); 

    printf("\n");
    printf("Execute time    =   %llu;   Total refs = %llu\n", totalCycle, refCount);
    printf("Flush time      =   %llu\n", flushTime);
    printf("Inst refs       =   %llu;   Data refs = %llu\n", instCount, dataCount);

    printf("\n");
    printf("Number of reference types:  [Percentage]\n");
    printf("    Reads   =        %llu   [ %.1f%%]\n", readCount, (float)readCount / (float)refCount *100.0);
    printf("    Writes  =        %llu   [ %.1f%%]\n", writeCount, (float)writeCount / (float)refCount *100.0);
    printf("    Inst.   =        %llu   [ %.1f%%]\n", instCount, (float)instCount / (float)refCount *100.0);
    printf("    Total   =        %llu\n", refCount);
  
    printf("\n");
    printf("Total cycles for activities:  [Percentage]\n");
    printf("    Reads   =       %llu    [%.1f%%]\n", readCycle, (float)readCycle / (float)totalCycle *100.0);
    printf("    Writes  =       %llu    [%.1f%%]\n", writeCycle, (float)writeCycle / (float)totalCycle *100.0);
    printf("    Inst.   =       %llu    [%.1f%%]\n", instCycle, (float)instCycle / (float)totalCycle *100.0);
    printf("    Total   =       %llu\n", totalCycle);

    printf("\n");
    printf("Average goes here\n");

    printf("\n");
    printf("Memory Level:  L1i\n");
    printf("    Hit Count = %llu  Miss Count = %llu\n", l1iHitCount, l1iMissCount);
    printf("    Total Requests = %llu\n", l1iRefCount);
    printf("    Hit Rate = %.1f%%   Miss Rate = %.1f%%\n", (float)l1iHitCount/(float)l1iRefCount * 100, (float)l1iMissCount/(float)l1iRefCount * 100);
    printf("    Kickouts = %llu; Dirty kickouts = %llu; Transfers = %llu\n", l1iKickouts, l1iDirtyKickouts, l1iTransfers);
    printf("    Flush Kickouts = %llu\n", l1iFlushKickouts);

    printf("\n");
    printf("Memory Level:  L1d\n");
    printf("    Hit Count = %llu  Miss Count = %llu\n", l1dHitCount, l1dMissCount);
    printf("    Total Requests = %llu\n", l1dRefCount);
    printf("    Hit Rate = %.1f%%   Miss Rate = %.1f%%\n", (float)l1dHitCount/(float)l1dRefCount * 100, (float)l1dMissCount/(float)l1dRefCount * 100);
    printf("    Kickouts = %llu; Dirty kickouts = %llu; Transfers = %llu\n", l1dKickouts, l1dDirtyKickouts, l1dTransfers);
    printf("    Flush Kickouts = %llu\n", l1dFlushKickouts);

    printf("\n");
    printf("Memory Level:  L2\n");
    printf("    Hit Count = %llu  Miss Count = %llu\n", l2HitCount, l2MissCount);
    printf("    Total Requests = %llu\n", l2RefCount);
    printf("    Hit Rate =  %.1f%%   Miss Rate = %.1f%%\n", (float)l2HitCount/(float)l2RefCount * 100, (float)l2MissCount/(float)l2RefCount * 100);
    printf("    Kickouts = %llu; Dirty kickouts = %llu; Transfers = %llu\n", l2Kickouts, l2DirtyKickouts, l2Transfers);
    printf("    Flush Kickouts = %llu\n", l2FlushKickouts);
}

int printValid(){
    int i;
    printf("\n");
    printf("Cache final contents - Index and Tag values are in HEX\n");
    printf("\n");
    printf("Memory Level: L1i\n");
    for(i = 0; i < ((L1_cache_size)/L1_block_size); i++){
        if(dmL1i[i].addr != 0)
            printf("Index:    %x | V:%d D:%d Tag:    %llx |\n", i, dmL1i[i].valid, dmL1i[i].dirty, dmL1i[i].addr >> numBitsL1);
    }
    printf("\n");
    printf("Memory Level: L1d\n");
    for(i = 0; i < ((L1_cache_size)/L1_block_size); i++){
        if(dmL1d[i].addr != 0)
            printf("Index:    %x | V:%d D:%d Tag:    %llx |\n", i, dmL1d[i].valid, dmL1d[i].dirty, dmL1d[i].addr >> numBitsL1);
    }
    printf("\n");
    printf("Memory Level: L2\n");
    for(i = 0; i < (L2_cache_size/L2_block_size); i++){
        if(dmL2[i].addr != 0)
           printf("Index:    %x | V:%d D:%d Tag:    %llx |\n", i, dmL2[i].valid, dmL2[i].dirty, dmL2[i].addr >> numBitsL2);
    }
}

int printL1(){
    int i;
    printf("\n");
    printf("All Blocks in L1 Instruction Cache\n");
    printf("\n");
    for(i = 0; i < ((L1_cache_size)/L1_block_size); i++){
        printf("Block: %llx   Tag: 0x%llx     Address: 0x%llx     Dirty: %d   Valid: %d\n", dmL1i[i].addr % blockNumL1, dmL1i[i].tag, dmL1i[i].addr, dmL1i[i].dirty, dmL1i[i].valid);
    }
    printf("\n");
    printf("All Blocks in L1 Data Cache\n");
    printf("\n");
    for(i = 0; i < ((L1_cache_size)/L1_block_size); i++){
        printf("Block: %llx   Tag: 0x%llx     Address: 0x%llx     Dirty: %d   Valid: %d\n", dmL1d[i].addr % blockNumL1, dmL1d[i].tag, dmL1d[i].addr, dmL1d[i].dirty, dmL1d[i].valid);
    }
}

int printL2(){
    int i;
    printf("\n");
    printf("All Blocks in L2 Cache\n");
    printf("\n");
    for(i = 0; i < (L2_cache_size/L2_block_size); i++){
        printf("Block: %llx   Tag: 0x%llx     Address: 0x%llx     Dirty: %d   Valid: %d\n", dmL2[i].addr % blockNumL2, dmL2[i].tag, dmL2[i].addr, dmL2[i].dirty, dmL2[i].valid);
    }
}