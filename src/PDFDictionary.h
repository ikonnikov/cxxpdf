// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <map>
#include <utility>

#include "PDFObject.h"
#include "PDFName.h"

class PDFDictionary : public virtual PDFObject {
 public:
    PDFDictionary();
    virtual ~PDFDictionary() noexcept;

    virtual size_t getLength() const override;
    void put(const std::shared_ptr<PDFName> ppdfName, const std::shared_ptr<PDFObject> pdfObject);

 protected:
     std::map<const std::shared_ptr<PDFName>, const std::shared_ptr<PDFObject>> m_value;
};
