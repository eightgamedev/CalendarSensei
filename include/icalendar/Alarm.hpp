# pragma once
# include "icalendar/actions/Action.hpp"
# include "icalendar/actions/AudioAction.hpp"
# include "icalendar/actions/DisplayAction.hpp"
# include "icalendar/actions/EmailAction.hpp"
# include "icalendar/actions/ProcedureAction.hpp"

namespace icalendar
{
	class Alarm
	{
	public:
		Alarm();
		~Alarm();
		Alarm(const Alarm& other);
		Alarm& operator=(const Alarm& other);
		static Alarm parseFromICS(const Array<String>& icsContent);
		void setAction(std::unique_ptr<actions::Action> action);
		void setDescription(const String& description);
		void setTrigger(const String& trigger);
		actions::Action* getAction() const;
		String getDescription() const;
		String getTrigger() const;
	private:
		std::unique_ptr<actions::Action> m_action;
		String m_description;
		String m_trigger;
	};
}
