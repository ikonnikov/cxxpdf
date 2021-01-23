// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFString.h"

PDFString::PDFString(const std::string& pdfString, bool isHex) : m_value(pdfString), m_isHex(isHex) {
    this->m_type = PDFObject::Types::kSTRING;
}

PDFString::~PDFString() {
    // dtor
}

std::size_t PDFString::getLength() const {
    return m_value.size();
}

bool PDFString::isHex() const {
    return m_isHex;
}
