# include "s3dex/icalendar/TimeZone.hpp"

namespace s3dex::icalendar::timezone
{
	HashTable<String, int32> timeZoneOffsetMinutesMap =
	{



	};

	TimeZone::TimeZone(String id)
		: m_id(id)
	{
	}

	TimeZone::~TimeZone()
	{
	}
}
