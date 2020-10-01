// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFDictionary.h"

PDFDictionary::PDFDictionary() {
    this->m_type = PDFObject::Types::kDICTIONARY;
}

PDFDictionary::~PDFDictionary() noexcept  {
    //
}

size_t PDFDictionary::getLength() const {
    return m_value.size();
}

void PDFDictionary::put(const std::shared_ptr<PDFName> ppdfName, const std::shared_ptr<PDFObject> pdfObject) {
    m_value.insert(std::make_pair(ppdfName, pdfObject));
}
