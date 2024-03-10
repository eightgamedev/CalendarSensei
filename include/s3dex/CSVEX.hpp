# pragma once
# include <unicode/ucnv.h>
# include <unicode/ucsdet.h>

namespace s3dex
{
	class CSVEX
	{
	public:
		CSVEX() = delete;
		static s3d::CSV open(const FilePathView& path);

		/// @brief 
		/// @param csv 
		/// @param path 
		/// @param characterSet 文字コードを指定します。デフォルトはUTF-8です。
		static void save(const s3d::CSV& csv, const FilePathView& path, const String& characterSet = U"UTF-8");
	};
}
