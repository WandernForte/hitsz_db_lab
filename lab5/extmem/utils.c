#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <windows.h>
#include"utils.h"
/**
 * @brief  线性关系搜索选择算法
 * 
 * @param targetVal 搜索的目标值(这里的实现明显是不合理的) 
 * @param targetRel 搜索的目标关系
 * @param targetWay 搜索的是第几个属性(可能用不上)
 * @param buf 
 */
void LinearRelationSelect(int targetVal, int targetRel, Buffer* buf){
    int init_addr=0;
    int lmt_addr=16;
    if(targetRel>0){
        init_addr=16;
        lmt_addr=48;
    }
    else {
        init_addr=0;
        lmt_addr=16;
    }
    int X = -1;
    int Y = -1;
    char str[5];
    for(int i=init_addr;i<lmt_addr;i++){
        unsigned char *blk=readBlockFromDisk(i+1, buf);
        printf("Reading Block:%d...\n", i+1);
        for(int j=0;j<TUPLE_PER_BLOCK;j++){
            for (int k = 0; k < 4; k++)
            {
                str[k] = *(blk + j*8 + k);
            }
            X = atoi(str);
            for (int k = 0; k < 4; k++)
            {
                str[k] = *(blk + j*8 + 4 + k);
            }
            Y = atoi(str);
            if(X==targetVal) {
                printf("(%d, %d) \n", X, Y);
            }

            }
            freeBlockInBuffer(blk,buf);
    }
    }

/**
 * @brief 二阶段归并排序算法, 具体实现是块内先排序，然后再归并排序
 * 
 * @param setBlockSize 子集合中的块数,(可能也用不到？)
 * @param targetRel 选择的关系
 */

void TPMMSort(int targetRel, Buffer* buf){
    //stage1:
    printf("Sorting...\n");
    int init_w_blk=301;
    int w_blk=0;
    int init_addr=0;
    int lmt_addr=16;
    char str[5];
    int X = -1;
    int Y = -1;
    if(targetRel>0){
        init_addr=16;
        lmt_addr=48;
    }
    else {
        init_addr=0;
        lmt_addr=16;
    }
    //首先清空整个缓冲区
    freeAllBlockInBuffer(buf);
    //子集合内快速排序
    for(int addr=init_addr;addr<lmt_addr;addr++){
        unsigned char *blk=readBlockFromDisk(addr+1, buf);
        Sleep(10);
        printf("Reading Block:%d...\n", addr+1);
        if(addr%8==7){
        quick_sort(buf->data, 0, buf->numAllBlk*(TUPLE_PER_BLOCK)-1);

        for(int blkid=0;blkid < buf->numAllBlk;blkid++){
            writeBlockToDisk((buf->data+blkid),init_w_blk+w_blk,buf);
            // buf->data=buf->data++;
            blk++;
        }
        freeAllBlockInBuffer(buf);
        }
        
    }
        for(int addr=init_addr;addr<lmt_addr;addr++){
            unsigned char *blk=readBlockFromDisk(init_w_blk+addr, buf);
            showBlock(blk);
            if(buf->numFreeBlk==0){
                freeAllBlockInBuffer(buf);
            }
        }
    
}
void quick_sort(unsigned char* blk, int l, int r)
{
    if (l < r)
    {
        //Swap(s[l], s[(l + r) / 2]); 

        int i = l, j = r; char x[5],trap1[5], trap2[9];
        for(int idx=0;idx<8;idx++){
            trap2[idx]=*(blk + l*8 + idx);
            *(blk + l*8 + idx)=*(blk + (l+r)/2*8 + idx);
            *(blk + (l+r)/2*8 + idx)=trap2[idx];
        }
        for(int idx=0;idx<4;idx++){
            x[idx]=*(blk + l*8 + idx);
        }
        for(int idx=0;idx<8;idx++){
            trap2[idx]=*(blk + l*8 + idx);
        }
        while (i < j)
        {   
            for(int idx=0;idx<4;idx++){
                trap1[idx]=*(blk + j*8 + idx);
            }
            while(i < j && atoi(trap1) >= atoi(x)) // 从右向左找第一个小于x的数
                j--;  
            if(i < j)
            { 
                // records[i++] = records[j];
            for(int idx=0;idx<4;idx++){
                *(blk + i*8 + idx)=*(blk + j*8 + idx);
                *(blk + i*8 +4+ idx)=*(blk + j*8 +4+ idx);
                
            }
            i++;
            }
            for(int idx=0;idx<4;idx++){
                trap1[idx]=*(blk + i*8 + idx);
            }
            while(i < j && atoi(trap1) < atoi(x)) // 从左向右找第一个大于等于x的数
                i++;  
            if(i < j) {
                // records[j--] = records[i];
                for(int idx=0;idx<4;idx++){
                *(blk + j*8 + idx)=*(blk + i*8 + idx);
                *(blk + j*8 +4+ idx)=*(blk + i*8 +4+ idx);
                
            }
            j--;
            }
        }
        // trap1 = x;
        // for()
        for(int idx=0;idx<8;idx++)
            *(blk + i*8+idx)=trap2[idx];
        quick_sort(blk, l, i - 1); // 递归调用 
        quick_sort(blk, i + 1, r);
    }
}
void IndexRelationSelection(){

}

void SortMergeJoin(){

}

void SortTwinSearch(){

}

void HashTwinSearch(){

}
int getAttrL(){
    return 0;
}

int getAttrR(){
    return 0;
}
void showBlockInBuffer(int blkid, Buffer* buf){
    int i=0;
    unsigned char *blk; /* A pointer to a block */
    if ((blk = readBlockFromDisk(blkid, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        return ;
    }

    /* Process the data in the block */
    int X = -1;
    int Y = -1;
    int addr = -1;

    char str[5];
    printf("block %d:\n",blkid);
    for (i = 0; i < 7; i++) //一个blk存7个元组加一个地址
    {

        for (int k = 0; k < 4; k++)
        {
            str[k] = *(blk + i*8 + k);
        }
        X = atoi(str);
        for (int k = 0; k < 4; k++)
        {
            str[k] = *(blk + i*8 + 4 + k);
        }
        Y = atoi(str);
        printf("(%d, %d) ", X, Y);
    }
    for (int k = 0; k < 4; k++)
    {
        str[k] = *(blk + i*8 + k);
    }
    addr = atoi(str);
    printf("\nnext address = %d \n", addr);
}
void showBlock(unsigned char* blk){
    int X = -1;
    int Y = -1;
    int addr = -1;
    int i=0;
    char str[5];
    for (i = 0; i < 7; i++) //一个blk存7个元组加一个地址
    {

        for (int k = 0; k < 4; k++)
        {
            str[k] = *(blk + i*8 + k);
        }
        X = atoi(str);
        for (int k = 0; k < 4; k++)
        {
            str[k] = *(blk + i*8 + 4 + k);
        }
        Y = atoi(str);
        printf("(%d, %d) ", X, Y);
    }
    for (int k = 0; k < 4; k++)
    {
        str[k] = *(blk + i*8 + k);
    }
    addr = atoi(str);
    printf("\nnext address = %d \n", addr);
}