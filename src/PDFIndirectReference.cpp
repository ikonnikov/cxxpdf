// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFIndirectReference.h"

PDFIndirectReference::PDFIndirectReference() : m_number(0), m_generation() {
    this->m_type = PDFObject::Types::kINDIRECT;
}

PDFIndirectReference::PDFIndirectReference(const std::pair<std::int64_t, std::int64_t>& refPair) : m_number(std::get<0>(refPair)), m_generation(std::get<1>(refPair)) {
    this->m_type = PDFObject::Types::kINDIRECT;
}

PDFIndirectReference::~PDFIndirectReference() {
    // dtor
}

std::size_t PDFIndirectReference::getLength() const {
    return 0;
}
