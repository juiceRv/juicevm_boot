#ifndef RV_DEF_FILE_INCLUDE
#define RV_DEF_FILE_INCLUDE
#include "rv_config.h"
typedef struct{
    uint8_t      used;
    uint8_t      irq_v;
    uint64_t     ecode;//addr offset for mtvec value
}rv64_irq_info_t;
typedef struct{
    uint64_t x0_zero; // Hardwired zero
    uint64_t x1_ra; // Return address
    uint64_t x2_sp; // Stack  pointer
    uint64_t x3_gp; // Global pointer
    uint64_t x4_tp; // Thread pointer
    uint64_t x5_t0; // Temporary
    uint64_t x6_t1; // Temporary
    uint64_t x7_t2; // Temporary
    uint64_t x8_s0_fp; // Saved register,frame_pointer
    uint64_t x9_s1; // Saved register
    uint64_t x10_a0; // Funtion argument,return value
    uint64_t x11_a1; // Funtion argument,return value
    uint64_t x12_a2; // Funtion argument
    uint64_t x13_a3; // Funtion argument
    uint64_t x14_a4; // Funtion argument
    uint64_t x15_a5; // Funtion argument
    uint64_t x16_a6; // Funtion argument
    uint64_t x17_a7; // Funtion argument
    uint64_t x18_s2; // Saved register
    uint64_t x19_s3; // Saved register
    uint64_t x20_s4; // Saved register
    uint64_t x21_s5; // Saved register
    uint64_t x22_s6; // Saved register
    uint64_t x23_s7; // Saved register
    uint64_t x24_s8; // Saved register
    uint64_t x25_s9; // Saved register
    uint64_t x26_s10;// Saved register
    uint64_t x27_s11;// Saved register
    uint64_t x28_t3; // Temporary
    uint64_t x29_t4; // Temporary
    uint64_t x30_t5; // Temporary
    uint64_t x31_t6; // Temporary
    uint64_t pc;
}rv_cpu_reg_t;
typedef enum{
    RV_exception_Illegal_Instruction = 1,
    RV_exception_LoadAddress_Misaligned = 2,
    RV_exception_StoreAddress_Misaligned,
    RV_exception_Environment_Call_FromMachine,
    RV_exception_Breakpoint,
    RV_exception_FloatingPoint_Disabled,
    RV_exception_Instruction_page_fault,
    RV_exception_Load_page_fault,
    RV_exception_Store_or_AMO_page_fault,
    RV_exception_Store_or_AMO_access_fault,
    RV_exception_Instruction_access_fault,
    RV_exception_Load_access_fault
}rv_exception_t;
typedef enum{
    rv_privileged_level_User_mode = 0,
    rv_privileged_level_Supervisor_Mode = 1,
    rv_privileged_level_Reserved = 2,
    rv_privileged_level_Machine_mode = 3,
}rv_privileged_level;
typedef struct{
    uint8_t        *mem_p;
    rv_cpu_reg_t    reg;
    rv_exception_t  excpTrigg;
    uint64_t        priv_reg[4096];
    rv_privileged_level priv_mode;
}rv_cpu_t;
extern rv64_irq_info_t* interrupt_request(rv64_irq_info_t *irq_info,rv_privileged_level next_priv_mode);
extern void enter_exception_common(rv64_irq_info_t *irq_info,rv_privileged_level next_priv_mode);

typedef uint32_t (*load_inst_from_mem_t)(uint64_t);
typedef uint8_t (*load_byte_from_mem_t)(uint64_t);
typedef void (*store_byte_to_mem_t)(uint64_t,uint8_t);
typedef uint16_t (*load_half_from_mem_t)(uint64_t);
typedef void (*store_half_to_mem_t)(uint64_t,uint16_t);
typedef uint32_t (*load_word_from_mem_t)(uint64_t);
typedef void (*store_word_to_mem_t)(uint64_t,uint32_t);
typedef void (*check_isexist_irq_need_execu_t)(void);
#ifdef RV64I_SUPPORT_ENBALE
typedef uint64_t (*load_double_from_mem_t)(uint64_t);
typedef void (*store_double_to_mem_t)(uint64_t,uint64_t);
#endif
typedef struct{
    uint64_t addr_min;
    uint64_t addr_max;
    check_isexist_irq_need_execu_t check_isexist_irq_need_execu;
    load_inst_from_mem_t load_inst_from_mem;
    load_byte_from_mem_t load_byte_from_mem;
    store_byte_to_mem_t store_byte_to_mem;
    load_half_from_mem_t load_half_from_mem;
    store_half_to_mem_t store_half_to_mem;
    load_word_from_mem_t load_word_from_mem;
    store_word_to_mem_t store_word_to_mem;
    #ifdef RV64I_SUPPORT_ENBALE
    load_double_from_mem_t load_double_from_mem;
    store_double_to_mem_t store_double_to_mem;
    #endif
}rv_peripheral_device_t;

typedef struct{
    uint8_t  irq_v;//1bit
    uint64_t addr_min;
    uint64_t addr_max;
}rv64_retain_irq_mcause_table_sub_t;



void juicevm_set_cpu_start(void);
void juicevm_set_cpu_stop(void);
uint8_t juicevm_get_cpu_state(void);
typedef enum{
    JUICEVM_CHECK_TYPE_BKP = 0,
    JUICEVM_CHECK_TYPE_WTP = 1
}juicevm_check_t;

typedef enum{
    JUICEVM_MEM_OPTYPE_READ = 0,
    JUICEVM_MEM_OPTYPE_WRITE = 1,
}juicevm_mem_optype_t;
uint64_t juicevm_cpu_check_mem_and_pc(juicevm_check_t check_type,volatile uint64_t pc,volatile uint64_t mem_addr,juicevm_mem_optype_t mem_optype,volatile uint64_t len);
typedef void (*juicevm_checkcb_t)(juicevm_check_t check_type,volatile uint64_t pc,volatile uint64_t mem_addr,juicevm_mem_optype_t mem_optype,volatile uint64_t len);
typedef void (*juicevm_tickcb_t)(void);
rv64_irq_info_t *interrupt_vertor_register(rv64_irq_info_t *irq_info);

#if defined(ESPIDF)
#define global_vm_log_printf(...)  ORIGIN_PRINTF(__VA_ARGS__)
#else
int global_vm_log_printf(const char *fmt, ...);
#endif

#endif // RV_DEF_FILE_INCLUDE