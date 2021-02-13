ENTRY(CosmOS)
SECTIONS
{
	.text 0xFFFF800000000000 :
	{
		_text_start = .;
		*(.text)
		*(.rodata*)
		_text_end = .;
		. = ALIGN(4096);
	}
	.data :
	{
		_data_start = .;
		KEEP(*(.rawdata*))
		_data_end = .;
		. = ALIGN(4096);
	}
	.bss :
	{
		_bss_start = .;
		*(.bss)
		*(COMMON)
		_ebss = .;
		_bss_end = .;
		. = ALIGN(4096);
	}
	_end = .;
}