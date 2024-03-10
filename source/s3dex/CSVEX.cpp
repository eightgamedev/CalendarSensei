# include "s3dex/CSVEX.hpp"

namespace s3dex
{
	s3d::CSV CSVEX::open(const FilePathView& path)
	{
		BinaryReader reader(path);
		if (!reader)
		{
			// ファイルのオープンに失敗
			return CSV();
		}

		// ファイルのサイズを取得
		const size_t fileSize = reader.size();

		// ファイルの内容をバイト列として読み込む
		std::string bytes(fileSize, '\0');
		reader.read(bytes.data(), fileSize);

		// ICUの文字コード検出器を開く
		UErrorCode status = U_ZERO_ERROR;
		UCharsetDetector* csd = ucsdet_open(&status);

		// バイト列を文字コード検出器に設定する
		ucsdet_setText(csd, reinterpret_cast<const char*>(bytes.data()), bytes.size(), &status);

		// 最も可能性の高い文字コードを検出する
		const UCharsetMatch* match = ucsdet_detect(csd, &status);
		const char* name = ucsdet_getName(match, &status);

		// 文字コード検出器を閉じる
		ucsdet_close(csd);

		// 文字コードの変換
		std::string convertedBytes(fileSize * 2, '\0'); // 変換後のバイト列の最大サイズを確保
		UErrorCode convertStatus = U_ZERO_ERROR;
		int32_t convertedSize = ucnv_convert("UTF-8", name, convertedBytes.data(), convertedBytes.size(), bytes.data(), bytes.size(), &convertStatus);
		if (U_FAILURE(convertStatus))
		{
			// 変換に失敗
			return CSV();
		}

		// バイト列を文字列に変換
		const std::string str(convertedBytes.begin(), convertedBytes.begin() + convertedSize);
		const String text = Unicode::FromUTF8(str);
		CSV csv;
		for (const auto& line : text.split(U'\n'))
		{
			if (line.isEmpty()) // 空行を無視
			{
				continue;
			}
			for (auto& column : line.split(U','))
			{
				column.trim();
				csv.write(column);
			}
			csv.newLine();
		}
		return csv;
	}

	void CSVEX::save(const s3d::CSV& csv, const FilePathView& path, const String& characterSet)
	{
		BinaryWriter writer(path);
		if (!writer)
		{
			// ファイルのオープンに失敗
			return;
		}

		// CSVを文字列に変換
		String text;
		for (size_t i = 0; i < csv.rows(); ++i)
		{
			for (size_t j = 0; j < csv.columns(i); ++j)
			{
				text += csv[i][j];
				if (j != csv.columns(i) - 1)
				{
					text += U',';
				}
			}
			text += U'\n';
		}

		// 文字列をUTF-8バイト列に変換
		const std::u16string utf16 = Unicode::ToUTF16(text);

		// UTF-8バイト列を指定された文字セットのバイト列に変換
		UErrorCode error = U_ZERO_ERROR;
		UConverter* conv = ucnv_open(characterSet.narrow().c_str(), &error);
		if (U_FAILURE(error))
		{
			// エラーハンドリング
			return;
		}

		int32_t destCapacity = utf16.length() * 2; // 変換後のバイト列の最大長
		char* dest = new char[destCapacity];
		int32_t destLength = ucnv_fromUChars(conv, dest, destCapacity, reinterpret_cast<const UChar*>(utf16.c_str()), utf16.length(), &error);
		if (U_FAILURE(error))
		{
			// エラーハンドリング
			delete[] dest;
			return;
		}

		// 変換したバイト列をファイルに書き込む
		writer.write(dest, destLength);

		delete[] dest;
		ucnv_close(conv);
	}
}
