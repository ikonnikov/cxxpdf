#include "Tokeniser.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

Tokeniser::Tokeniser(std::ifstream& pdfFileStream, std::streamoff startPos) : m_pdfFileStream(pdfFileStream), m_pdfVersionToken("%PDF-") {
    std::cout << u8"this is a ctor::Tokeniser" << std::endl;  // todo: cout delete it

    seek(startPos);
}

Tokeniser::~Tokeniser() noexcept {
    std::cout << u8"this is a dtor::Tokeniser" << std::endl;  // todo: cout delete it
}

const char* Tokeniser::getVersionToken() const {
	return m_pdfVersionToken;
}

std::string Tokeniser::tokenizeDocumentHeader(int* major, int* minor) const {
	std::streamsize buffSize = 1024;
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
    std::streamoff startPos = m_pdfFileStream.tellg();

    std::streamsize buffSize = 1024;

    m_pdfFileStream.seekg(0, std::ios_base::end);
    std::streamoff currPos = m_pdfFileStream.tellg();
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
        }
        else {
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

std::pair<std::any, Tokeniser::TokenTypes> Tokeniser::nextToken() const {
    std::stringstream tempBuff;

    char currentChar = '\0';

    do {
        currentChar = m_pdfFileStream.get();
    } while (currentChar != std::ifstream::traits_type::eof() && Tokeniser::isWhitespace(currentChar));

    if (currentChar == std::ifstream::traits_type::eof())
        return std::make_pair(nullptr, Tokeniser::TokenTypes::kTK_ENDOFFILE);

    switch (currentChar) {
        case '[':
            return std::make_pair(nullptr, Tokeniser::TokenTypes::kTK_START_ARRAY);
            break;
        
        case ']':
            return std::make_pair(nullptr, Tokeniser::TokenTypes::kTK_END_ARRAY);
            break;
        
        default:
            tempBuff.str(std::string());

            if (currentChar == '-' || currentChar == '+' || currentChar == '.' || (currentChar >= '0' && currentChar <= '9')) {
                if (currentChar == '.')
                    tempBuff << '0';

                do {
                    tempBuff << currentChar;

                    currentChar = m_pdfFileStream.get();
                } while (currentChar != std::ifstream::traits_type::eof() && ((currentChar >= '0' && currentChar <= '9') || currentChar == '.'));

                std::string tempString(tempBuff.str());

                if (tempString.find(".") == std::string::npos)
                    return std::make_pair(std::make_any<int64_t>(boost::lexical_cast<int64_t>(tempString)), Tokeniser::TokenTypes::kTK_NUMBER_INT);
                else
                    return std::make_pair(std::make_any<double>(boost::lexical_cast<double>(tempString)), Tokeniser::TokenTypes::kTK_NUMBER_REAL);
            } else {
                do {
                    tempBuff << currentChar;
                    
                    currentChar = m_pdfFileStream.get();
                } while (!specialChars[currentChar]);

                std::string tempString(tempBuff.str());

                return std::make_pair(std::make_any<std::string>(tempString), Tokeniser::TokenTypes::kTK_OTHER);
            }
            break;
    }

    return std::make_pair(nullptr, Tokeniser::TokenTypes::kTK_NA);
}

bool Tokeniser::isWhitespace(int ch) {
    return (ch == 0 || ch == 9 || ch == 10 || ch == 12 || ch == 13 || ch == 32);
}