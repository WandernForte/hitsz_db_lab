#ifndef UTILS_H
#define UTILS_H
#include"extmem.h"
#include <stdbool.h>
#define R 0
#define S 1
// 
typedef struct TUPLE2d
{
    int x;
    int y;
}tuple2d;
void quick_sort(unsigned char*, int, int);
tuple2d readTupleFromBlock(const unsigned char *, int);
int getNextAddress(const unsigned char*);
void writeAddressToBlock(unsigned char*, int);
void writeTupleToBlock(unsigned char*, int, tuple2d);
void writeBufferToBlockAndDisk(unsigned char **,int *,int *,Buffer*);
void writeTupleToBlockAndDisk(unsigned char **, int *, int *, tuple2d , Buffer*);
//MISSION 1
void LinearRelationSelect(int, int,Buffer*);
//MISSION 2
void TPMMS_1(int, int, int, Buffer*);
void TPMMS_2(int, int, int, int *, Buffer*);
void TPMMSort(int, int, int, int *, Buffer*);

//MISSION 3
void makeIndex(int, int, int, int *, Buffer*);
void writeJoinTtupleToBlock(unsigned char *, int, tuple2d, tuple2d);
void writeJoinTupleToBlockAndDisk(unsigned char **, int *, int *, tuple2d, tuple2d, Buffer*);
void indexSearch(int, int, int, int, Buffer*);
//MISSION 4
void SortMergeJoin(int, int, int, int, int, Buffer*);
//MISSION 5
void makeIntersection(int, int, int, int, int, Buffer*);
void showBlockInBuffer(int, Buffer*);
void showBlock(unsigned char*);
void showBuffer(unsigned char*);
#endif