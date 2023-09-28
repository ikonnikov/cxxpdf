// Copyright (c) 2023 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#pragma once

#include "common.h"

constexpr std::int64_t kDEFAULT_BUF_SIZE(1024);  // size of buffer
constexpr const char* kPDF_VERSION_TOKEN("%PDF-");

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


class tokeniser_base {
 public:
     enum class Types {
         kTK_NA = 0,
         kTK_NUMBER_INT,
         kTK_NUMBER_REAL,
         kTK_STRING,
         kTK_STRING_HEX,
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
     tokeniser_base() {};
     //explicit tokeniser_base(PDFFileStream& pdfStream, std::streamoff startPos = 0);
     virtual ~tokeniser_base() noexcept {};

     //Tokeniser& seek(std::streamoff newPos) = 0;
     //template<class T> T& seek(std::streamoff newPos) = 0;

     //virtual void seek(std::streamoff newPos) const = 0;

     //std::streamoff tell() const;
     //void backOnePosition() const;

     //std::string getDocumentHeader(int* major, int* minor) const;
     //std::streamoff getStartXref(bool andLocate = true) const;

     //std::pair<std::any, Tokeniser::Types> nextToken() const;
     //std::pair<std::any, Tokeniser::Types> nextValidToken() const;

     static bool isWhitespace(int c);
     static int getHex(int v);

     virtual bool readStream(std::string& buffer, std::streamsize buffSize) const = 0;
     //char skipWhitespaces(const char currentChar) const;
     //char skipWhitespaces() const;

 protected:
     //PDFFileStream& m_pdfStream;
     //std::streamoff m_endStreamPos;

};

template<typename Device>
class stream_tokeniser : public virtual tokeniser_base {
 public:
     typedef tokeniser_base::Types Types;

 public:
     stream_tokeniser() = delete;
     explicit stream_tokeniser(boost::iostreams::stream<Device>& pdfStream, std::streamoff startPos = 0);

 public:
     void seek(std::streamoff newPos) const;
     stream_tokeniser<Device>& seek(std::streamoff newPos);
     std::streamoff tell() const;
     void backOnePosition() const;

     std::string getDocumentHeader(int* major, int* minor) const;
     std::streamoff getStartXref(bool andLocate = true) const;

     std::pair<std::any, tokeniser_base::Types> nextToken() const;
     std::pair<std::any, tokeniser_base::Types> nextValidToken() const;

     char skipWhitespaces(const char currentChar) const;
     char skipWhitespaces() const;

     bool readStream(std::string& buffer, std::streamsize buffSize) const;

 protected:
     boost::iostreams::stream<Device>& m_pdfStream;
     std::streamoff m_endStreamPos;
};

typedef stream_tokeniser<boost::iostreams::file_source> file_tokeniser;
typedef stream_tokeniser<boost::iostreams::array_source> array_tokeniser;


template<typename Device>
bool stream_tokeniser<Device>::readStream(std::string& buffer, std::streamsize buffSize) const {
    if (m_pdfStream.read(&buffer[0], buffSize))
        return true;

    return false;
}

template<typename Device>
stream_tokeniser<Device>::stream_tokeniser(boost::iostreams::stream<Device>& pdfStream, std::streamoff startPos) : m_pdfStream(pdfStream), m_endStreamPos(-1) {
    m_pdfStream.seekg(0, std::ios_base::end);
    m_endStreamPos = m_pdfStream.tellg();

    seek(startPos);
}

template<typename Device>
void stream_tokeniser<Device>::seek(std::streamoff newPos) const {
    m_pdfStream.seekg(newPos);
}

template<typename Device>
stream_tokeniser<Device>& stream_tokeniser<Device>::seek(std::streamoff newPos) {
    m_pdfStream.seekg(newPos);

    return *this;
}

template<typename Device>
std::streamoff stream_tokeniser<Device>::tell() const {
    return m_pdfStream.tellg();
}

template<typename Device>
void stream_tokeniser<Device>::backOnePosition() const {
    m_pdfStream.seekg(-1, std::ios_base::cur);
}

template<typename Device>
std::string stream_tokeniser<Device>::getDocumentHeader(int* major, int* minor) const {
    std::streamsize buffSize = kDEFAULT_BUF_SIZE;

    if (buffSize > m_endStreamPos)
        buffSize = m_endStreamPos;

    std::string buffer(buffSize, '\0');

    if (!m_pdfStream.read(&buffer[0], buffSize))
        throw std::exception("Failed to tokenize a document (pdf header don't readed)");

    std::size_t findStringLen = std::strlen(kPDF_VERSION_TOKEN);

    std::size_t pdfTokenBegPos = buffer.find(kPDF_VERSION_TOKEN);
    if (pdfTokenBegPos == std::string::npos)
        throw std::exception("Failed to tokenize a document (pdf header begin don't found)");

    std::size_t pdfTokenEndPos = buffer.find_first_of(" \t\n\r", pdfTokenBegPos + findStringLen);
    if (pdfTokenEndPos == std::string::npos)
        throw std::exception("Failed to tokenize a document (pdf header end don't found)");

    std::string pdfVersion = buffer.substr(pdfTokenBegPos + findStringLen, pdfTokenEndPos - (pdfTokenBegPos + findStringLen));

    sscanf_s(pdfVersion.c_str(), "%d.%d", major, minor);

    m_pdfStream.seekg(pdfTokenEndPos + 1);

    return pdfVersion;
}

template<typename Device>
std::streamoff stream_tokeniser<Device>::getStartXref(bool andLocate) const {
    std::streamsize buffSize = kDEFAULT_BUF_SIZE;

    if (buffSize > m_endStreamPos)
        buffSize = m_endStreamPos;

    std::streamoff startPos = m_pdfStream.tellg();
    std::streamoff currPos = m_endStreamPos;
    std::ios_base::seekdir seekDirection = std::ios_base::end;

    std::streamoff seekedPos(-1);

    while (currPos > startPos) {
        if (currPos < buffSize)
            buffSize = currPos;

        m_pdfStream.seekg(-1 * buffSize, seekDirection);
        currPos = m_pdfStream.tellg();

        std::string buffer(buffSize, '\0');
        if (!m_pdfStream.read(&buffer[0], buffSize))
            throw std::exception("Failed to tokenize a document (start xref position don't readed)");

        std::size_t xrefBufferPos = buffer.rfind("startxref");
        if (xrefBufferPos != std::string::npos) {
            seekedPos = currPos + std::fpos<std::size_t>(xrefBufferPos);

            m_pdfStream.seekg(seekedPos);
            break;
        }
        else {
            m_pdfStream.seekg(currPos);
            seekDirection = std::ios_base::cur;
        }
    }

    if (!andLocate)
        m_pdfStream.seekg(startPos);

    return seekedPos;
}

template<typename Device>
std::pair<std::any, tokeniser_base::Types> stream_tokeniser<Device>::nextToken() const {
    std::stringstream tempBuff;
    std::string tempString;

    char currentChar = '\0';
    char nestedChar1 = '\0';
    char nestedChar2 = '\0';

    do {
        currentChar = m_pdfStream.get();
    } while (currentChar != std::ifstream::traits_type::eof() && tokeniser_base::isWhitespace(currentChar));

    if (currentChar == std::ifstream::traits_type::eof())
        return std::make_pair(nullptr, tokeniser_base::Types::kTK_ENDOFFILE);

    switch (currentChar) {
        case '[':
            return std::make_pair(nullptr, tokeniser_base::Types::kTK_START_ARRAY);
            break;

        case ']':
            return std::make_pair(nullptr, tokeniser_base::Types::kTK_END_ARRAY);
            break;

        case '%':
            tempBuff.str(std::string());

            currentChar = m_pdfStream.get();  // pass-through the %-token and jump to next char
            while (currentChar != std::ifstream::traits_type::eof() && currentChar != '\r' && currentChar != '\n') {
                tempBuff << currentChar;

                currentChar = m_pdfStream.get();
            }

            tempString = tempBuff.str();

            return std::make_pair(std::make_any<std::string>(tempString), tokeniser_base::Types::kTK_COMMENT);
            break;

        case '<':
            nestedChar1 = skipWhitespaces(m_pdfStream.get());

            if (nestedChar1 == '<') {
                return std::make_pair(nullptr, tokeniser_base::Types::kTK_START_DIC);
                break;
            }

            tempBuff.str(std::string());

            while (true) {
                nestedChar1 = skipWhitespaces(nestedChar1);

                if (nestedChar1 == '>')
                    break;

                nestedChar1 = getHex(nestedChar1);
                if (nestedChar1 < 0)
                    break;

                nestedChar2 = skipWhitespaces(m_pdfStream.get());

                if (nestedChar2 == '>') {
                    currentChar = nestedChar1 << 4;

                    tempBuff << currentChar;
                    break;
                }

                nestedChar2 = getHex(nestedChar2);
                if (nestedChar2 < 0)
                    break;

                currentChar = (nestedChar1 << 4) + nestedChar2;
                tempBuff << currentChar;

                nestedChar1 = m_pdfStream.get();
            }

            if (nestedChar1 < 0 || nestedChar2 < 0)
                throw std::exception("Failed to tokenize a document (error reading string)");

            tempString = tempBuff.str();

            return std::make_pair(std::make_any<std::string>(tempString), tokeniser_base::Types::kTK_STRING_HEX);
            break;

        case '>':
            currentChar = m_pdfStream.get();
            if (currentChar != '>') {
                throw std::exception("Failed to tokenize a document (greater than not expected)");
            }

            return std::make_pair(nullptr, tokeniser_base::Types::kTK_END_DIC);
            break;

        case '/':
            tempBuff.str(std::string());

            do {
                currentChar = m_pdfStream.get();

                if (specialChars[currentChar] != 0)
                    break;
                else if (currentChar == '#')  // next two chars is a hex-number
                    currentChar = getHex(m_pdfStream.get() << 4) + getHex(m_pdfStream.get());

                tempBuff << currentChar;
            } while (currentChar != std::ifstream::traits_type::eof());

            tempString = tempBuff.str();
            backOnePosition();  // rewind back one position

            return std::make_pair(std::make_any<std::string>(tempString), tokeniser_base::Types::kTK_NAME);
            break;

        case '(':
            break;  // todo: impl it

        default:
            tempBuff.str(std::string());

            if (currentChar == '-' || currentChar == '+' || currentChar == '.' || (currentChar >= '0' && currentChar <= '9')) {
                if (currentChar == '.')
                    tempBuff << '0';

                do {
                    tempBuff << currentChar;

                    currentChar = m_pdfStream.get();
                } while (currentChar != std::ifstream::traits_type::eof() && ((currentChar >= '0' && currentChar <= '9') || currentChar == '.'));

                tempString = tempBuff.str();

                backOnePosition();

                if (tempString.find(".") == std::string::npos)
                    return std::make_pair(std::make_any<std::int64_t>(boost::lexical_cast<std::int64_t>(tempString)), tokeniser_base::Types::kTK_NUMBER_INT);
                else
                    return std::make_pair(std::make_any<double>(boost::lexical_cast<double>(tempString)), tokeniser_base::Types::kTK_NUMBER_REAL);
            }
            else {
                do {
                    tempBuff << currentChar;

                    currentChar = m_pdfStream.get();
                } while (specialChars[currentChar] == 0);

                tempString = tempBuff.str();

                backOnePosition();

                return std::make_pair(std::make_any<std::string>(tempString), tokeniser_base::Types::kTK_OTHER);
            }
            break;
    }

    return std::make_pair(nullptr, tokeniser_base::Types::kTK_NA);
}

template<typename Device>
std::pair<std::any, tokeniser_base::Types> stream_tokeniser<Device>::nextValidToken() const {
    int level = 0;
    std::streamoff frstTokenPos(0);

    std::int64_t frstTokenValue(0);
    std::int64_t scndTokenValue(0);

    while (true) {
        auto [tokenValue, tokenType] = nextToken();

        if (tokenType == tokeniser_base::Types::kTK_ENDOFFILE)
            break;
        else if (tokenType == tokeniser_base::Types::kTK_COMMENT)
            continue;

        switch (level) {
            case 0:
                if (tokenType != tokeniser_base::Types::kTK_NUMBER_INT)
                    return std::make_pair(tokenValue, tokenType);

                frstTokenPos = tell();
                frstTokenValue = std::any_cast<std::int64_t>(tokenValue);

                ++level;
                break;

            case 1:
                if (tokenType != tokeniser_base::Types::kTK_NUMBER_INT) {
                    seek(frstTokenPos);

                    return std::make_pair(std::make_any<std::int64_t>(frstTokenValue), tokeniser_base::Types::kTK_NUMBER_INT);
                }

                scndTokenValue = std::any_cast<std::int64_t>(tokenValue);

                ++level;
                break;

            default:
                if (tokenType == tokeniser_base::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "R") {
                    return std::make_pair(std::make_any<std::pair<std::int64_t, std::int64_t>>(frstTokenValue, scndTokenValue), tokeniser_base::Types::kTK_REF);
                }
                else {
                    seek(frstTokenPos);

                    return std::make_pair(std::make_any<std::int64_t>(frstTokenValue), tokeniser_base::Types::kTK_NUMBER_INT);
                }
        }
    }

    return std::make_pair(nullptr, tokeniser_base::Types::kTK_NA);
}

template<typename Device>
char stream_tokeniser<Device>::skipWhitespaces(const char currentChar) const {
    char checkChar = currentChar;

    while (tokeniser_base::isWhitespace(checkChar))
        checkChar = m_pdfStream.get();

    return checkChar;
}

template<typename Device>
char stream_tokeniser<Device>::skipWhitespaces() const {
    char checkChar = m_pdfStream.get();

    while (tokeniser_base::isWhitespace(checkChar))
        checkChar = m_pdfStream.get();

    backOnePosition();

    return checkChar;
}
