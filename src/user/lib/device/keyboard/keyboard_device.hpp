//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#ifndef _KEYBOARD_DEVICE_HPP
#define _KEYBOARD_DEVICE_HPP

#include <device/device.hpp>
#include <types.h>

/*
 * press and release state
 */
typedef enum keypress_state { KEYPRESS_MAKE, KEYPRESS_BREAK } keypress_state;

/**
 * key action
 */
struct key_action_t {
    uint8_t key;  // encoded row-col position on abstract keyboard
    keypress_state state;
};

class KeyboardDevice : public Device {
  public:
    void read(struct key_action* action);
};

#endif