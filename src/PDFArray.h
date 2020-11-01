// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <vector>
#include <iostream>  // todo: delete it

#include "PDFObject.h"

class PDFArray;
using PDFArrayPtr = std::shared_ptr<PDFArray>;

class PDFArray : public virtual PDFObject {
 public:
    PDFArray();
    virtual ~PDFArray() noexcept;

    std::size_t getLength() const override;
    void add(PDFObjectPtr pdfObject);

 protected:
    std::vector<PDFObjectPtr> m_value;
};
