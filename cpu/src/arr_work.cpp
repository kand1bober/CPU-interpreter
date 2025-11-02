#include "../include/arr_work.hpp"

//---------------- Array of blocks Functions --------------

// void free_block_arr(BlockArr* block_arr)
// {
//     assert(block_arr);

//     for (int i = 0; i < block_arr->arr_sz; i++)
//     {
//         free(block_arr->arr[i]->instr_arr);
//         free(block_arr->arr[i]);
//     }

//     free(block_arr->arr);
// }

//---------------------- Block Functions ------------------

// CpuInfo find_block(Register curr_pc, BlockArr* block_arr, BaseBlock** ans)
// {
//     assert(block_arr);
//     assert(ans);

//     for (int i = 0; i < block_arr->arr_sz; i++)
//     {
//         if (block_arr->arr[i]->pc_beg == curr_pc)
//         {
//             *ans = block_arr->arr[i];
//             return kBaseBlockFound;
//         }
//     }

//     *ans = NULL;
//     return kBaseBlockNotFound;
// }


// BaseBlock* start_block(Register curr_pc, BlockArr* block_arr)
// {
//     assert(block_arr);

//     BaseBlock* new_block = (BaseBlock*)malloc(sizeof(BaseBlock));
//     if (new_block == NULL)
//     {
//         perror("malloc");
//         exit(1);
//     }
//     new_block->sz = 0;
//     new_block->pc_beg = curr_pc; 
//     new_block->instr_arr = NULL;

//     block_arr->arr = (BaseBlock**)realloc(block_arr->arr, sizeof(BaseBlock*) * (block_arr->arr_sz + 1));
//     if (block_arr->arr == NULL)
//     {
//         perror("realloc");
//         exit(1);
//     }

//     block_arr->arr_sz += 1;
//     block_arr->arr[block_arr->arr_sz - 1] = new_block;    

//     return new_block;
// }


// void end_block(Register curr_pc, BaseBlock* block)
// {
//     assert(block);

//     block->pc_end = curr_pc;
// }


// void add_instr_to_block(BaseBlock* block, DecodedResult* new_instr)
// {
//     assert(block);
//     assert(new_instr);
    
//     block->instr_arr = (DecodedResult*)realloc(block->instr_arr, sizeof(DecodedResult) * (block->sz + 1));
//     if (block->instr_arr == NULL)
//     {
//         perror("realloc");
//         exit(1);
//     }
//     block->sz += 1;

//     block->instr_arr[block->sz - 1] = *new_instr;
// }
