# include "icalendar/RecurrenceRule.hpp"

namespace icalendar
{
	RecurrenceRule::RecurrenceRule()
	{
	}

	void RecurrenceRule::parseFromICal(RecurrenceRule& rule, const String& icalRow)
	{
		// プレフィックスと対応する処理をマップに格納
		HashTable<String, std::function<void(const String&)>> prefixHandlers = {
			{ U"FREQ=", [&](const String& value) { rule.setFrequency(value); } },
			{ U"INTERVAL=", [&](const String& value) { rule.setInterval(ParseInt<int32>(value)); } },
			{ U"COUNT=", [&](const String& value) { rule.setCount(ParseInt<int32>(value)); } },
			{ U"UNTIL=", [&](const String& value) {
				// DTSTARTの値型とUNTILの値型が一致しているか、
				// またDTSTARTが現地時間で指定されている場合にUNTILも現地時間で指定されているかなどのチェックが必要

				// UNTILの値がDATE-TIME形式であるかチェック
				bool isDateTime = value.contains('T');

				// UNTILの値がDATE-TIME形式であり、かつUTC時間であるかチェック
				bool isUTC = isDateTime && value.ends_with('Z');

				// UNTILの値がDATE形式、またはDATE-TIME形式でUTC時間である場合のみ、値を設定
				if (!isDateTime || isUTC)
				{
					rule.setUntil(icalendar::time::parseToDateTime(value));
				}
				else
				{
					// それ以外の場合はエラーをスロー
					throw Error(U"Invalid UNTIL value: {}"_fmt(value));
				}
			}},
			{ U"BYSECOND=", [&](const String& value) { rule.setBySecond(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYMINUTE=", [&](const String& value) { rule.setByMinute(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYHOUR=", [&](const String& value) { rule.setByHour(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYDAY=", [&](const String& value) { rule.setByDay(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYMONTHDAY=", [&](const String& value) { rule.setByMonthDay(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYYEARDAY=", [&](const String& value) { rule.setByYearDay(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYWEEKNO=", [&](const String& value) { rule.setByWeekNo(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYMONTH=", [&](const String& value) { rule.setByMonth(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
			{ U"BYSETPOS=", [&](const String& value) { rule.setBySetPos(value.split(',').map([](const String& s) { return ParseInt<int32>(s); })); } },
		};

		// マップをループして、行がどのプレフィックスで始まるかを確認
		for (const auto& [prefix, handler] : prefixHandlers)
		{
			if (icalRow.starts_with(prefix))
			{
				// プレフィックスを除去して値を取得
				String value = icalRow.substr(prefix.length());
				// 対応する処理を実行
				handler(value);
				break;
			}
		}

	}

	void RecurrenceRule::setFrequency(const String& frequency)
	{
		m_frequency = frequency;
	}

	void RecurrenceRule::setInterval(int32 interval)
	{
		m_interval = interval;
	}

	void RecurrenceRule::setCount(int32 count)
	{
		m_count = count;
	}

	void RecurrenceRule::setUntil(const DateTime& until)
	{
		m_until = until;
	}

	void RecurrenceRule::setStart(const DateTime& start)
	{
		m_start = start;
	}

	void RecurrenceRule::setBySecond(const Array<int32>& bySecond)
	{
		m_bySecond = bySecond;
	}

	void RecurrenceRule::setByMinute(const Array<int32>& byMinute)
	{
		m_byMinute = byMinute;
	}

	void RecurrenceRule::setByHour(const Array<int32>& byHour)
	{
		m_byHour = byHour;
	}

	void RecurrenceRule::setByDay(const Array<int32>& byDay)
	{
		m_byDay = byDay;
	}

	void RecurrenceRule::setByMonthDay(const Array<int32>& byMonthDay)
	{
		m_byMonthDay = byMonthDay;
	}

	void RecurrenceRule::setByYearDay(const Array<int32>& byYearDay)
	{
		m_byYearDay = byYearDay;
	}

	void RecurrenceRule::setByWeekNo(const Array<int32>& byWeekNo)
	{
		m_byWeekNo = byWeekNo;
	}

	void RecurrenceRule::setByMonth(const Array<int32>& byMonth)
	{
		m_byMonth = byMonth;
	}

	void RecurrenceRule::setBySetPos(const Array<int32>& bySetPos)
	{
		m_bySetPos = bySetPos;
	}

	void RecurrenceRule::setWeekStart(const DayOfWeek& weekStart)
	{
		m_weekStart = weekStart;
	}

	const String& RecurrenceRule::getFrequency() const
	{
		return m_frequency;
	}

	Optional<int32> RecurrenceRule::getInterval() const
	{
		return m_interval;
	}

	Optional<int32> RecurrenceRule::getCount() const
	{
		return m_count;
	}

	const Optional<DateTime>& RecurrenceRule::getUntil() const
	{
		return m_until;
	}

	const DateTime& RecurrenceRule::getStart() const
	{
		return m_start;
	}

	const Optional<Array<int32>>& RecurrenceRule::getBySecond() const
	{
		return m_bySecond;
	}

	const Optional<Array<int32>>& RecurrenceRule::getByMinute() const
	{
		return m_byMinute;
	}

	const Optional<Array<int32>>& RecurrenceRule::getByHour() const
	{
		return m_byHour;
	}

	const Optional<Array<int32>>& RecurrenceRule::getByDay() const
	{
		return m_byDay;
	}

	const Optional<Array<int32>>& RecurrenceRule::getByMonthDay() const
	{
		return m_byMonthDay;
	}

	const Optional<Array<int32>>& RecurrenceRule::getByYearDay() const
	{
		return m_byYearDay;
	}

	const Optional<Array<int32>>& RecurrenceRule::getByWeekNo() const
	{
		return m_byWeekNo;
	}

	const Optional<Array<int32>>& RecurrenceRule::getByMonth() const
	{
		return m_byMonth;
	}

	const Optional<Array<int32>>& RecurrenceRule::getBySetPos() const
	{
		return m_bySetPos;
	}

	const Optional<DayOfWeek>& RecurrenceRule::getWeekStart() const
	{
		return m_weekStart;
	}
}
