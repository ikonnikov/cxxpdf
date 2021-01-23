// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <memory>
#include <utility>

#include "PDFObject.h"

class PDFIndirectReference;
using PDFIndirectReferencePtr = std::shared_ptr<PDFIndirectReference>;

class PDFIndirectReference : public virtual PDFObject {
 public:
     PDFIndirectReference();
     explicit PDFIndirectReference(const std::pair<std::int64_t, std::int64_t>& refPair);
     virtual ~PDFIndirectReference() noexcept;

     std::size_t getLength() const override;

 protected:
     std::int64_t m_number;
     std::int64_t m_generation;
};
