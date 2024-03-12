# pragma once
# include "icalendar/Time.hpp"

namespace icalendar
{
	class RecurrenceRule
	{
	public:
		RecurrenceRule();
		~RecurrenceRule();
		static void parseFromICal(RecurrenceRule& rule, const String& ical);
		void setFrequency(const String& frequency);
		void setInterval(int32 interval);
		void setCount(int32 count);
		void setUntil(const DateTime& until);
		void setStart(const DateTime& start);
		void setBySecond(const Array<int32>& bySecond);
		void setByMinute(const Array<int32>& byMinute);
		void setByHour(const Array<int32>& byHour);
		void setByDay(const Array<int32>& byDay);
		void setByMonthDay(const Array<int32>& byMonthDay);
		void setByYearDay(const Array<int32>& byYearDay);
		void setByWeekNo(const Array<int32>& byWeekNo);
		void setByMonth(const Array<int32>& byMonth);
		void setBySetPos(const Array<int32>& bySetPos);
		void setWeekStart(const DayOfWeek& weekStart);
		const String& getFrequency() const;
		Optional<int32> getInterval() const;
		Optional<int32> getCount() const;
		const Optional<DateTime>& getUntil() const;
		const DateTime& getStart() const;
		const Optional<Array<int32>>& getBySecond() const;
		const Optional<Array<int32>>& getByMinute() const;
		const Optional<Array<int32>>& getByHour() const;
		const Optional<Array<int32>>& getByDay() const;
		const Optional<Array<int32>>& getByMonthDay() const;
		const Optional<Array<int32>>& getByYearDay() const;
		const Optional<Array<int32>>& getByWeekNo() const;
		const Optional<Array<int32>>& getByMonth() const;
		const Optional<Array<int32>>& getBySetPos() const;
		const Optional<DayOfWeek>& getWeekStart() const;
	private:
		String m_frequency;
		Optional<int32> m_interval;
		Optional<int32> m_count;
		Optional<DateTime> m_until;
		DateTime m_start;
		Optional<Array<int32>> m_bySecond;
		Optional<Array<int32>> m_byMinute;
		Optional<Array<int32>> m_byHour;
		Optional<Array<int32>> m_byDay;
		Optional<Array<int32>> m_byMonthDay;
		Optional<Array<int32>> m_byYearDay;
		Optional<Array<int32>> m_byWeekNo;
		Optional<Array<int32>> m_byMonth;
		Optional<Array<int32>> m_bySetPos;
		Optional<DayOfWeek> m_weekStart;
	};
}
