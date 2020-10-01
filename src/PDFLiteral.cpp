// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFLiteral.h"

PDFLiteral::PDFLiteral(const PDFLiteral::Types type) : m_literalType(type) {
    std::cout << __FUNCSIG__ << std::endl;

    this->m_type = PDFObject::Types::kLITERAL;
}

PDFLiteral::~PDFLiteral() {
    std::cout << __FUNCSIG__ << std::endl;
}

size_t PDFLiteral::getLength() const {
    return 0;
}

PDFLiteral::Types PDFLiteral::getLiteralType() const {
    return m_literalType;
}