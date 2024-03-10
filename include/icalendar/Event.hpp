# pragma once
# include "icalendar/Time.hpp"

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
		void setStart(const DateTime& startUTC);
		void setEnd(const DateTime& endUTC);
		void setSummary(const String& summary);
		void setDescription(const String& description);
		void setLocation(const String& location);
		void setTimeStamp(const DateTime& timeStamp);
		void setUid(const String& uid);
		void setClass(const String& classValue);
		void setCreated(const DateTime& created);
		void setLastModified(const DateTime& lastModified);
		void setSequence(const String& sequence);
		void setStatus(const String& status);

		bool isAllDay() const;
		Optional<String> getTimeZoneName() const;
		DateTime getStart() const;
		DateTime getEnd() const;
		String getSummary() const;
		Optional<String> getDescription() const;
		Optional<String> getLocation() const;
		DateTime getTimeStamp() const;
		Optional<String> getUid() const;
		Optional<String> getClass() const;
		Optional<DateTime> getCreated() const;
		Optional<DateTime> getLastModified() const;
		Optional<String> getSequence() const;
		Optional<String> getStatus() const;

	private:
		bool m_isAllDay;

		DateTime m_timeStamp;

		Optional<String> m_timeZoneName;

		// イベントの開始日時
		DateTime m_startUTC;

		// イベントの終了日時
		DateTime m_endUTC;

		// イベントのタイトル
		String m_summary;

		// イベントの説明
		Optional<String> m_description;

		// イベントの場所
		Optional<String> m_location;

		// イベントのID
		Optional<String> m_uid;

		Optional<String> m_class;

		Optional<DateTime> m_created;

		Optional<DateTime> m_lastModified;

		Optional<String> m_sequence;

		Optional<String> m_status;

		Optional<String> m_transp;
	};
}
