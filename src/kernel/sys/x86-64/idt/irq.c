/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#include <obj/x86-64/pic/pic.h>
#include <sys/interrupt_router/interrupt_router.h>
#include <sys/x86-64/idt/irq.h>

void irq0_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(0, frame);
    pic_send_eoi(0);
}

void irq1_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(1, frame);
    pic_send_eoi(1);
}

void irq2_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(2, frame);
    pic_send_eoi(2);
}

void irq3_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(3, frame);
    pic_send_eoi(3);
}

void irq4_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(4, frame);
    pic_send_eoi(4);
}

void irq5_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(5, frame);
    pic_send_eoi(5);
}

void irq6_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(6, frame);
    pic_send_eoi(6);
}

void irq7_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(7, frame);
    pic_send_eoi(7);
}

void irq8_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(8, frame);
    pic_send_eoi(8);
}

void irq9_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(9, frame);
    pic_send_eoi(9);
}

void irq10_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(10, frame);
    pic_send_eoi(10);
}

void irq11_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(11, frame);
    pic_send_eoi(11);
}

void irq12_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(12, frame);
    pic_send_eoi(12);
}

void irq13_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(13, frame);
    pic_send_eoi(13);
}

void irq14_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(14, frame);
    pic_send_eoi(14);
}

void irq15_handler(stack_frame* frame) {
    interrupt_router_route_interrupt(15, frame);
    pic_send_eoi(15);
}
