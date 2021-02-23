// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFDictionary.h"

PDFDictionary::PDFDictionary() {
    this->m_type = PDFObject::Types::kDICTIONARY;
}

PDFDictionary::~PDFDictionary() noexcept  {
    // dtor
}

std::size_t PDFDictionary::getLength() const {
    return 0;
}

std::size_t PDFDictionary::getSize() const {
    return m_value.size();
}

void PDFDictionary::put(const PDFNamePtr& pdfName, const PDFObjectPtr pdfObject) {
    auto [iter, success] = m_value.insert({pdfName, pdfObject});

    auto iter1 = m_value.begin();


    if (!success)
        throw std::exception("Failed to insert a new value in dictionary");
}

const PDFObjectPtr PDFDictionary::get(const PDFNamePtr& pdfName) const {
    auto iter = m_value.find(pdfName);

    if (iter != m_value.end())
        return iter->second;

    return nullptr;
}

const PDFObjectPtr PDFDictionary::get(const std::string& pdfName) const {
    return get(std::make_shared<PDFName>(pdfName));
}

const MapDict::const_iterator PDFDictionary::begin() const {
    return m_value.begin();
}

const MapDict::const_iterator PDFDictionary::end() const {
    return m_value.end();
}
