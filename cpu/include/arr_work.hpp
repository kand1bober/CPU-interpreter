#include <assert.h>
#include <unordered_map>
#include <new>

#include "cpu_header.hpp"
#include "stages.hpp"

#ifndef ARR_WORK_HEADER
#define ARR_WORK_HEADER

typedef enum 
{
    kBuilding,
    kNotBuilding,
} BaseBlockState;

struct BaseBlock
{
public:
    size_t sz_;
    Register pc_beg_;
    Register pc_end_;
    std::vector<DecodedResult> instr_arr_;

    BaseBlock(Register curr_pc) :
        pc_beg_(curr_pc),
        pc_end_(0),
        sz_(0)
        {}

    void add_instr(DecodedResult& instr)
    {
        instr_arr_.push_back(instr);
        sz_ = instr_arr_.size();
    }

    void end_block(Register curr_pc)
    {
        pc_end_ = curr_pc;
    }
};

struct BlockTable
{
private:
    int sz_;
    std::unordered_map<Register, BaseBlock*> hash_;

public:
    BlockTable() : sz_(0) {}

    BaseBlock* start_block(Register curr_pc)
    {
        BaseBlock* new_block = new BaseBlock(curr_pc);
        hash_[curr_pc] = new_block;

        return new_block;
    }

    CpuInfo lookup_block(Register curr_pc, BaseBlock** ans)
    {
        auto hit = hash_.find(curr_pc);
        if (hit == hash_.end())
            return kBaseBlockNotFound;
        else 
        {
            *ans = hit->second;
            return kBaseBlockFound;
        }
    }

    ~BlockTable(){
        for (auto& pair : hash_){
            pair.second;
            delete pair.second;
        }
    }
};

void process_cmd(CpuState* cpu_state, Memory* memory, uint32_t curr_cmd, BlockTable& block_table);

#endif
