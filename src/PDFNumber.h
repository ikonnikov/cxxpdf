// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFObject.h"

class PDFNumber : public virtual PDFObject {
 public:
     explicit PDFNumber(std::int64_t intValue);
     explicit PDFNumber(double realValue);
     PDFNumber();
     virtual ~PDFNumber() noexcept;

     std::size_t getLength() const override;
     const std::variant<std::int64_t, double> getValue() const;

     std::int64_t asInteger(std::int64_t defaultValue = 0) const;
     double asDouble(double defaultValue = 0.0) const;

 protected:
     std::variant<std::int64_t, double> m_value;
};
