// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFObject.h"
#include "PDFNumber.h"

class PDFArray : public virtual PDFObject {
 public:
    PDFArray();
    virtual ~PDFArray() noexcept;

    std::size_t getLength() const override;
    std::size_t size() const ;
    void add(PDFObjectPtr pdfObject);
    const std::vector<PDFObjectPtr> getVector() const;

    const std::vector<std::int64_t> getIntVector() const;

    PDFObjectPtr& operator[](const std::size_t _Pos) noexcept;
    const PDFObjectPtr& operator[](const std::size_t _Pos) const noexcept;

 protected:
    std::vector<PDFObjectPtr> m_value;
};
