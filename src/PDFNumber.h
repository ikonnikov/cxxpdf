// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <variant>
#include <iostream>  // todo: delete it

#include "PDFObject.h"

class PDFNumber;
using PDFNumberPtr = std::shared_ptr<PDFNumber>;

class PDFNumber : public virtual PDFObject {
 public:
     PDFNumber(std::int64_t intValue);
     PDFNumber(double realValue);
     virtual ~PDFNumber() noexcept;

     std::size_t getLength() const override;
     const std::variant<std::int64_t, double> getValue() const;

 protected:
     std::variant<std::int64_t, double> m_value;
};
