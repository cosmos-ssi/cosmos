/*****************************************************************
 * This file is part of CosmOS                                   *
 * Copyright (C) 2020 Kurt M. Weber                              *
 * Released under the stated terms in the file LICENSE           *
 * See the file "LICENSE" in the source distribution for details *
 *****************************************************************/

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <obj/x86-64/keyboard/abstract_keyboard.h>

void keyboard_objectmgr_register_objects();

void keyboard_remove_command_queue();
void keyboard_send_command_queue();

#endif
