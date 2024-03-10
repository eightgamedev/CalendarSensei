# pragma once
# include "icalendar/CalendarProperty.hpp"
# include "icalendar/Event.hpp"

namespace icalendar
{
	class ICalendar
	{
	public:
		ICalendar();
		~ICalendar();

		void setCalendarProperty(const CalendarProperty& calendarProperty);
		// icsファイルを読み込むメソッド
		void load(const FilePath& path);

		// icsファイルに書き込むメソッド
		void save(const FilePath& path) const;

		void addEvent(const Event& event);
 
		Array<Event> getEvents() const;
	private:
		CalendarProperty parseCalendarProperty();
		CalendarProperty m_calendarProperty;
		TextReader m_textReader;
		// Eventクラスのインスタンスのリスト
		Array<Event> m_events;
		bool m_beginEventReached = false;
	};
}
