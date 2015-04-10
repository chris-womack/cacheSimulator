#include <stdio.h>
#include <stdlib.h>
#include "gen.h"
#include "configlib.h"

int cache_flush(){
    return 0; 
}; 

int access(char op, unsigned long long int address){
    unsigned long long int cycles = 0;
    
    if(op == 'R'){ // read
        if (inL1d(address)){
            cycles+=L1_hit_time;
        }
        else{
            cycles+=L1_miss_time;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL1d(address); // put it in the L1 cache after accessing it from teh L2 cache. 
            }
            else{
                    cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                    cycles+=loadL2(address);
                    cycles+=loadL1d(address);
            }
        }
    }
    else if(op == 'I'){ // instruction
        if (inL1i(address))
            cycles+=L1_hit_time;
        else{
            cycles+=L1_miss_time;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                loadL1i(address); // put it in the L1 cache after accessing it from the L2 cache. Note: don't care about writebacks since instructions won't.
            }
            else{
                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL2(address);
                loadL1i(address);
            }
        }
    }
    else if(op == 'W'){ // write
        if (inL1d(address)){
            cycles+=L1_hit_time;
        }
        else{
            cycles+=L1_miss_time;
            if(inL2(address)){ // not in L1 cache but in L2 
                cycles+=L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL1d(address); // put it in the L1 cache after accessing it from teh L2 cache.
            }
            else{
                cycles+=L2_miss_time+mem_sendaddr+mem_ready+ (((float)(mem_chunktime*L2_block_size))/((float) mem_chunksize))+L2_hit_time+L1_hit_time+(L2_transfer_time*( ((float) L1_block_size) / ((float ) L2_bus_width) ));
                cycles+=loadL2(address);
                cycles+=loadL1d(address);
            }
        }
        dirtyL1d(1,address); // we mark the L1 block dirty no matter what. L2 is left clean as per the instructions.
    }
    else{
        return -1; 
    }
    return cycles;
}