// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <vector>
#include <iostream>  // todo: delete it

#include "PDFObject.h"

class PDFArray : public virtual PDFObject {
 public:
    //explicit PDFArray();
    PDFArray();
    virtual ~PDFArray() noexcept;

    virtual size_t getLength() const override;
    void add(std::shared_ptr<PDFObject>);

 private:
    std::vector<std::shared_ptr<PDFObject>> m_value;
};
