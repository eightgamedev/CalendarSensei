# pragma once

namespace icalendar
{
	namespace time
	{
		std::chrono::system_clock::time_point DateTimeToTimePoint(const DateTime& dateTime);
		DateTime TimePointToDateTime(const std::chrono::system_clock::time_point& timePoint);
		DateTime parseToDateTime(const String& iCalString);
		DateTime toUTC(const DateTime& dateTime, const String& timeZoneName);
		DateTime toLocal(const DateTime& dateTime, const String& timeZoneName);
		String parseToICalDateTime(const DateTime& dateTime);
		DayOfWeek parseToDayOfWeek(const String& iCalString);
	};
}
