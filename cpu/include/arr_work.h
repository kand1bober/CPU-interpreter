#include <assert.h>

#include "cpu_header.h"
#include "stages.h"

#ifndef ARR_WORK_HEADER
#define ARR_WORK_HEADER

typedef enum 
{
    kBuilding,
    kNotBuilding,
} BaseBlockState;

typedef struct 
{
    Register pc_beg;
    Register pc_end;
    int sz;
    DecodedResult* instr_arr;
} BaseBlock;

typedef struct
{
    int arr_sz;
    BaseBlock** arr;
} BlockArr;

CpuInfo find_block(Register curr_pc, BlockArr* block_arr, BaseBlock** ans);
BaseBlock* start_block(Register curr_pc, BlockArr* block_arr);
void end_block(Register curr_pc, BaseBlock* block);
void add_instr_to_block(BaseBlock* block, DecodedResult* new_instr);
void free_block_arr(BlockArr* block_arr);

void process_cmd(CpuState* cpu_state, Memory* memory, uint32_t curr_cmd, BlockArr* block_arr);

#endif
