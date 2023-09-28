// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "XRef.hpp"

XRef::XRef(file_tokeniser& streamTokeniser) : m_trailer(nullptr) {
    m_xrefTable.clear();

    readXRef<file_tokeniser>(&streamTokeniser);
}

XRef::XRef(array_tokeniser& streamTokeniser) : m_trailer(nullptr) {
    m_xrefTable.clear();

    readXRef<array_tokeniser>(&streamTokeniser);
}

XRef::~XRef() noexcept {
    m_xrefTable.clear();
}

const PDFObjectPtr XRef::getPdfObject(std::int64_t xrefIndex) const {
    auto& xrefItem = m_xrefTable.at(xrefIndex);

    return xrefItem.pdfObject;  // std::get<2>(xrefItem);
}
