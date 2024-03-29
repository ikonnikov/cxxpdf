﻿// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFObject.h"
#include "PDFName.h"

using MapDict = std::map<const PDFNamePtr, const PDFObjectPtr, PDFName::cmpByStringName>;

class PDFDictionary : public virtual PDFObject {
 public:
    PDFDictionary();
    virtual ~PDFDictionary() noexcept;

    std::size_t getLength() const override;
    std::size_t getSize() const;
    void put(const PDFNamePtr& pdfName, const PDFObjectPtr pdfObject);
    const PDFObjectPtr get(const PDFNamePtr& pdfName) const;
    const PDFObjectPtr get(const std::string& pdfName) const;
    const MapDict::const_iterator begin() const;
    const MapDict::const_iterator end() const;

 protected:
     MapDict m_value;
};
