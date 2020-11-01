// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "PDFStream.h"

PDFStream::PDFStream() : m_length(0), m_streamBytes(nullptr) {
    std::cout << __FUNCSIG__ << std::endl;

    this->m_type = PDFObject::Types::kSTREAM;
}

PDFStream::PDFStream(PDFDictionaryPtr& pdfDictionary) : PDFStream() {
    std::cout << __FUNCSIG__ << std::endl;

    std::copy(pdfDictionary->begin(), pdfDictionary->end(), std::inserter(m_value, m_value.end()));
}

PDFStream::~PDFStream() {
    std::cout << __FUNCSIG__ << std::endl;
    
    if (m_streamBytes)
        delete[] m_streamBytes;
}

std::size_t PDFStream::getLength() const {
    return m_length;
}

void PDFStream::setStreamBytes(const char* bytes, std::size_t length) {
    m_streamBytes = new char[length]();
    ::memcpy_s(m_streamBytes, length, bytes, length);

    m_length = length;
}
