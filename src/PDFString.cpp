// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFString.h"

#include <iostream>  // todo: delete it

PDFString::PDFString(const std::string& pdfString) : m_value(pdfString) {
    std::cout << __FUNCSIG__ << " = " << pdfString << std::endl;

    this->m_type = PDFObject::Types::kSTRING;
}

PDFString::~PDFString() {
    std::cout << __FUNCSIG__ << std::endl;
}

size_t PDFString::getLength() const {
    return m_value.size();
}
