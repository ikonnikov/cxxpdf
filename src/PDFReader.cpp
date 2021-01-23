// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFReader.h"

PDFReader::PDFReader(const std::string& filename) : m_password(""), m_doc(nullptr), m_filePath(filename) {
    load_from_file();
}

PDFReader::~PDFReader() noexcept {
    // dtor
}

void PDFReader::load_from_file() {
    m_doc = std::make_shared<PDFDoc>(m_filePath);
}

const std::shared_ptr<PDFDoc> PDFReader::getPDFDocument() {
    return m_doc;
}
