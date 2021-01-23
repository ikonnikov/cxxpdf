// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFObject.h"

PDFObject::PDFObject() : m_type(PDFObject::Types::kNA) {
    //
}

PDFObject::~PDFObject() {
    // dtor
}

PDFObject::Types PDFObject::getType() const {
    return m_type;
}

bool PDFObject::isDictionary() const {
    return getType() == PDFObject::Types::kDICTIONARY;
}

bool PDFObject::isLiteral() const {
    return getType() == PDFObject::Types::kLITERAL;
}

bool PDFObject::isInteger() const {
    return getType() == PDFObject::Types::kNUMBER_INT;
}

bool PDFObject::isReal() const {
    return getType() == PDFObject::Types::kNUMBER_REAL;
}

bool PDFObject::isBoolean() const {
    return getType() == PDFObject::Types::kBOOLEAN;
}

bool PDFObject::isString() const {
    return getType() == PDFObject::Types::kSTRING;
}

bool PDFObject::isArray() const {
    return getType() == PDFObject::Types::kARRAY;
}

bool PDFObject::isStream() const {
    return getType() == PDFObject::Types::kSTREAM;
}

bool PDFObject::isName() const {
    return getType() == PDFObject::Types::kNAME;
}
bool PDFObject::isNull() const {
    return getType() == PDFObject::Types::kNULL;
}
bool PDFObject::isIndirect() const {
    return getType() == PDFObject::Types::kINDIRECT;
}
