﻿# pragma once

namespace icalendar::actions
{
	class Action
	{
	public:
		Action();
		virtual ~Action();
		virtual void parseFromICS(const Array<String>& icsContent) = 0;
		void setDuration(const String& duration);
		void setRepeat(const String& repeat);
		String getDuration() const;
		String getRepeat() const;
	private:
		String m_duration;
		String m_repeat;
	};
}
