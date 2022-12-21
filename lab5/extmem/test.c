#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "utils.h"
int main(int argc, char **argv)
{
    Buffer buf; /* A buffer */
    unsigned char *blk; /* A pointer to a block */
    int i = 0;
    int R_last_resorted_blk;

    int S_last_resorted_blk;
    int S_last_index_blk;
    /* Initialize the buffer */
    if (!initBuffer(520, 64, &buf))
    {
        perror("Buffer Initialization Failed!\n");
        return -1;
    }

    // /* Get a new block in the buffer */
    // blk = getNewBlockInBuffer(&buf);

    // /* Fill data into the block */
    // for (i = 0; i < 8; i++)
    //     *(blk + i) = 'a' + i;

    // /* Write the block to the hard disk */
    // if (writeBlockToDisk(blk, 8888, &buf) != 0)
    // {
    //     perror("Writing Block Failed!\n");
    //     return -1;
    // }

    /* Read the block from the hard disk */
    // showBlockInBuffer(1,&buf);
    /*Utils Functions*/
    LinearRelationSelect(128,1,&buf);
    // TPMMSort(0, &buf);
    // showBlock(1,&buf);
        // 任务二，两阶段多路归并排序算法
    // 对R排序，结果存入301-316号磁盘块
    TPMMSort(1,16,301,&R_last_resorted_blk, &buf);
    // 对S排序，结果存入317-348号磁盘块
    TPMMSort(17,48,317,&S_last_resorted_blk, &buf);
    /* Process the data in the block */
    makeIndex(317,S_last_resorted_blk,401,&S_last_index_blk, &buf);
    indexSearch(401,S_last_index_blk,501,128,&buf);

    SortMergeJoin(301, 316, 317, 348, 601,&buf);

    makeIntersection(301, 316, 317, 348, 701, &buf);
    // printf("\n");
    // printf("IO's is %d\n", buf.numIO); /* Check the number of IO's */
    system("pause");
    return 0;
}

