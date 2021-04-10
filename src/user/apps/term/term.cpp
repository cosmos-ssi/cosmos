//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <term.hpp>

Term::Term() {
    this->serialDevice = 0;
}

uint16_t Term::readline(uint8_t* line, uint16_t len) {
    uint8_t CR = 13;
    uint16_t idx = 0;
    while (idx < len) {
        uint8_t c = this->serialDevice->readChar();
        if (c == CR) {
            line[idx] = 0;
            return idx;
        } else {
            line[idx] = c;
            idx = idx + 1;
        }
    }
    line[idx] = 0;
    return idx;
}

uint8_t Term::processline(uint8_t* line) {
    return 0;
}

void Term::lineloop() {
    uint16_t buffer_len = 1024;

    uint8_t go = 1;
    while (go) {
        uint8_t buffer[buffer_len];
        readline(buffer, buffer_len);
        processline(buffer);
    }
}

void Term::run() {
    if (0 == this->serialDevice) {
        this->serialDevice = (SerialObject*)Object::find("serial0");
    }
    if (0 != this->serialDevice) {
        lineloop();
    }
}

uint64_t main(uint64_t argc, uint8_t* argv[]) {
    //   Term* term = new Term();
    // term->run();
    return 0;
}
