#include "SFMT.h"
#define BLOCK_SIZE 100000
#define BLOCK_SIZE64 50000
#define COUNT 1000
static w128_t array1[BLOCK_SIZE / 4];
static w128_t array2[10000 / 4];
int main(){



    int i, j;
    uint32_t *array32 = (uint32_t *)array1;
    sfmt_t sfmt;

    if (sfmt_get_min_array_size32(&sfmt) > BLOCK_SIZE) {
	printf("array size too small!\n");
        return 0;
    }
    /* 32 bit generation */
    sfmt_init_gen_rand(&sfmt, 1234);
    for (i = 0; i < 10; i++) {
	for (j = 0; j < COUNT; j++) {
	    //sfmt_fill_array32(&sfmt, array32, BLOCK_SIZE);
	}
    }
    for(int k=0; k<2100000000; k++)
        sfmt_genrand_uint32(&sfmt);

}

