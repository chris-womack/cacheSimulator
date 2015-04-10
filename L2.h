/* L2 Cache Simulator Library */

// // Search for address in L2. Returns 0 if false, 1 if true.
int inL2(unsigned long long int address);

// Writes address to L2. Must be a block address.
// DOES NOT SPAN BLOCKS!
int loadL2(unsigned long long int address);

// Marks dirty bit for L2.
int dirtyL2(int value, unsigned long long int address);