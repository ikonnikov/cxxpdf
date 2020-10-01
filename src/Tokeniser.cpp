// Copyright (c) 2020 cxxPDF project, Ikonnikov Kirill, All rights reserved.
//
// Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.

#include "Tokeniser.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

Tokeniser::Tokeniser(std::ifstream& pdfFileStream, std::streamoff startPos) : m_pdfFileStream(pdfFileStream), m_endStreamPos(-1), m_pdfVersionToken("%PDF-") {
    m_pdfFileStream.seekg(0, std::ios_base::end);
    m_endStreamPos = m_pdfFileStream.tellg();

    seek(startPos);
}

Tokeniser::~Tokeniser() noexcept {
    // dtor
}

std::string Tokeniser::getDocumentHeader(int* major, int* minor) const {
    std::streamsize buffSize = kDEFAULT_BUF_SIZE;

    if (buffSize > m_endStreamPos)
        buffSize = m_endStreamPos;

    std::string buffer(buffSize, '\0');

    if (!m_pdfFileStream.read(&buffer[0], buffSize))
        throw std::exception("Failed to tokenize a document (pdf header don't readed)");

    size_t findStringLen = strlen(m_pdfVersionToken);

    std::size_t pdfTokenBegPos = buffer.find(m_pdfVersionToken);
    if (pdfTokenBegPos == std::string::npos)
        throw std::exception("Failed to tokenize a document (pdf header begin don't found)");

    auto pdfTokenEndPos = buffer.find_first_of(" \t\n\r", pdfTokenBegPos + findStringLen);
    if (pdfTokenEndPos == std::string::npos)
        throw std::exception("Failed to tokenize a document (pdf header end don't found)");

    std::string pdfVersion = buffer.substr(pdfTokenBegPos + findStringLen, pdfTokenEndPos - (pdfTokenBegPos + findStringLen));

    sscanf_s(pdfVersion.c_str(), "%d.%d", major, minor);

    m_pdfFileStream.seekg(pdfTokenEndPos + 1);

    return pdfVersion;
}

std::streamoff Tokeniser::getStartXref(bool andLocate) const {
    std::streamsize buffSize = kDEFAULT_BUF_SIZE;

    if (buffSize > m_endStreamPos)
        buffSize = m_endStreamPos;

    std::streamoff startPos = m_pdfFileStream.tellg();
    std::streamoff currPos = m_endStreamPos;
    std::ios_base::seekdir seekDirection = std::ios_base::end;

    std::streamoff seekedPos(-1);

    while (currPos > startPos) {
        if (currPos < buffSize)
            buffSize = currPos;

        m_pdfFileStream.seekg(-1 * buffSize, seekDirection);
        currPos = m_pdfFileStream.tellg();

        std::string buffer(buffSize, '\0');
        if (!m_pdfFileStream.read(&buffer[0], buffSize))
            throw std::exception("Failed to tokenize a document (start xref position don't readed)");

        std::size_t xrefBufferPos = buffer.rfind("startxref");
        if (xrefBufferPos != std::string::npos) {
            seekedPos = currPos + std::fpos<std::size_t>(xrefBufferPos);

            m_pdfFileStream.seekg(seekedPos);
            break;
        } else {
            m_pdfFileStream.seekg(currPos);
            seekDirection = std::ios_base::cur;
        }
    }

    if (!andLocate)
        m_pdfFileStream.seekg(startPos);

    return seekedPos;
}

void Tokeniser::seek(std::streamoff newPos) const {
    m_pdfFileStream.seekg(newPos);
}

std::streamoff Tokeniser::tell() const {
    return m_pdfFileStream.tellg();
}

void Tokeniser::backOnePosition() const {
    m_pdfFileStream.seekg(-1, std::ios_base::cur);
}

std::pair<std::any, Tokeniser::Types> Tokeniser::nextToken() const {
    std::stringstream tempBuff;
    std::string tempString;

    char currentChar = '\0';
    char nestedChar1 = '\0';
    char nestedChar2 = '\0';

    do {
        currentChar = m_pdfFileStream.get();
    } while (currentChar != std::ifstream::traits_type::eof() && Tokeniser::isWhitespace(currentChar));

    if (currentChar == std::ifstream::traits_type::eof())
        return std::make_pair(nullptr, Tokeniser::Types::kTK_ENDOFFILE);

    switch (currentChar) {
    case '[':
        return std::make_pair(nullptr, Tokeniser::Types::kTK_START_ARRAY);
        break;

    case ']':
        return std::make_pair(nullptr, Tokeniser::Types::kTK_END_ARRAY);
        break;

    case '%':
        tempBuff.str(std::string());

        do {  // todo: skip %-token
            tempBuff << currentChar;

            currentChar = m_pdfFileStream.get();
        } while (currentChar != std::ifstream::traits_type::eof() && currentChar != '\r' && currentChar != '\n');

        tempString = tempBuff.str();

        return std::make_pair(std::make_any<std::string>(tempString), Tokeniser::Types::kTK_COMMENT);
        break;

    case '<':
        nestedChar1 = m_pdfFileStream.get();

        while (isWhitespace(nestedChar1))
            nestedChar1 = m_pdfFileStream.get();

        if (nestedChar1 == '<') {
            return std::make_pair(nullptr, Tokeniser::Types::kTK_START_DIC);
            break;
        }

        tempBuff.str(std::string());

        while (true) {
            while (isWhitespace(nestedChar1))
                nestedChar1 = m_pdfFileStream.get();

            if (nestedChar1 == '>')
                break;

            nestedChar1 = getHex(nestedChar1);
            if (nestedChar1 < 0)
                break;

            nestedChar2 = m_pdfFileStream.get();

            while (isWhitespace(nestedChar2))
                nestedChar2 = m_pdfFileStream.get();

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

            nestedChar1 = m_pdfFileStream.get();
        }

        if (nestedChar1 < 0 || nestedChar2 < 0)
            throw std::exception("Failed to tokenize a document (error reading string)");

        tempString = tempBuff.str();

        return std::make_pair(std::make_any<std::string>(tempString), Tokeniser::Types::kTK_STRING);
        break;

    case '>':
        currentChar = m_pdfFileStream.get();
        if (currentChar != '>') {
            auto poss = m_pdfFileStream.tellg();  // todo: delete it
            throw std::exception("Failed to tokenize a document (greater than not expected)");
        }

        return std::make_pair(nullptr, Tokeniser::Types::kTK_END_DIC);
        break;

    case '/':
        tempBuff.str(std::string());

        do {
            currentChar = m_pdfFileStream.get();

            if (specialChars[currentChar] != 0)
                break;
            else if (currentChar == '#')  // next two chars is a hex-number
                currentChar = getHex(m_pdfFileStream.get() << 4) + getHex(m_pdfFileStream.get());

            tempBuff << currentChar;
        } while (currentChar != std::ifstream::traits_type::eof());

        tempString = tempBuff.str();
        backOnePosition();  // rewind back one position

        return std::make_pair(std::make_any<std::string>(tempString), Tokeniser::Types::kTK_NAME);
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

                currentChar = m_pdfFileStream.get();
            } while (currentChar != std::ifstream::traits_type::eof() && ((currentChar >= '0' && currentChar <= '9') || currentChar == '.'));

            tempString = tempBuff.str();

            backOnePosition();

            if (tempString.find(".") == std::string::npos)
                return std::make_pair(std::make_any<int64_t>(boost::lexical_cast<int64_t>(tempString)), Tokeniser::Types::kTK_NUMBER_INT);
            else
                return std::make_pair(std::make_any<double>(boost::lexical_cast<double>(tempString)), Tokeniser::Types::kTK_NUMBER_REAL);
        } else {
            do {
                tempBuff << currentChar;

                currentChar = m_pdfFileStream.get();
            } while (!specialChars[currentChar]);
            //} while (specialChars[currentChar] == 0);  // todo: is to best?

            tempString = tempBuff.str();

            backOnePosition();

            return std::make_pair(std::make_any<std::string>(tempString), Tokeniser::Types::kTK_OTHER);
        }
        break;
    }

    return std::make_pair(nullptr, Tokeniser::Types::kTK_NA);
}

bool Tokeniser::isWhitespace(int ch) {
    return (ch == 0 || ch == 9 || ch == 10 || ch == 12 || ch == 13 || ch == 32);
}

int Tokeniser::getHex(int v) {
    int retValue = 0;

    if (v >= '0' && v <= '9')
        retValue = v - '0';
    else if (v >= 'A' && v <= 'F')
        retValue = v - 'A' + 10;
    else if (v >= 'a' && v <= 'f')
        retValue = v - 'a' + 10;
    else
        retValue = -1;

    return retValue;
}

std::pair<std::any, Tokeniser::Types> Tokeniser::nextValidToken() const {
    int level = 0;
    std::streamoff fstPos;

    int64_t fstToken(0);
    int64_t scndToken(0);

    while (true) {
        auto [tokenValue, tokenType] = nextToken();

        if (tokenType == Tokeniser::Types::kTK_ENDOFFILE)
            break;
        else if (tokenType == Tokeniser::Types::kTK_COMMENT)
            continue;

        switch (level) {
        case 0:
            if (tokenType != Tokeniser::Types::kTK_NUMBER_INT)
                return std::make_pair(tokenValue, tokenType);

            fstPos = tell();
            fstToken = std::any_cast<int64_t>(tokenValue);

            ++level;
            break;

        case 1:
            if (tokenType != Tokeniser::Types::kTK_NUMBER_INT) {
                seek(fstPos);

                return std::make_pair(std::make_any<int64_t>(fstToken), Tokeniser::Types::kTK_NUMBER_INT);
            }

            scndToken = std::any_cast<int64_t>(tokenValue);

            ++level;
            break;

        default:
            if (tokenType == Tokeniser::Types::kTK_OTHER && std::any_cast<std::string>(tokenValue) == "R")
                return std::make_pair(std::make_any<std::pair<int64_t, int64_t>>(fstToken, scndToken), Tokeniser::Types::kTK_REF);
            else {
                seek(fstPos);
                
                return std::make_pair(std::make_any<int64_t>(fstToken), Tokeniser::Types::kTK_NUMBER_INT);
            }
        }
    }

    return std::make_pair(nullptr, Tokeniser::Types::kTK_NA);
}
