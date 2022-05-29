/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2022 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _API_INTERFACES_H
#define _API_INTERFACES_H

typedef enum syscap_interfaces_t {
    /*
     * INTERFACE_NULL should always be 0 and INTERFACE_GENERIC should always be
     * 1; after that, list interfaces in alphabetical order and let them be
     * auto-numbered.  INTERFACE_MAX should always be last.
     */
    API_INTERFACE_NULL = 0,
    API_INTERFACE_GENERIC = 1,
    API_INTERFACE_ENUMERABLE,
    API_INTERFACE_BLOCK_CONSUMER,
    API_INTERFACE_BLOCK_PRODUCER,
    API_INTERFACE_CHAR_CONSUMER,
    API_INTERFACE_SEEKABLE,
    API_INTERFACE_MAX
} syscap_interface_t;

typedef void* implemented_interfaces_list[API_INTERFACE_MAX + 1];

#if 0
#define BEGIN_INTERFACE_LIST(driver)                                                                                   \
    implemented_interfaces_list driver_driver_interfaces = {NULL};                                                     \
    void driver_initialize_interfaces() {

#define EXPORTED_INTERFACE(driver, interface)
#endif

#endif