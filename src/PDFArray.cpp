// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFArray.h"

PDFArray::PDFArray() {
    std::cout << __FUNCSIG__ << std::endl;

    this->m_type = PDFObject::Types::kARRAY;
}

PDFArray::~PDFArray() {
    std::cout << __FUNCSIG__ << std::endl;
}

size_t PDFArray::getLength() const {
    return m_value.size();
}

void PDFArray::add(const std::shared_ptr<PDFObject> pdfObject) {
    m_value.push_back(pdfObject);
}
