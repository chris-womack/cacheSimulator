#include <stdio.h>
#include <stdlib.h>
#include "L1.h"
#include "L2.h"
#include "gen.h"
#include "configlib.h"

int configRead(char *file){
	if(file == NULL){ // default
		L1_block_size    = 32;
		L1_cache_size  	 = 8192;
		L1_assoc		 = 1;
		L1_hit_time		 = 1;
		L1_miss_time	 = 1;
		L2_block_size	 = 64;
		L2_cache_size	 = 32768;
		L2_assoc		 = 1;
		L2_hit_time		 = 5;
		L2_miss_time	 = 7;
		L2_transfer_time = 5;
		L2_bus_width     = 16;
		DIRECT_MAPPED	 = 1;
		FULLY_ASSOC		 = 0;
		SET_ASSOC        = 0;
		mem_sendaddr	 = 10;
		mem_ready		 = 30;
		mem_chunktime	 = 15;
		mem_chunksize	 = 8;
	}
	else{ // config file
		FILE *fr;
		int num[18];
		int numr;
		int i = 0;
	    fr = fopen (file, "r");
	    while(i < 19){ // size is always fixed
	    	fscanf(fr, "%*s %d\n", &numr);
	    	num[i] = numr;
	    	i++;
	    }
	    fclose(fr);

	    // set param from array
		L1_block_size    = num[0];
		L1_cache_size  	 = num[1];
		L1_assoc		 = num[2];
		L1_hit_time		 = num[3];
		L1_miss_time	 = num[4];
		L2_block_size	 = num[5];
		L2_cache_size	 = num[6];
		L2_assoc		 = num[7];
		L2_hit_time		 = num[8];
		L2_miss_time	 = num[9];
		L2_transfer_time = num[10];
		L2_bus_width     = num[11];
		DIRECT_MAPPED	 = num[12];
		FULLY_ASSOC		 = num[13];
		SET_ASSOC        = num[14];
		mem_sendaddr	 = num[15];
		mem_ready		 = num[16];
		mem_chunktime	 = num[17];
		mem_chunksize	 = num[18];
	}
	return 0;
}
