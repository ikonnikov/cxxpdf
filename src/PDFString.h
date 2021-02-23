// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFObject.h"

class PDFString : public virtual PDFObject {
 public:
    explicit PDFString(const std::string& pdfString, bool isHex = false);
    virtual ~PDFString() noexcept;

    std::size_t getLength() const override;
    bool isHex() const;

 protected:
    const std::string m_value;
    const bool m_isHex;
};
