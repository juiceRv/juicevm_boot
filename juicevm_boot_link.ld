OUTPUT_ARCH(riscv)
ENTRY(_start)
MEMORY
{
	ram  : org = 0x80000000, len = 8 * 1024 * 1024
}

SECTIONS
{
	.text :
	{
		*(.init)
		*(.text*)
		. = ALIGN(8);
	} > ram

	.data ALIGN(16) :
	{
		*(.data*)
    	*(.rodata*)
		. = ALIGN(16);
	} > ram

	PROVIDE(__bss_start = .);
	.bss ALIGN(16) :
	{
		*(.bss*)
		. = ALIGN(16);
	} > ram
	PROVIDE(__bss_end = .);
	. = ALIGN(8);
  PROVIDE(__stack_end = 0x80000000 + 6 * 1024 * 1024);

}
