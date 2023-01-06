#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
#include"utils.h"

/**
 * @brief 从块中读取元组记录
 * 
 * @param blk 
 * @param offset 
 * @return tuple2d 
 */
tuple2d readTupleFromBlock(const unsigned char *blk, int offset) {
    tuple2d t;
    char str[5] = "";
    for (int i = 0; i < 4; i++) {
        str[i] = (char) *(blk + offset * 8 + i);
    }
    t.x = atoi(str);
    for (int i = 0; i < 4; i++) {
        str[i] = (char) *(blk + offset * 8 + 4 + i);
    }
    t.y = atoi(str);
    return t;
}

/**
 * @brief 元组写入块，块满了写入磁盘
 * 
 * @param blk 
 * @param addr 
 * @param offset 
 * @param t 
 * @param buf 
 */
void writeTupleToBlockAndDisk(unsigned char **blk, int *addr, int *offset, tuple2d t, Buffer* buf)
{
    if (*offset >= 7)
    {
        writeAddressToBlock(*blk, *addr + 1);
        if (writeBlockToDisk(*blk, *addr, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            exit(-1);
        }
        printf("Output To Disk: %d\n", *addr);
        (*addr)++;
        *blk = getNewBlockInBuffer(buf);
        *offset = 0;
    }
    writeTupleToBlock(*blk, *offset, t);
    (*offset)++;
}

/**
 * @brief Get the Next Address
 * 
 * @param blk 
 * @return int 
 */
int getNextAddress(const unsigned char *blk)
{
    char addr[5] = "";
    for (int i = 0; i < 4; i++)
    {
        addr[i] = (char) *(blk + 56 + i);
    }
    return atoi(addr);
}

/**
 * @brief 将指定块的下一块编号写为指定编号
 * 
 * @param blk 
 * @param addr 
 */
void writeAddressToBlock(unsigned char *blk, int addr)
{
    char str_addr[9] = "";
    sprintf(str_addr, "%d", addr);
    memcpy(blk + 56, str_addr, 8);
}



/**
 * @brief 将元组写入指定块的指定位置
 * 
 * @param blk 
 * @param offset 
 * @param t 
 */
void writeTupleToBlock(unsigned char *blk, int offset, tuple2d t)
{
    char strx[5] = "";
    sprintf(strx, "%d", t.x);
    memcpy(blk + offset * 8, strx, 4);
    char stry[5] = "";
    sprintf(stry, "%d", t.y);
    memcpy(blk + offset * 8 + 4, stry, 4);
}

/**
 * @brief  写回缓冲区仍有的未写回磁盘的数据
 * 
 * @param blk 
 * @param addr 
 * @param offset 
 */
void writeBufferToBlockAndDisk(unsigned char **blk,int *addr,int *offset, Buffer *buf)
{
    if (*offset != 0)
    {
        writeAddressToBlock(*blk, *addr + 1);
        if (writeBlockToDisk(*blk, *addr, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            exit(-1);
        }
        printf("Output To Disk: %d\n", *addr);
    }
    else
    {
        freeBlockInBuffer(*blk, buf);
    }
}

/**
 * @brief  线性关系搜索选择算法
 * 
 * @param targetVal 搜索的目标值(这里的实现明显是不合理的) 
 * @param targetRel 搜索的目标关系
 * @param targetWay 搜索的是第几个属性(可能用不上)
 * @param buf 
 */
void LinearRelationSelect(int targetVal, int targetRel, Buffer* buf){
    printf("MISSION 1 START!\n");
    size_t init_numIO = buf->numIO;
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
                printf("(X=%d, Y=%d) \n", X, Y);
            }

            }
            freeBlockInBuffer(blk, buf);
    }
    printf("TOTAL IO: %ld\n", buf->numIO - init_numIO);
    }

/**
 * @brief TPMMS Stage 1
 * 
 * @param first_blk 
 * @param last_blk 
 * @param first_temp_blk 
 * @param buf 
 */
void TPMMS_1(int first_blk, int last_blk, int first_temp_blk, Buffer* buf)
{
    int blk_num = last_blk - first_blk + 1;
    // 缓冲区一共8个块，这里每次放6个进去
    int group_num = (int) ceil((double) blk_num / 6);
    int cur_addr = first_blk;
    int cur_temp_addr = first_temp_blk;
    unsigned char *result_blk;
    if ((result_blk = getNewBlockInBuffer(buf)) == NULL)
    {
        perror("Get new Block Failed!\n");
        exit(-1);
    }
    for (int i = 0; i < group_num; i++)
    {
        int blk_cnt = 0;
        unsigned char *blk[6];
        for (; blk_cnt < 6 && cur_addr <= last_blk; cur_addr++, blk_cnt++)
        {
            if ((blk[blk_cnt] = readBlockFromDisk(cur_addr, buf)) == NULL)
            {
                perror("Reading Block Failed!\n");
                exit(-1);
            }
        }
        for (int j = 0; j < blk_cnt * 7 - 1; j++)
        {
            int tuple_addr_j = j / 7;
            int tuple_offset_j = j % 7;
            for (int k = j + 1; k < blk_cnt * 7; k++)
            {
                int tuple_addr_k = k / 7;
                int tuple_offset_k = k % 7;
                tuple2d t1 = readTupleFromBlock(blk[tuple_addr_j], tuple_offset_j);
                tuple2d t2 = readTupleFromBlock(blk[tuple_addr_k], tuple_offset_k);
                if (t1.x > t2.x)
                {
                    writeTupleToBlock(blk[tuple_addr_j], tuple_offset_j, t2);
                    writeTupleToBlock(blk[tuple_addr_k], tuple_offset_k, t1);
                }
            }

        }
        int offset = 0;
        for (int j = 0; j < blk_cnt; j++)
        {
            for (int k = 0; k < 7; k++)
            {
                tuple2d t = readTupleFromBlock(blk[j], k);
                writeTupleToBlockAndDisk(&result_blk, &cur_temp_addr, &offset, t, buf);
            }
        }

        if (offset != 0) {
            writeAddressToBlock(result_blk, cur_temp_addr + 1);
            if (writeBlockToDisk(result_blk, cur_temp_addr, buf) != 0)
            {
                perror("Writing Block Failed!\n");
                exit(-1);
            }
            printf("Output To Disk: %d\n", cur_temp_addr);
            result_blk = getNewBlockInBuffer(buf);
            cur_temp_addr++;
        }
        for (int j = 0; j < blk_cnt; j++)
        {
            freeBlockInBuffer(blk[j], buf);
        }

    }
    freeBlockInBuffer(result_blk, buf);
}

/**
 * @brief TPMMS Stage 2
 * 
 * @param first_temp_blk 
 * @param last_temp_blk 
 * @param first_output_blk 
 * @param last_output_blk 
 * @param buf 
 */
void TPMMS_2(int first_temp_blk, int last_temp_blk, int first_output_blk, int *last_output_blk, Buffer* buf)
{
    int blk_num = last_temp_blk - first_temp_blk + 1;
    int group_num = (int) ceil((double) blk_num / 6);
    int output = first_output_blk;
    int offset = 0;
    unsigned char *result_blk;
    if ((result_blk = getNewBlockInBuffer(buf)) == NULL)
    {
        perror("Get new Block Failed!\n");
        exit(-1);
    }
    unsigned char *m_compare;
    if ((m_compare = getNewBlockInBuffer(buf)) == NULL)
    {
        perror("Get new Block Failed!\n");
        exit(-1);
    }
    // 组内最前的未排序的块
    unsigned char *blk[group_num];
    // 块内最前的未排序的元组
    int tuple_ptr[group_num];
    // 当前在排序的块
    int blk_sorting_ptr[group_num];

    for (int i = 0; i < group_num; i++)
    {
        if ((blk[i] = readBlockFromDisk(first_temp_blk + i * 6, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            exit(-1);
        }
        tuple2d t = readTupleFromBlock(blk[i], 0);
        writeTupleToBlock(m_compare, i, t);
        tuple_ptr[i] = 1;
        blk_sorting_ptr[i] = 0;
    }

    while (1)
    {
        int min = 9999;
        // 最小值所在位置
        int min_index = 0;
        // 子集是否完成排序
        bool group_finish[group_num];

        // 找最小值所在位置
        for (int i = 0; i < group_num; i++)
        {
            tuple2d t = readTupleFromBlock(m_compare, i);
            // 排序结束人为添加标识
            if (t.x == 9999)
            {
                group_finish[i] = true;
            }
            else
            {
                group_finish[i] = false;
            }
            if (t.x < min)
            {
                min = t.x;
                min_index = i;
            }
        }
        // 全部排序是否完成
        bool finish = true;
        for (int i = 0; i < group_num; i++)
        {
            if (group_finish[i] == 0)
            {
                finish = false;
                break;
            }
        }
        // 排序已全部完成
        if (finish)
        {
            writeBufferToBlockAndDisk(&result_blk, &output, &offset, buf);
            *last_output_blk = output;
            freeBlockInBuffer(m_compare, buf);
            for (int i = 0; i < group_num; i++)
            {
                freeBlockInBuffer(blk[i], buf);
            }
            // 删除中间文件
            for (int i = first_temp_blk; i <= last_temp_blk; i++)
            {
                if (dropBlockOnDisk(i) != 0)
                {
                    perror("Dropping Block Fails!\n");
                    exit(-1);
                }
            }
            return;
        }
        // 排序
        tuple2d t = readTupleFromBlock(m_compare, min_index);
        // 将该元组写入已排序结果
        writeTupleToBlockAndDisk(&result_blk, &output, &offset, t, buf);
        // 读入下一个元组
        int blk_index = tuple_ptr[min_index] / 7;
        int tuple_offset = tuple_ptr[min_index] % 7;

        if (blk_index == blk_sorting_ptr[min_index])
        {
            // 当前块还未排完
            tuple2d temp = readTupleFromBlock(blk[min_index], tuple_offset);
            writeTupleToBlock(m_compare, min_index, temp);
            tuple_ptr[min_index]++;
        }
        else
        {
            // 当前块排完，拿下一块
            int next_addr = getNextAddress(blk[min_index]);
            if (blk_index < 6 && next_addr <= last_temp_blk)
            {
                freeBlockInBuffer(blk[min_index], buf);
                // 读入未排的
                if ((blk[min_index] = readBlockFromDisk(first_temp_blk + min_index * 6 + blk_index, buf)) == NULL)
                {
                    perror("Reading Block Failed!\n");
                    exit(-1);
                }
                blk_sorting_ptr[min_index] = blk_index;
                tuple2d temp = readTupleFromBlock(blk[min_index], tuple_offset);
                writeTupleToBlock(m_compare, min_index, temp);
                tuple_ptr[min_index]++;
            } else
            {
                // 排完，写入9999标识
                tuple2d temp = {9999, 9999};
                writeTupleToBlock(m_compare, min_index, temp);
            }
        }
    }
}


/**
 * @brief 两阶段多路归并排序算法
 * 
 * @param first_blk 
 * @param last_blk 
 * @param first_output_blk 
 * @param last_output_blk 
 * @param buf 
 */

void TPMMSort(int first_blk, int last_blk, int first_output_blk, int *last_output_blk, Buffer* buf){
    
    printf("MISSION 2 START!\n");
    size_t input_numIO = buf->numIO;
    if (first_blk == 1)
    {
        printf("CHOOSE RELATION: R\n");
    }
    else
    {
        printf("CHOOSE RELATION: S\n");
    }
    int first_temp_blk = first_output_blk * 10;
    int last_temp_blk = first_temp_blk + last_blk - first_blk;
    // 第一步，划分子集并子集排序
    TPMMS_1(first_blk, last_blk, first_temp_blk, buf);
    // 第二步，各子集间归并排序
    TPMMS_2(first_temp_blk, last_temp_blk, first_output_blk, last_output_blk, buf);
    printf("TOTAL IO: %ld\n", buf->numIO - input_numIO);
    
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

/**
 * @brief 生成索引（取排序后的每一块第一个元组的第一个属性）
 * 
 * @param first_blk 
 * @param last_blk 
 * @param first_index_blk 
 * @param last_index_blk 
 */
void makeIndex(int first_blk, int last_blk, int first_index_blk, int *last_index_blk, Buffer* buf)
{
    printf("MISSION 3 START!\nCREATING INDEX...\n");
    int output = first_index_blk;
    int offset = 0;
    unsigned char *index_blk;
    if ((index_blk = getNewBlockInBuffer(buf)) == NULL)
    {
        perror("Get new Block Failed!\n");
        exit(-1);
    }
    // 遍历已排序的每块中的第一个元组的第一个属性记入索引
    for (int addr = first_blk; addr <= last_blk; addr++)
    {
        unsigned char *blk;
        if ((blk = readBlockFromDisk(addr, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            exit(-1);
        }
        tuple2d index = readTupleFromBlock(blk, 0);
        index.y = addr;
        writeTupleToBlockAndDisk(&index_blk, &output, &offset, index, buf);

        freeBlockInBuffer(blk, buf);
    }
    // 更新最后一块索引位置
    *last_index_blk = output;
    writeBufferToBlockAndDisk(&index_blk, &output, &offset, buf);
}

/**
 * @brief 基于索引的关系选择算法
 * 
 * @param first_index_blk 
 * @param last_index_blk 
 * @param first_output_blk 
 * @param search_data 
 */
void indexSearch(int first_index_blk, int last_index_blk, int first_output_blk,int search_data, Buffer* buf)
{
    // 存io数方便后续计算
    size_t old_numIO = buf->numIO;
    printf("Index Searching\nS.C = %d:\n",search_data);
    // 输出到301块
    int output = first_output_blk;
    int offset = 0;
    int tuple_num = 0;
    // 是否找到索引
    bool index_find = false;
    // 首个读取的块
    int blk_index = 0;
    for (int addr = first_index_blk; addr <= last_index_blk && !index_find; addr++)
    {
        unsigned char *blk;
        if ((blk = readBlockFromDisk(addr, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            exit(-1);
        }
        printf("Reading Indexed Block: %d\n", addr);
        for (int i = 0; i < 7; i++)
        {
            tuple2d index = readTupleFromBlock(blk, i);
            if (index.x >= search_data)
            {
                index_find = true;
                break;
            }
            blk_index = index.y;
        }
        if (!index_find)
        {
            printf("Requested Tuple Not Found!\n");
        }
        freeBlockInBuffer(blk, buf);
    }
    unsigned char *result_blk;
    if ((result_blk = getNewBlockInBuffer(buf)) == NULL)
    {
        perror("Get new Block Failed!\n");
        exit(-1);
    }
    // 是否搜索结束
    bool finish_flag = false;
    // 根据索引遍历块和块内元组
    for (int addr = blk_index; addr <= last_index_blk && !finish_flag; addr++)
    {
        unsigned char *blk;
        if ((blk = readBlockFromDisk(addr, buf)) == NULL)
        {
            perror("Reading Block Failed!\n");
            exit(-1);
        }
        printf("Reading Block: %d\n", addr);
        for (int i = 0; i < 7; i++)
        {
            tuple2d t = readTupleFromBlock(blk, i);
            if (t.x == search_data)
            {
                writeTupleToBlockAndDisk(&result_blk, &output, &offset, t, buf);
                printf("(X=%d, Y=%d)\n", t.x, t.y);
                tuple_num++;
            }
            else if (t.x > search_data)
            {
                finish_flag = true;
                break;
            }
        }
        freeBlockInBuffer(blk, buf);
    }
    writeBufferToBlockAndDisk(&result_blk, &output, &offset, buf);
    printf("TOTAL IO: %ld\n", buf->numIO - old_numIO);
}

/**
 * @brief 连接元组写入块
 * 
 * @param blk 
 * @param offset 
 * @param t1 
 * @param t2 
 */
void writeJoinTtupleToBlock(unsigned char *blk, int offset, tuple2d t1, tuple2d t2)
{
    writeTupleToBlock(blk, offset * 2, t1);
    writeTupleToBlock(blk, offset * 2 + 1, t2);
}

/**
 * @brief 将连接后的元组写入块，写满块写入磁盘
 * 
 * @param blk 
 * @param addr 
 * @param offset 
 * @param t1 
 * @param t2 
 */
void writeJoinTupleToBlockAndDisk(unsigned char **blk, int *addr, int *offset, tuple2d t1, tuple2d t2, Buffer* buf)
{
    if (*offset >= 3)
    {
        writeAddressToBlock(*blk, *addr + 1);
        if (writeBlockToDisk(*blk, *addr, buf) != 0)
        {
            perror("Writing Block Failed!\n");
            exit(-1);
        }
        printf("Output To Disk: %d\n", *addr);
        (*addr)++;
        *blk = getNewBlockInBuffer(buf);
        memset(*blk, 0, 64);
        *offset = 0;
    }
    writeJoinTtupleToBlock(*blk, *offset, t1, t2);
    (*offset)++;
}

/**
 * @brief 基于排序的连接算法
 * 
 * @param R_first_sorted_blk 
 * @param R_last_sorted_blk 
 * @param S_first_sorted_blk 
 * @param S_last_sorted_blk 
 * @param first_output_blk 
 */
void SortMergeJoin(int R_first_sorted_blk, int R_last_sorted_blk, int S_first_sorted_blk, int S_last_sorted_blk, int first_output_blk, Buffer* buf)
{
    printf("MISSION 4 START!\n");
    int join = 0;
    int r_cur_blk = R_first_sorted_blk;
    int s_cur_blk = S_first_sorted_blk;
    int output = first_output_blk;
    int offset = 0;
    unsigned char *result_blk;
    if ((result_blk = getNewBlockInBuffer(buf)) == NULL)
    {
        perror("Get new Block Failed!\n");
        exit(-1);
    }
    memset(result_blk, 0, 64);
    unsigned char *r_blk;
    if ((r_blk = readBlockFromDisk(r_cur_blk, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        exit(-1);
    }
    unsigned char *s_blk;
    if ((s_blk = readBlockFromDisk(s_cur_blk, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        exit(-1);
    }
    int r_cur_offset = 0;
    int s_cur_offset = 0;
    while (r_cur_blk <= R_last_sorted_blk && s_cur_blk <= S_last_sorted_blk)
    {
        tuple2d t_r = readTupleFromBlock(r_blk, r_cur_offset);
        tuple2d t_s = readTupleFromBlock(s_blk, s_cur_offset);
        if (t_r.x < t_s.x)
        {
            r_cur_offset++;
        }
        else if (t_r.x > t_s.x)
        {
            s_cur_offset++;
        }
        else
        {
            int s_temp_blk = s_cur_blk;
            int s_temp_offset = s_cur_offset;
            unsigned char *s_temp;
            if ((s_temp = readBlockFromDisk(s_temp_blk, buf)) == NULL)
            {
                perror("Reading Block Failed!\n");
                exit(-1);
            }
            while (s_temp_blk <= S_last_sorted_blk)
            {
                tuple2d t_s_temp = readTupleFromBlock(s_temp, s_temp_offset);
                if (t_s_temp.x > t_r.x)
                {
                    freeBlockInBuffer(s_temp, buf);
                    break;
                }
                writeJoinTupleToBlockAndDisk(&result_blk, &output, &offset, t_s_temp, t_r, buf);
                join++;
                s_temp_offset++;
                if (s_temp_offset >= 7)
                {
                    freeBlockInBuffer(s_temp, buf);
                    s_temp_blk++;
                    if (s_temp_blk > S_last_sorted_blk)
                    {
                        break;
                    }
                    if ((s_temp = readBlockFromDisk(s_temp_blk, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        exit(-1);
                    }
                    s_temp_offset = 0;
                }
            }
            r_cur_offset++;
        }

        if (r_cur_offset >= 7)
        {
            freeBlockInBuffer(r_blk, buf);
            r_cur_blk++;
            if (r_cur_blk > R_last_sorted_blk)
            {
                break;
            }
            if ((r_blk = readBlockFromDisk(r_cur_blk, buf)) == NULL)
            {
                perror("Reading Block Failed!\n");
                exit(-1);
            }
            r_cur_offset = 0;
        }
        if (s_cur_offset >= 7)
        {
            freeBlockInBuffer(s_blk, buf);
            s_cur_blk++;
            if (s_cur_blk > S_last_sorted_blk)
            {
                break;
            }
            if ((s_blk = readBlockFromDisk(s_cur_blk, buf)) == NULL)
            {
                perror("Reading Block Failed!\n");
                exit(-1);
            }
            s_cur_offset = 0;
        }
    }
    writeBufferToBlockAndDisk(&result_blk, &output, &offset, buf);
    freeBlockInBuffer(r_blk, buf);
    freeBlockInBuffer(s_blk, buf);
    printf("TOTAL: %d TIMES\n", join);
}

/**
 * @brief 基于排序或散列的两趟扫描算法，计算交集
 * 
 * @param R_first_sorted_blk 
 * @param R_last_sorted_blk 
 * @param S_first_sorted_blk 
 * @param S_last_sorted_blk 
 * @param first_output_blk 
 */
void makeIntersection(int R_first_sorted_blk, int R_last_sorted_blk, int S_first_sorted_blk, int S_last_sorted_blk, int first_output_blk,Buffer* buf)
{
    printf("MISSION 5 START!\nGET INTERSECTION\n");
    int intersection = 0;
    int r_cur_blk = R_first_sorted_blk;
    int s_cur_blk = S_first_sorted_blk;
    int output = first_output_blk;
    int offset = 0;
    unsigned char *result_blk;
    if ((result_blk = getNewBlockInBuffer(buf)) == NULL)
    {
        perror("Get new Block Failed!\n");
        exit(-1);
    }
    memset(result_blk, 0, 64);
    unsigned char *r_blk;
    if ((r_blk = readBlockFromDisk(r_cur_blk, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        exit(-1);
    }
    unsigned char *s_blk;
    if ((s_blk = readBlockFromDisk(s_cur_blk, buf)) == NULL)
    {
        perror("Reading Block Failed!\n");
        exit(-1);
    }
    int r_cur_offset = 0;
    int s_cur_offset = 0;
    while (r_cur_blk <= R_last_sorted_blk && s_cur_blk <= S_last_sorted_blk)
    {
        tuple2d t_r = readTupleFromBlock(r_blk, r_cur_offset);
        tuple2d t_s = readTupleFromBlock(s_blk, s_cur_offset);
        if (t_r.x < t_s.x)
        {
            r_cur_offset++;
        }
        else if (t_r.x > t_s.x)
        {
            s_cur_offset++;
        }
        else
        {
            int r_temp_blk = r_cur_blk;
            int r_temp_offset = r_cur_offset;
            unsigned char *r_temp;
            if ((r_temp = readBlockFromDisk(r_temp_blk, buf)) == NULL)
            {
                perror("Reading Block Failed!\n");
                exit(-1);
            }
            while (r_temp_blk <= R_last_sorted_blk)
            {
                tuple2d t_r_temp = readTupleFromBlock(r_temp, r_temp_offset);

                if (t_r_temp.x > t_s.x)
                {
                    freeBlockInBuffer(r_temp, buf);
                    break;
                }

                if (t_r_temp.y == t_s.y)
                {
                    writeTupleToBlockAndDisk(&result_blk, &output, &offset, t_s,buf);
                    printf("(X=%d, Y=%d)\n", t_s.x, t_s.y);
                    intersection++;
                }

                r_temp_offset++;
                if (r_temp_offset >= 7)
                {
                    freeBlockInBuffer(r_temp, buf);
                    r_temp_blk++;
                    if (r_temp_blk > R_last_sorted_blk)
                    {
                        break;
                    }
                    if ((r_temp = readBlockFromDisk(r_temp_blk, buf)) == NULL)
                    {
                        perror("Reading Block Failed!\n");
                        exit(-1);
                    }
                    r_temp_offset = 0;
                }
            }
            s_cur_offset++;
        }
        if (r_cur_offset >= 7)
        {
            freeBlockInBuffer(r_blk, buf);
            r_cur_blk++;
            if (r_cur_blk > R_last_sorted_blk)
            {
                break;
            }
            if ((r_blk = readBlockFromDisk(r_cur_blk, buf)) == NULL)
            {
                perror("Reading Block Failed!\n");
                exit(-1);
            }
            r_cur_offset = 0;
        }
        if (s_cur_offset >= 7)
        {
            freeBlockInBuffer(s_blk, buf);
            s_cur_blk++;
            if (s_cur_blk > S_last_sorted_blk)
            {
                break;
            }
            if ((s_blk = readBlockFromDisk(s_cur_blk, buf)) == NULL)
            {
                perror("Reading Block Failed!\n");
                exit(-1);
            }
            s_cur_offset = 0;
        }
    }
    writeBufferToBlockAndDisk(&result_blk, &output, &offset, buf);
    freeBlockInBuffer(r_blk, buf);
    freeBlockInBuffer(s_blk, buf);
    printf("THERE IS %d TUPLES IN S INTERSECT R\n", intersection);
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
        printf("(X=%d, Y=%d) ", X, Y);
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
        printf("(X=%d, Y=%d) ", X, Y);
    }
    for (int k = 0; k < 4; k++)
    {
        str[k] = *(blk + i*8 + k);
    }
    addr = atoi(str);
    printf("\nnext address = %d \n", addr);
}

void showBuffer(unsigned char* buf_data){
    int X = -1;
    int Y = -1;
    int addr = -1;
    int i=0;
    char str[5];
    for (i = 0; i < 56; i++) //一个blk存7个元组加一个地址
    {

        for (int k = 0; k < 4; k++)
        {
            str[k] = *(buf_data + i*8 + k);
        }
        X = atoi(str);
        for (int k = 0; k < 4; k++)
        {
            str[k] = *(buf_data + i*8 + 4 + k);
        }
        Y = atoi(str);
        printf("(X=%d, Y=%d) ", X, Y);
    }
}