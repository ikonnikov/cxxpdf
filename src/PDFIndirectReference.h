// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"
#include "PDFObject.h"

class PDFIndirectReference : public virtual PDFObject {
 public:
     PDFIndirectReference();
     explicit PDFIndirectReference(const std::pair<std::int64_t, std::int64_t>& refPair);
     virtual ~PDFIndirectReference() noexcept;

 public:
     std::size_t getLength() const override;
     std::size_t getNumber() const;
     std::size_t getGeneration() const;

 protected:
     std::int64_t m_number;
     std::int64_t m_generation;
};
