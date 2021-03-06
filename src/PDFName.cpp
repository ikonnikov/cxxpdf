// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFName.h"

PDFName::PDFName(const std::string& pdfName) : m_value(pdfName) {
    std::cout << __FUNCSIG__ << " = " << m_value << std::endl;

    this->m_type = PDFObject::Types::kNAME;
}

PDFName::~PDFName() {
    std::cout << __FUNCSIG__ << " = " << m_value << std::endl;
}

size_t PDFName::getLength() const {
    return m_value.size();
}

const std::string PDFName::getName() const {
    return m_value;
}

bool PDFName::operator<(const PDFName& other) const {
    return m_value < other.getName();
}
