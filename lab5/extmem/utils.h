#ifndef UTILS_H
#define UTILS_H
#include"extmem.h"
#define R 0
#define S 1

typedef struct TUPLE2d
{
    char attr1[5];
    char attr2[5];
}tuple2d;
void quick_sort(unsigned char*, int, int);
void LinearRelationSelect(int, int,Buffer*);
void TPMMSort(int, Buffer*);
void IndexRelationSelection();
void SortMergeJoin();
void SortTwinSearch();
void HashTwinSearch();
int getAttrL();
int getAttrR();
void showBlockInBuffer(int, Buffer*);
void showBlock(unsigned char*);
#endif