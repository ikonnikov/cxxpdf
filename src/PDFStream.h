// Copyright (c) 2020-2021 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFDictionary.h"
#include "PDFNumber.h"
#include "StreamDecoder.h"
#include "PDFMutator.hpp"

class PDFStream : public virtual PDFDictionary {
 public:
    PDFStream();
    explicit PDFStream(const PDFDictionaryPtr& pdfDictionary);
    virtual ~PDFStream() noexcept;

    std::size_t getLength() const override;
    void setRawStreamBytes(const char* bytes, std::size_t length);
    std::vector<char> decodeFilteredStream() const;

 protected:
     std::size_t m_length;
     char* m_streamData;
};
