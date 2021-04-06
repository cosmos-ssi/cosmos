//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* this is the init program, which cosmos will read from disk, and execute in user-land context to kick off userland code
*/

#ifndef _INIT_HPP
#define _INIT_HPP

#include <cosmos.hpp>

uint64_t main(uint64_t argc, uint8_t* argv[]);

#endif
