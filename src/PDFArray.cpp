// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFArray.h"

PDFArray::PDFArray() {
    this->m_type = PDFObject::Types::kARRAY;
}

PDFArray::~PDFArray() {
    // dtor
}

std::size_t PDFArray::getLength() const {
    return size();
}

std::size_t PDFArray::size() const {
    return m_value.size();
}

void PDFArray::add(const PDFObjectPtr pdfObject) {
    m_value.push_back(pdfObject);
}

const std::vector<PDFObjectPtr> PDFArray::getVector() const {
    return m_value;
}

PDFObjectPtr& PDFArray::operator[](const std::size_t _Pos) noexcept {
    return m_value[_Pos];
}

const PDFObjectPtr& PDFArray::operator[](const std::size_t _Pos) const noexcept {
    return m_value[_Pos];
}

const std::vector<std::int64_t> PDFArray::getIntVector() const {
    std::vector<std::int64_t> intVector;

    for (PDFObjectPtr value : m_value) {
        if (!value->isInteger())
            continue;

        intVector.push_back(std::dynamic_pointer_cast<PDFNumber>(value)->asInteger(0));
    }

    return intVector;
}
