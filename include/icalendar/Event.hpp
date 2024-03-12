# pragma once
# include "icalendar/Time.hpp"
# include "icalendar/RecurrenceRule.hpp"

namespace icalendar
{
	class Event
	{
	public:
		Event();
		~Event();
		static void parseFromICal(Event& event, const String& iCalString);
		String toICalString() const;
		void setIsAllDay(bool isAllDay);
		void setTimeZoneName(const Optional<String>& timeZone);
		void setDateTimeStart(const DateTime& dateTimeStart);
		void setDateTimeEnd(const DateTime& endUTC);
		void setRecurrenceRule(const RecurrenceRule& recurrenceRule);
		void setSummary(const String& summary);
		void setDescription(const String& description);
		void setLocation(const String& location);
		void setTimeStamp(const DateTime& timeStamp);
		void setUniqueID(const String& uniqueID);
		void setClass(const String& classValue);
		void setCreated(const DateTime& created);
		void setLastModified(const DateTime& lastModified);
		void setSequence(const String& sequence);
		void setStatus(const String& status);
		void setTransparent(const String& transparent);

		bool isAllDay() const;
		Optional<String> getTimeZoneName() const;
		DateTime getDateTimeStart() const;
		DateTime getDateTimeEnd() const;
		Optional<RecurrenceRule> getRecurrenceRule() const;
		String getSummary() const;
		Optional<String> getDescription() const;
		Optional<String> getLocation() const;
		DateTime getTimeStamp() const;
		Optional<String> getUniqueID() const;
		Optional<String> getClass() const;
		Optional<DateTime> getCreated() const;
		Optional<DateTime> getLastModified() const;
		Optional<String> getSequence() const;
		Optional<String> getStatus() const;
		Optional<String> getTransparent() const;

	private:
		bool m_isAllDay;

		DateTime m_timeStamp;

		Optional<String> m_timeZoneName;

		// イベントの開始日時 (UTC)
		DateTime m_dateTimeStart;

		// イベントの終了日時 (UTC)
		DateTime m_dateTimeEnd;

		// イベントの繰り返し規則
		Optional<RecurrenceRule> m_recurrenceRule;

		// イベントのタイトル
		String m_summary;

		// イベントの説明
		Optional<String> m_description;

		// イベントの場所
		Optional<String> m_location;

		// イベントのID
		Optional<String> m_uniqueID;

		Optional<String> m_class;

		Optional<DateTime> m_created;

		Optional<DateTime> m_lastModified;

		Optional<String> m_sequence;

		Optional<String> m_status;

		Optional<String> m_transparent;
	};
}
