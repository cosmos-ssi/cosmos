//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020 Tom Everett                                 *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <dev/rtc/cmos.h>
#include <asm/asm.h>

void cmos_write_register(uint8_t reg, uint8_t val){
	uint8_t pv;
	
	asm_cli();
	asm_out_b(CMOS_REGISTER_SELECT_PORT, 0x80 | reg);	// disable NMI so we don't screw up the CMOS RTC irreparably
	pv = asm_in_b(CMOS_REGISTER_DATA_PORT);
	asm_out_b(CMOS_REGISTER_SELECT_PORT, 0x80 | reg);
	asm_out_b(CMOS_REGISTER_DATA_PORT, pv | val);
	
	asm_out_b(CMOS_REGISTER_SELECT_PORT, asm_in_b(CMOS_REGISTER_SELECT_PORT) & 0x7F);	// re-enable nmi
	asm_sti();
	
	return;
}

uint8_t cmos_read_register(uint8_t reg){
	uint8_t b;
	
	asm_cli();
	asm_out_b(CMOS_REGISTER_SELECT_PORT, reg);
	b = asm_in_b(CMOS_REGISTER_DATA_PORT);
	asm_sti();
	
	return b;
}