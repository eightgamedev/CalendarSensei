# include "icalendar/actions/Action.hpp"

namespace icalendar::actions
{
	Action::Action()
	{
	}

	Action::~Action()
	{
	}

	void Action::setDuration(const String& duration)
	{
		m_duration = duration;
	}

	void Action::setRepeat(const String& repeat)
	{
		m_repeat = repeat;
	}

	String Action::getDuration() const
	{
		return m_duration;
	}

	String Action::getRepeat() const
	{
		return m_repeat;
	}
}
