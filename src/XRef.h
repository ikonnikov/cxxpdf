// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <memory>
#include <utility>

#include "Tokeniser.h"
#include "PDFObject.h"
#include "PDFDictionary.h"
#include "PDFName.h"
#include "PDFString.h"
#include "PDFArray.h"
#include "PDFLiteral.h"
#include "PDFIndirectReference.h"
#include "PDFStream.h"
#include "PDFNumber.h"

class XRef {
 public:
    XRef() = delete;
    explicit XRef(TokeniserPtr& documentTokeniser);
    virtual ~XRef() noexcept;

 private:
    void read_xref(TokeniserPtr& documentTokeniser);
    bool read_xref_stream(TokeniserPtr& documentTokeniser);
    bool read_xref_section(TokeniserPtr& documentTokeniser);

    PDFObjectPtr read_object(TokeniserPtr& documentTokeniser);
    PDFDictionaryPtr read_dictionary(TokeniserPtr& documentTokeniser);
    PDFArrayPtr read_array(TokeniserPtr& documentTokeniser);

 private:
    mutable std::recursive_mutex m_mutex;
};
