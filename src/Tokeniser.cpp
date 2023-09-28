// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "Tokeniser.hpp"

bool tokeniser_base::isWhitespace(int ch) {
    return (ch == 0 || ch == 9 || ch == 10 || ch == 12 || ch == 13 || ch == 32);
}

int tokeniser_base::getHex(int v) {
    int retValue = 0;

    if (v >= '0' && v <= '9')
        retValue = v - '0';
    else if (v >= 'A' && v <= 'F')
        retValue = v - 'A' + 10;
    else if (v >= 'a' && v <= 'f')
        retValue = v - 'a' + 10;
    else
        retValue = -1;

    return retValue;
}
