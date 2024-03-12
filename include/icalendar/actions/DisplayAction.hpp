# pragma once
# include "icalendar/actions/Action.hpp"

namespace icalendar::actions
{
	class DisplayAction : public Action
	{
	public:
		DisplayAction();
		~DisplayAction();
		void parseFromICS(const Array<String>& icsContent);
		void setDescription(const String& description);
		String getDescription() const;
	private:
		String m_description;
	};
}
