# include "icalendar/ICalendar.hpp"

namespace icalendar
{
	ICalendar::ICalendar()
	{
	}

	ICalendar::~ICalendar()
	{
	}

	void ICalendar::setCalendarProperty(const CalendarProperty& calendarProperty)
	{
		m_calendarProperty = calendarProperty;
	}

	void ICalendar::load(const FilePath& path)
	{
		m_events.clear();
		m_textReader.open(path);
		if (not m_textReader)
		{
			throw Error{ U"Failed to open the file : {}"_fmt(path) };
		}

		setCalendarProperty(parseCalendarProperty());

		String line;
		// std::optional<Event> currentEvent;
		m_textReader.close();
		m_textReader.open(path);
		Array<Array<String>> icsContentAboutEvent; // BEGIN:VEVENTからEND:VEVENTまで行を格納する配列の配列
		Array<String> icsContent; // BEGIN:VEVENTからEND:VEVENTまでの行を格納する配列

		while (m_textReader.readLine(line))
		{
			if (line.starts_with(U"END:VEVENT"))
			{
				icsContent.emplace_back(line);
				icsContentAboutEvent.emplace_back(icsContent);
				icsContent.clear();
			}
			else if (line.starts_with(U"BEGIN:VEVENT"))
			{
				icsContent.clear();
				icsContent.emplace_back(line);
			}
			else
			{
				icsContent.emplace_back(line);
			}
		}

		for (const auto& icsContent : icsContentAboutEvent)
		{
			addEvent(Event::parseFromICal(icsContent));
		}
	}

	void ICalendar::save(const FilePath& path) const
	{
		TextWriter writer{ path };
		if (not writer)
		{
			throw Error{ U"Failed to open the file : {}"_fmt(path) };
		}

		writer.writeln(U"BEGIN:VCALENDAR");

		writer.writeln(U"PRODID:{}"_fmt(m_calendarProperty.getProdId()));
		writer.writeln(U"VERSION:{}"_fmt(m_calendarProperty.getVersion()));
		writer.writeln(U"CALSCALE:{}"_fmt(m_calendarProperty.getCalscale()));
		writer.writeln(U"METHOD:{}"_fmt(m_calendarProperty.getMethod()));

		if (m_calendarProperty.getGoogle_CALNAME().has_value())
		{
			writer.writeln(U"X-WR-CALNAME:{}"_fmt(m_calendarProperty.getGoogle_CALNAME().value()));
		}

		if (m_calendarProperty.getGoogle_CALDESC().has_value())
		{
			writer.writeln(U"X-WR-CALDESC:{}"_fmt(m_calendarProperty.getGoogle_CALDESC().value()));
		}

		if (m_calendarProperty.getGoogle_TIMEZONE().has_value())
		{
			writer.writeln(U"X-WR-TIMEZONE:{}"_fmt(m_calendarProperty.getGoogle_TIMEZONE().value()));
		}

		for (const auto& event : m_events)
		{
			writer.write(event.toICalString());
		}
		writer.writeln(U"END:VCALENDAR");
	}

	void ICalendar::addEvent(const Event& event)
	{
		m_events.push_back(event);
	}

	CalendarProperty ICalendar::getCalendarProperty() const
	{
		return m_calendarProperty;
	}

	Array<Event> ICalendar::getEvents() const
	{
		return m_events;
	}

	CalendarProperty ICalendar::parseCalendarProperty()
	{
		CalendarProperty calendarProperty;
		String line;

		// プレフィックスと対応する処理をマップに格納
		std::map<String, std::function<void(const String&)>> prefixHandlers = {
			{U"PRODID:", [&](const String& value) {
				calendarProperty.setProdId(value);
			}},
			{U"VERSION:", [&](const String& value) {
				calendarProperty.setVersion(value);
			}},
			{U"CALSCALE:", [&](const String& value) {
				calendarProperty.setCalscale(value);
			}},
			{U"METHOD:", [&](const String& value) {
				calendarProperty.setMethod(value);
			}},
			{U"X-WR-CALNAME:", [&](const String& value) {
				calendarProperty.setGoogle_CALNAME(value);
			}},
			{U"X-WR-CALDESC:", [&](const String& value) {
				calendarProperty.setGoogle_CALDESC(value);
			}},
			{U"X-WR-TIMEZONE:", [&](const String& value) {
				calendarProperty.setGoogle_TIMEZONE(value);
			}}
		};

		while (m_textReader.readLine(line))
		{
			if (line.starts_with(U"BEGIN:VEVENT"))
			{
				break;
			}

			// マップをループして、行がどのプレフィックスで始まるかを確認
			for (const auto& [prefix, handler] : prefixHandlers)
			{
				if (line.starts_with(prefix))
				{
					// プレフィックスを除去して値を取得
					String value = line.substr(prefix.length());
					// 対応する処理を実行
					handler(value);
					break;
				}
			}
		}
		return calendarProperty;
	}
}
