// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <string>
#include <iostream>  // todo: delete it

#include "PDFObject.h"

class PDFName : public virtual PDFObject {
 public:
     explicit PDFName(const std::string& pdfName);
     virtual ~PDFName() noexcept;
     
     virtual size_t getLength() const override;
     const std::string getName() const;

 public:
     bool operator<(const PDFName& other) const;

 private:
     const std::string m_value;
};
