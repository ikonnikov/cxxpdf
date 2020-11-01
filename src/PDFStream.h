// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <iostream>  // todo: delete it
#include <string.h>

#include "PDFDictionary.h"

class PDFStream;
using PDFStreamPtr = std::shared_ptr<PDFStream>;

class PDFStream : public virtual PDFDictionary {
 public:
    PDFStream();
    PDFStream(PDFDictionaryPtr& pdfDictionary);
    virtual ~PDFStream() noexcept;

    std::size_t getLength() const override;
    void setStreamBytes(const char* bytes, std::size_t length);

 protected:
     std::size_t m_length;
     char* m_streamBytes;
};
