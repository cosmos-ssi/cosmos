/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <sys/interrupt_router/interrupt_router.h>
#include <sys/x86-64/idt/irq.h>
#include <sys/x86-64/interrupts/apic.h>

void irq0_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(0, frame);
    apic_send_eoi();
}

void irq1_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(1, frame);
    apic_send_eoi();
}

void irq2_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(2, frame);
    apic_send_eoi();
}

void irq3_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(3, frame);
    apic_send_eoi();
}

void irq4_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(4, frame);
    apic_send_eoi();
}

void irq5_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(5, frame);
    apic_send_eoi();
}

void irq6_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(6, frame);
    apic_send_eoi();
}

void irq7_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(7, frame);
    apic_send_eoi();
}

void irq8_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(8, frame);
    apic_send_eoi();
}

void irq9_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(9, frame);
    apic_send_eoi();
}

void irq10_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(10, frame);
    apic_send_eoi();
}

void irq11_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(11, frame);
    apic_send_eoi();
}

void irq12_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(12, frame);
    apic_send_eoi();
}

void irq13_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(13, frame);
    apic_send_eoi();
}

void irq14_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(14, frame);
    apic_send_eoi();
}

void irq15_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(15, frame);
    apic_send_eoi();
}
