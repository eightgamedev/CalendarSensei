# pragma once
# include "icalendar/actions/Action.hpp"

namespace icalendar::actions
{
	class ProcedureAction : public Action
	{
		public:
		ProcedureAction();
		~ProcedureAction();
		virtual std::unique_ptr<Action> clone() const override;
		void parseFromICS(const Array<String>& icsContent);
		void setAttachment(const String& attachment);
		String getAttachment() const;
	private:
		String m_attachment;
	};
}
