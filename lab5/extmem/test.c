#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "utils.h"
int main(int argc, char **argv)
{
    Buffer buf; /* A buffer */
    unsigned char *blk; /* A pointer to a block */
    int i = 0;

    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    /* Get a new block in the buffer */
    blk = getNewBlockInBuffer(&buf);

    /* Fill data into the block */
    for (i = 0; i < 8; i++)
        *(blk + i) = 'a' + i;

    /* Write the block to the hard disk */
    if (writeBlockToDisk(blk, 8888, &buf) != 0)
    {
        perror("Writing Block Failed!\n");
        return -1;
    }

    /* Read the block from the hard disk */
    showBlockInBuffer(1,&buf);
    /*Utils Functions*/
    LinearRelationSelect(128,1,&buf);
    TPMMSort(0, &buf);
    // showBlock(1,&buf);

    /* Process the data in the block */

    printf("\n");
    printf("IO's is %d\n", buf.numIO); /* Check the number of IO's */
    system("pause");
    return 0;
}

