// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <iostream>  // todo: delete it

#include "PDFObject.h"

class PDFIndirectReference : public virtual PDFObject {
public:
    PDFIndirectReference();
    explicit PDFIndirectReference(const std::pair<int64_t, int64_t>& refPair);
    virtual ~PDFIndirectReference() noexcept;

    virtual size_t getLength() const override;

private:
    int64_t m_number;
    int64_t m_generation;
};
