
CosmOS routes all interrupts via `/interrupts/interrupt_router.h`.

The ISR's in `interrupts/irq.c` all route to `routeInterrupt` which is defined in `/interrupts/interrupt_router.h`

`/interrupts/interrupt_router.h` provides the function `void interrupt_router_register_interrupt_handler(int interruptNumber, interrupt_handler func);
` which devices can use to establish a subscription to an interrupt.

The function prototype `typedef void (*interrupt_handler)(stack_frame *frame);` must be implemented by devices wishing to subscribe to an interrupt.

 