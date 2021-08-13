#include <printf.h>
#include <stdint.h>
#include <rv_mtvec_map.h>

void main(void){
    // *(uint64_t *)(pdev_mtime_mtimecmp_addr+1) = 300;
    uint64_t i;
    printf("%s : %d\n",__FUNCTION__,__LINE__);
    while(1){
        i = 1000;
        while(i--);
        // printf("wait for cb\n");
    }
}
void execption_handle(uint64_t mcause){
    printf_("execption_handle %x\n",mcause);
    if(mcause & 0x7){
        printf("mtime cb\n");
    }
}

void _putchar(char character){
    *(char *)(pdev_uart0_write_addr) = character;
}