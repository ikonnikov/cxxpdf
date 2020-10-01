// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFIndirectReference.h"

PDFIndirectReference::PDFIndirectReference() : m_number(0), m_generation(){
    std::cout << __FUNCSIG__ << std::endl;

    this->m_type = PDFObject::Types::kINDIRECT;
}

PDFIndirectReference::PDFIndirectReference(const std::pair<int64_t, int64_t>& refPair) : m_number(std::get<0>(refPair)), m_generation(std::get<1>(refPair)) {
    std::cout << __FUNCSIG__ << std::endl;

    this->m_type = PDFObject::Types::kINDIRECT;
}

PDFIndirectReference::~PDFIndirectReference() {
    std::cout << __FUNCSIG__ << std::endl;
}

size_t PDFIndirectReference::getLength() const {
    return 0;
}
