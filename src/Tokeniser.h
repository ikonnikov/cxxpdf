// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include <fstream>
#include <any>
#include <utility>
#include <string>
#include <iostream>  // todo: delete it

constexpr int64_t kDEFAULT_BUF_SIZE = 1024;  // size of buffer

// '1' = character is white space (white-space characters, table 3.1 => 0(NULL), 9(TAB), 10(LF), 12(FF), 13(CR), 32(SPACE))
// '2' = special delimeters, character ends a name or command => 40('('), 41(')'), 60('<'), 62('>'), 91('['), 93(']'), 123('{'), 125('}'), 47('/'), 37('%')
static const char specialChars[256] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,  // 0x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1x
    1, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2,  // 2x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0,  // 3x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0,  // 5x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0,  // 7x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9x
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // ax
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // bx
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // cx
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // dx
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // ex
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // fx
};

class Tokeniser {
 public:
    enum class Types {
        kTK_NA = 0,
        kTK_NUMBER_INT,
        kTK_NUMBER_REAL,
        kTK_STRING,
        kTK_NAME,
        kTK_COMMENT,
        kTK_START_ARRAY,
        kTK_END_ARRAY,
        kTK_START_DIC,
        kTK_END_DIC,
        kTK_REF,
        kTK_OTHER,
        kTK_ENDOFFILE
    };

 public:
    Tokeniser() = delete;
    explicit Tokeniser(std::ifstream& pdfFileStream, std::streamoff startPos = 0);
    virtual ~Tokeniser() noexcept;

    void seek(std::streamoff pos) const;
    std::streamoff tell() const;
    void backOnePosition() const;

    std::string getDocumentHeader(int* major, int* minor) const;
    std::streamoff getStartXref(bool andLocate = true) const;

    std::pair<std::any, Tokeniser::Types> nextToken() const;
    std::pair<std::any, Tokeniser::Types> nextValidToken() const;

    static bool isWhitespace(int c);
    static int getHex(int v);

 private:
    std::ifstream& m_pdfFileStream;
    std::streamoff m_endStreamPos;

    const char* m_pdfVersionToken;
};
