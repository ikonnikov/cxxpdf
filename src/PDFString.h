// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <string>

#include "PDFObject.h"

class PDFString : public virtual PDFObject {
 public:
    explicit PDFString(const std::string& pdfString);
    virtual ~PDFString() noexcept;

    virtual size_t getLength() const override;

 private:
    const std::string m_value;
};
