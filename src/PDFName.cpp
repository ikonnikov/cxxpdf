// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFName.h"

PDFName::PDFName(const std::string& pdfName) : m_value(pdfName) {
    this->m_type = PDFObject::Types::kNAME;
}

PDFName::~PDFName() {
    // dtor
}

std::size_t PDFName::getLength() const {
    return m_value.size();
}

const std::string PDFName::getName() const {
    return m_value;
}

bool PDFName::equal(const std::string& cmpString) const {
    return 0 == m_value.compare(cmpString);
}
