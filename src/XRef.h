// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
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
#include "PDFMutator.hpp"

class XRef {
 public:
    XRef() = delete;
    explicit XRef(TokeniserPtr& documentTokeniser);
    virtual ~XRef() noexcept;

 private:
    void readXRef(Tokeniser& documentTokeniser);
    bool readXRefStream(Tokeniser& documentTokeniser);
    bool readXRefSection(Tokeniser& documentTokeniser);

    PDFObjectPtr readObject(Tokeniser& documentTokeniser);
    PDFDictionaryPtr readDictionary(Tokeniser& documentTokeniser);
    PDFArrayPtr readArray(Tokeniser& documentTokeniser);

 protected:
    mutable std::recursive_mutex m_mutex;
    std::map<std::int64_t, std::tuple<std::int64_t, std::int64_t>> m_xrefTable;
};
