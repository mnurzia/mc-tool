#include "LangDef.hpp"

namespace App::Components::FunctionEditor {
	
	static bool TokenizeCStyleString(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end) {
		const char* p = in_begin;

		if (*p == '"') {
			p++;

			while (p < in_end) {
				// handle end of string
				if (*p == '"') {
					out_begin = in_begin;
					out_end = p + 1;
					return true;
				}

				// handle escape character for "
				if (*p == '\\' && p + 1 < in_end && p[1] == '"')
					p++;

				p++;
			}
		}

		return false;
	}

	static bool TokenizeCStyleIdentifier(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end) {
		const char* p = in_begin;

		if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_') {
			p++;

			while ((p < in_end) && ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_'))
				p++;

			out_begin = in_begin;
			out_end = p;
			return true;
		}

		return false;
	}

	static bool TokenizeMCFSelector(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end) {
		const char* p = in_begin;

		if (*p == '@') {
			if (p < in_end) {
				p++;
				if (*p == 'p' || *p == 'r' || *p == 'a' || *p == 'e' || *p == 's') {
					p++;
					out_begin = in_begin;
					out_end = p;
					return true;
				}
			}
		}
		return false;
	}

	static bool TokenizeCStyleNumber(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
	{
		const char* p = in_begin;

		const bool startsWithNumber = *p >= '0' && *p <= '9';

		if (*p != '+' && *p != '-' && !startsWithNumber)
			return false;

		p++;

		bool hasNumber = startsWithNumber;

		while (p < in_end && (*p >= '0' && *p <= '9')) {
			hasNumber = true;

			p++;
		}

		if (hasNumber == false)
			return false;

		bool isFloat = false;

		if (p < in_end) {
			if (*p == '.') {
				isFloat = true;

				p++;

				while (p < in_end && (*p >= '0' && *p <= '9'))
					p++;
			}
		}

		if (hasNumber) {
			// floating point exponent
			if (p < in_end && (*p == 'e' || *p == 'E')) {
				isFloat = true;

				p++;

				if (p < in_end && (*p == '+' || *p == '-'))
					p++;

				bool hasDigits = false;

				while (p < in_end && (*p >= '0' && *p <= '9')) {
					hasDigits = true;

					p++;
				}

				if (hasDigits == false)
					return false;
			}

		}

		out_begin = in_begin;
		out_end = p;
		return true;
	}

	static bool TokenizeCStylePunctuation(const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end)
	{
		(void)in_end;

		switch (*in_begin) {
			case '[':
			case ']':
			case '{':
			case '}':
			case '!':
			case '%':
			case '^':
			case '&':
			case '*':
			case '(':
			case ')':
			case '-':
			case '+':
			case '=':
			case '~':
			case '|':
			case '<':
			case '>':
			case '?':
			case ':':
			case '/':
			case ';':
			case ',':
			case '.':
				out_begin = in_begin;
				out_end = in_begin + 1;
				return true;
		}

		return false;
	}

	const TextEditor::LanguageDefinition& FunctionLangDef()
	{
		static bool inited = false;
		static TextEditor::LanguageDefinition lang_def;
		if (!inited) {
			static const char* const mcf_keywords[] = {
				"execute", "tellraw", "time"
			};
			for (auto& k : mcf_keywords)
				lang_def.mKeywords.insert(k);

			static const char* const identifiers[] = {
				"if", "as"
			};
			static const char* const mcf_selectors[] = {
				"@p", "@r", "@a", "@e", "@s"
			};
			for (auto& k : identifiers) {
				TextEditor::Identifier id;
				id.mCustomColor = TextEditor::PaletteIndex::Identifier;
				lang_def.mIdentifiers.insert(std::make_pair(std::string(k), id));
			}
			for (auto& k : mcf_selectors) {
				TextEditor::Identifier id;
				id.mCustomColor = TextEditor::PaletteIndex::KnownIdentifier;
				lang_def.mIdentifiers.insert(std::make_pair(std::string(k), id));
			}

			lang_def.mTokenize = [](const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, TextEditor::PaletteIndex& paletteIndex) -> bool {
				paletteIndex = TextEditor::PaletteIndex::Max;

				while (in_begin < in_end && isascii(*in_begin) && isblank(*in_begin))
					in_begin++;

				if (in_begin == in_end) {
					out_begin = in_end;
					out_end = in_end;
					paletteIndex = TextEditor::PaletteIndex::Default;
				}
				else if (TokenizeCStyleString(in_begin, in_end, out_begin, out_end))
					paletteIndex = TextEditor::PaletteIndex::String;
				else if (TokenizeMCFSelector(in_begin, in_end, out_begin, out_end))
					paletteIndex = TextEditor::PaletteIndex::Identifier;
				else if (TokenizeCStyleIdentifier(in_begin, in_end, out_begin, out_end))
					paletteIndex = TextEditor::PaletteIndex::Identifier;
				else if (TokenizeCStyleNumber(in_begin, in_end, out_begin, out_end))
					paletteIndex = TextEditor::PaletteIndex::Number;
				else if (TokenizeCStylePunctuation(in_begin, in_end, out_begin, out_end))
					paletteIndex = TextEditor::PaletteIndex::Punctuation;

				return paletteIndex != TextEditor::PaletteIndex::Max;
			};
			// Temporary workaround
			lang_def.mCommentStart = "////////////////";
			lang_def.mCommentEnd = "////////////////";
			lang_def.mSingleLineComment = "#";

			lang_def.mCaseSensitive = true;
			lang_def.mAutoIndentation = true;

			lang_def.mName = "mcfunction";

			inited = true;
		}
		return lang_def;
	}

}