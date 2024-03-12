# include "icalendar/Time.hpp"

namespace icalendar::time
{
	std::chrono::system_clock::time_point DateTimeToTimePoint(const DateTime& dateTime)
	{
		std::tm t{};
		t.tm_sec = dateTime.second;
		t.tm_min = dateTime.minute;
		t.tm_hour = dateTime.hour;
		t.tm_mday = dateTime.day;
		t.tm_mon = dateTime.month - 1; // tm_monは0から始まるので1を引く
		t.tm_year = dateTime.year - 1900; // tm_yearは1900からの年数なので1900を引く
		t.tm_isdst = -1; // 夏時間はシステムに任せる

		std::time_t tt = std::mktime(&t);
		if (tt == -1)
		{
			throw std::runtime_error("Failed to convert DateTime to time_t");
		}

		return std::chrono::system_clock::from_time_t(tt);
	}

	DateTime TimePointToDateTime(const std::chrono::system_clock::time_point& timePoint)
	{
		std::time_t tt = std::chrono::system_clock::to_time_t(timePoint);
		std::tm t;
		localtime_s(&t, &tt);

		return DateTime{ t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec };
	}

	/// @brief DateTimeを指定したタイムゾーンの時刻として解釈しUTCに変換
	/// @param iCalString 
	/// @param timeZoneName 
	/// @return DateTime (UTC) 
	DateTime toUTC(const DateTime& dateTime, const String& timeZoneName)
	{
		// DateTimeからchrono::system_clock::time_pointに変換
		std::chrono::system_clock::time_point timePoint = DateTimeToTimePoint(dateTime);

		// タイムゾーンを考慮してchrono::zoned_timeに変換
		std::chrono::zoned_time zt{ std::chrono::locate_zone(timeZoneName.narrow()), timePoint };

		// UTCに変換
		std::chrono::zoned_time ztUTC{ std::chrono::locate_zone("UTC"), zt.get_sys_time() };

		// chrono::system_clock::time_pointからDateTimeに変換
		return TimePointToDateTime(ztUTC.get_sys_time());
	}

	/// @brief dateTimeUTCを指定したタイムゾーンの時刻に変換
	/// @param dateTimeUTC 
	/// @param timeZoneName 
	/// @return 
	DateTime toLocal(const DateTime& dateTimeUTC, const String& timeZoneName)
	{
		// DateTimeからchrono::system_clock::time_pointに変換
		std::chrono::system_clock::time_point timePoint = DateTimeToTimePoint(dateTimeUTC);

		// タイムゾーンを考慮してchrono::zoned_timeに変換
		std::chrono::zoned_time zt{ std::chrono::locate_zone(timeZoneName.narrow()), timePoint };

		// chrono::system_clock::time_pointからDateTimeに変換
		return TimePointToDateTime(zt.get_sys_time());
	}

	/// @brief iCal文字列をDateTimeに変換
	/// @param iCalString yyyyMMddTHHmmSS or yyyyMMdd
	/// @return DateTime
	DateTime parseToDateTime(const String& iCalString)
	{

		int32 year = std::stoi(iCalString.substr(0, 4).narrow(), nullptr, 10);
		int32 month = std::stoi(iCalString.substr(4, 2).narrow(), nullptr, 10);
		int32 day = std::stoi(iCalString.substr(6, 2).narrow(), nullptr, 10);

		if (iCalString.length() == 8)
		{
			return DateTime{ year, month, day };
		}
		int32 hour = std::stoi(iCalString.substr(9, 2).narrow(), nullptr, 10);
		int32 minute = std::stoi(iCalString.substr(11, 2).narrow(), nullptr, 10);
		int32 second = std::stoi(iCalString.substr(13, 2).narrow(), nullptr, 10);
		return DateTime{ year, month, day, hour, minute, second };
	}



	/// @brief DateTimeをiCal文字列に変換
	/// @param dateTime DateTime
	/// @return iCal文字列
	String time::parseToICalDateTime(const DateTime& dateTime)
	{
		return dateTime.format(U"yyyyMMdd") + U"T" + dateTime.format(U"HHmmss") + U"Z";
	}

	/// @brief iCal文字列をDayOfWeekに変換
	/// @param iCalString
	/// @return DayOfWeek
	DayOfWeek time::parseToDayOfWeek(const String& iCalString)
	{
		if (iCalString == U"SU")
		{
			return DayOfWeek::Sunday;
		}
		else if (iCalString == U"MO")
		{
			return DayOfWeek::Monday;
		}
		else if (iCalString == U"TU")
		{
			return DayOfWeek::Tuesday;
		}
		else if (iCalString == U"WE")
		{
			return DayOfWeek::Wednesday;
		}
		else if (iCalString == U"TH")
		{
			return DayOfWeek::Thursday;
		}
		else if (iCalString == U"FR")
		{
			return DayOfWeek::Friday;
		}
		else if (iCalString == U"SA")
		{
			return DayOfWeek::Saturday;
		}
		else
		{
			throw Error(U"Invalid DayOfWeek string"_fmt(iCalString));
		}
	}

}
