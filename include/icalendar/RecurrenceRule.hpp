﻿# pragma once
# include "icalendar/Time.hpp"

namespace icalendar
{

	struct ByDay
	{
		int32 day;
		Optional<DayOfWeek> dayOfWeek;
	};

	class RecurrenceRule
	{
	public:
		RecurrenceRule();
		static RecurrenceRule parseFromICal(const String& rrules);
		void setFrequency(const String& frequency);
		void setInterval(int32 interval);
		void setCount(int32 count);
		void setUntil(const DateTime& until);
		void setDateTimeStart(const DateTime& start);
		void setBySecond(const Array<int32>& bySecond);
		void setByMinute(const Array<int32>& byMinute);
		void setByHour(const Array<int32>& byHour);
		void setByDay(const Array<ByDay>& byDay);
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
		const DateTime& getDateTimeStart() const;
		const Array<int32>& getBySecond() const;
		const Array<int32>& getByMinute() const;
		const Array<int32>& getByHour() const;
		const Array<ByDay>& getByDay() const;
		const Array<int32>& getByMonthDay() const;
		const Array<int32>& getByYearDay() const;
		const Array<int32>& getByWeekNo() const;
		const Array<int32>& getByMonth() const;
		const Array<int32>& getBySetPos() const;
		const Optional<DayOfWeek>& getWeekStart() const;
	private:
		String m_frequency;
		Optional<int32> m_interval;
		Optional<int32> m_count;
		Optional<DateTime> m_until;
		DateTime m_start;
		Array<int32> m_bySecond;
		Array<int32> m_byMinute;
		Array<int32> m_byHour;
		Array<ByDay> m_byDay;
		Array<int32> m_byMonthDay;
		Array<int32> m_byYearDay;
		Array<int32> m_byWeekNo;
		Array<int32> m_byMonth;
		Array<int32> m_bySetPos;
		Optional<DayOfWeek> m_weekStart;
	};
}
