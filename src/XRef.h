// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <memory>

#include "Tokeniser.h"
#include "PDFObject.h"
#include "PDFDictionary.h"
#include "PDFName.h"
#include "PDFString.h"
#include "PDFArray.h"
#include "PDFLiteral.h"
#include "PDFIndirectReference.h"

class XRef {
 public:
    XRef() = delete;
    explicit XRef(std::unique_ptr<Tokeniser>& documentTokeniser);
    virtual ~XRef() noexcept;

 private:
    void read_xref();
    bool read_xref_stream();
    bool read_xref_section();

    std::shared_ptr<PDFObject> read_object();
    std::shared_ptr<PDFDictionary> read_dictionary();
    std::shared_ptr<PDFArray> read_array();

 private:
    mutable std::recursive_mutex m_mutex;

    std::unique_ptr<Tokeniser>& m_documentTokeniser;
};
