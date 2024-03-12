# pragma once
# include "icalendar/Event.hpp"

namespace icalendar
{
	Event::Event()
	{
	}

	Event::~Event()
	{
	}

	void Event::parseFromICal(Event& event, const String& iCalString) 
	{
		// プレフィックスと対応する処理をマップに格納
		std::map<String, std::function<void(const String&)>> prefixHandlers = {
			{U"DTSTART;VALUE=DATE:", [&](const String& value) {
				DateTime startDate = time::parseToDateTime(value);
				event.setStart(startDate);
				event.setIsAllDay(true);
			}},
			{U"DTSTART;TZID=", [&](const String& value) {
				const size_t timeZoneNameEnd = value.includes(U":") ? value.indexOf(U":") : value.length();
				const String timeZoneName = value.substr(0, timeZoneNameEnd);

				const DateTime dateTimeLocal = time::parseToDateTime(value.substr(timeZoneNameEnd + 1));
				const DateTime dateTimeUTC = time::toUTC(dateTimeLocal, timeZoneName);

				event.setTimeZoneName(timeZoneName);
				event.setStart(dateTimeUTC);
				event.setIsAllDay(false);
			}},

			{U"DTSTART:", [&](const String& value) {
				DateTime startDate = time::parseToDateTime(value);
				event.setStart(startDate);
				event.setIsAllDay(false);
			}},
			{U"DTEND;VALUE=DATE:", [&](const String& value) {
				DateTime endDate = time::parseToDateTime(value);
				event.setEnd(endDate);
			}},
			{U"DTEND;TZID=", [&](const String& value) {
				const size_t timeZoneNameEnd = value.includes(U":") ? value.indexOf(U":") : value.length();
				const String timeZoneName = value.substr(0, timeZoneNameEnd);

				const DateTime dateTimeLocal = time::parseToDateTime(value.substr(timeZoneNameEnd + 1));
				const DateTime dateTimeUTC = time::toUTC(dateTimeLocal, timeZoneName);

				event.setTimeZoneName(timeZoneName);
				event.setEnd(dateTimeUTC);
			}},
			{U"DTEND:", [&](const String& value) {
				DateTime endDate = time::parseToDateTime(value);
				event.setEnd(endDate);
			}},

			{U"RRULE:", [&](const String& value) {
				RecurrenceRule rule = RecurrenceRule::parseFromICal(value);
				event.setRecurrenceRule(rule);
			}},

			{U"SUMMARY:", [&](const String& value) {
				event.setSummary(value);
			}},
			{U"DESCRIPTION:", [&](const String& value) {
				event.setDescription(value);
			}},
			{U"LOCATION:", [&](const String& value) {
				event.setLocation(value);
			}},
			{U"DTSTAMP:", [&](const String& value) {
				DateTime timeStamp = time::parseToDateTime(value);
				event.setTimeStamp(timeStamp);
			}},
			{U"UID:", [&](const String& value) {
				event.setUid(value);
			}},
			{U"CLASS:", [&](const String& value) {
				event.setClass(value);
			}},
			{U"CREATED:", [&](const String& value) {
				DateTime created = time::parseToDateTime(value);
				event.setCreated(created);
			}},
			{U"LAST-MODIFIED:", [&](const String& value) {
				DateTime lastModified = time::parseToDateTime(value);
				event.setLastModified(lastModified);
			}},
			{U"SEQUENCE:", [&](const String& value) {
				event.setSequence(value);
			}},
			{U"STATUS:", [&](const String& value) {
				event.setStatus(value);
			}},
		};

		// マップをループして、行がどのプレフィックスで始まるかを確認
		for (const auto& [prefix, handler] : prefixHandlers)
		{
			if (iCalString.starts_with(prefix))
			{
				// プレフィックスを除去して値を取得
				String value = iCalString.substr(prefix.length());
				// 対応する処理を実行
				handler(value);
				break;
			}
		}
	}

	String Event::toICalString() const {
		String iCalString;
		iCalString += U"BEGIN:VEVENT\n";
		if (isAllDay())
		{
			iCalString += U"DTSTART;VALUE=DATE:" + getStart().format(U"yyyyMMdd") + U"\n";
			iCalString += U"DTEND;VALUE=DATE:" + getEnd().format(U"yyyyMMdd") + U"\n";
		}
		else
		{
			iCalString += U"DTSTART:" + time::parseToICalDateTime(getStart()) + U"\n";
			iCalString += U"DTEND:" + time::parseToICalDateTime(getEnd()) + U"\n";
		}

		iCalString += U"SUMMARY:" + getSummary() + U"\n";
		if (getDescription().has_value())
		{
			iCalString += U"DESCRIPTION:" + getDescription().value() + U"\n";
		}
		if (getLocation().has_value())
		{
			iCalString += U"LOCATION:" + getLocation().value() + U"\n";
		}
		iCalString += U"DTSTAMP:" + time::parseToICalDateTime(getTimeStamp()) + U"\n";
		iCalString += U"END:VEVENT\n";
		return iCalString;
	}

	void Event::setIsAllDay(bool isAllDay)
	{
		m_isAllDay = isAllDay;
	}

	void Event::setTimeZoneName(const Optional<String>& timeZoneName)
	{
		m_timeZoneName = timeZoneName;
	}

	void Event::setStart(const DateTime& startUTC)
	{
		m_startUTC = startUTC;
	}

	void Event::setEnd(const DateTime& endUTC)
	{
		m_endUTC = endUTC;
	}

	void Event::setRecurrenceRule(const RecurrenceRule& recurrenceRule)
	{
		m_recurrenceRule = recurrenceRule;
	}

	void Event::setSummary(const String& summary)
	{
		m_summary = summary;
	}

	void Event::setDescription(const String& description)
	{
		m_description = description;
	}

	void Event::setLocation(const String& location)
	{
		m_location = location;
	}

	void Event::setTimeStamp(const DateTime& timeStamp)
	{
		m_timeStamp = timeStamp;
	}

	void Event::setUid(const String& uid)
	{
		m_uid = uid;
	}

	void Event::setClass(const String& classValue)
	{
		m_class = classValue;
	}

	void Event::setCreated(const DateTime& created)
	{
		m_created = created;
	}

	void Event::setLastModified(const DateTime& lastModified)
	{
		m_lastModified = lastModified;
	}

	void Event::setSequence(const String& sequence)
	{
		m_sequence = sequence;
	}

	void Event::setStatus(const String& status)
	{
		m_status = status;
	}

	void Event::setTransparent(const String& transparent)
	{
		m_transparent = transparent;
	}

	bool Event::isAllDay() const
	{
		return m_isAllDay;
	}

	Optional<String> Event::getTimeZoneName() const
	{
		return m_timeZoneName;
	}

	DateTime Event::getStart() const
	{
		return m_startUTC;
	}

	DateTime Event::getEnd() const
	{
		return m_endUTC;
	}

	Optional<RecurrenceRule> Event::getRecurrenceRule() const
	{
		return m_recurrenceRule;
	}

	String Event::getSummary() const
	{
		return m_summary;
	}

	Optional<String> Event::getDescription() const
	{
		return m_description;
	}

	Optional<String> Event::getLocation() const
	{
		return m_location;
	}

	DateTime Event::getTimeStamp() const
	{
		return m_timeStamp;
	}

	Optional<String> Event::getUid() const
	{
		return m_uid;
	}

	Optional<String> Event::getClass() const
	{
		return m_class;
	}

	Optional<DateTime> Event::getCreated() const
	{
		return m_created;
	}

	Optional<DateTime> Event::getLastModified() const
	{
		return m_lastModified;
	}

	Optional<String> Event::getSequence() const
	{
		return m_sequence;
	}

	Optional<String> Event::getStatus() const
	{
		return m_status;
	}

	Optional<String> Event::getTransparent() const
	{
		return m_transparent;
	}
}
