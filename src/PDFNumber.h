// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <variant>
#include <iostream>  // todo: delete it

#include "PDFObject.h"

class PDFNumber : public virtual PDFObject {
 public:
     PDFNumber(int64_t intValue);
     PDFNumber(double realValue);
     virtual ~PDFNumber() noexcept;

     virtual size_t getLength() const override;

 private:
     std::variant<int64_t, double> m_value;
};
