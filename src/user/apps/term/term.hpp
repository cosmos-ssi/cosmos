//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

/*
* userland console terminal
*/

#ifndef _TERM_HPP
#define _TERM_HPP

#include <cosmos.hpp>

class Term {
  private:
    SerialObject* serialDevice;
    void lineloop();
    uint16_t readline(uint8_t* line, uint16_t len);

    // return 0 to indicate end of terminal process
    uint8_t processline(uint8_t* line);

  public:
    Term();
    void run();
};

int main(int argc, char* argv[]);

#endif
