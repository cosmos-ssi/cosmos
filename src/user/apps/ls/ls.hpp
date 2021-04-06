//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* ls
*/

#ifndef _LS_HPP
#define _LS_HPP

#include <cosmos.hpp>

class ls {
  private:
  public:
    ls();
    void run();
};

uint64_t main(uint64_t argc, uint8_t* argv[]);

#endif
