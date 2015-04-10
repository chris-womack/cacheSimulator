/* L1 Cache Simulator Library */

// Search for address in L1. Returns 0 if false, 1 if true.
int inL1i(unsigned long long int address); // instruction
int inL1d(unsigned long long int address); // data

// Writes address to L1. Must be a block address.
// DOES NOT SPAN BLOCKS!
int loadL1i(unsigned long long int address); // instruction
int loadL1d(unsigned long long int address); // data

// Marks dirty bit for L1.
int dirtyL1i(int value, unsigned long long int address); // instruction
int dirtyL1d(int value, unsigned long long int address); // data