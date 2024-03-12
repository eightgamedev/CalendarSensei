# pragma once
# include "icalendar/actions/Action.hpp"

namespace icalendar::actions
{
	class ProcedureAction : public Action
	{
		public:
		ProcedureAction();
		~ProcedureAction();
		void parseFromICS(const Array<String>& icsContent);
		void setAttachment(const String& attachment);
		String getAttachment() const;
	private:
		String m_attachment;
	};
}
