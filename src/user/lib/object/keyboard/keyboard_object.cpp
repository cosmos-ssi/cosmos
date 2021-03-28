//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

extern "C" {
#include <abi/abi.h>
#include <assert.hpp>
}
#include <object/keyboard/keyboard_object.hpp>

void KeyboardObject::read(struct key_action* action) {
    ASSERT_NOT_NULL(action);
}
