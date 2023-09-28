// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFObject.h"

class PDFLiteral : public virtual PDFObject {
 public:
    enum class Types {
        kLIT_NA = 0,
        kLIT_END_ARRAY,
        kLIT_END_DIC,
        kLIT_POSITION
    };

 public:
    explicit PDFLiteral(const PDFLiteral::Types type);
    virtual ~PDFLiteral() noexcept;

    std::size_t getLength() const override;
    PDFLiteral::Types getLiteralType() const;

 private:
     const PDFLiteral::Types m_literalType;
};
