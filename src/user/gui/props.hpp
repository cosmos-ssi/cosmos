//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* propertes
*/
#ifndef _PROPS_HPP
#define _PROPS_HPP

#include <arraylist.hpp>
#include <types.h>

#define PROPS_KEY_SIZE 64
#define PROPS_VALUE_SIZE 64

struct arraylist;

class Prop {
  public:
    uint8_t key[PROPS_KEY_SIZE];
    uint8_t value[PROPS_VALUE_SIZE];
};

class Props {
  private:
    ArrayList<Prop>* props;

  public:
    Props();
    ~Props();
    void load(uint8_t* fs, uint8_t* fn);
    uint8_t* get(uint8_t* name);
};

#endif
