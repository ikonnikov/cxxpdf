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

class XRef {
 public:
    XRef() = delete;
    explicit XRef(TokeniserPtr& documentTokeniser);
    virtual ~XRef() noexcept;

 private:
    void readXref(TokeniserPtr& documentTokeniser);
    bool readXrefStream(TokeniserPtr& documentTokeniser);
    bool readXrefSection(TokeniserPtr& documentTokeniser);

    PDFObjectPtr readObject(TokeniserPtr& documentTokeniser);
    PDFDictionaryPtr readDictionary(TokeniserPtr& documentTokeniser);
    PDFArrayPtr readArray(TokeniserPtr& documentTokeniser);

 protected:
    mutable std::recursive_mutex m_mutex;
    std::vector<int> m_xrefTable;
};
