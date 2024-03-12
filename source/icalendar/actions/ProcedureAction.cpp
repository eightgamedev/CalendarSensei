# include "icalendar/actions/ProcedureAction.hpp"

namespace icalendar::actions
{
	ProcedureAction::ProcedureAction()
	{
	}

	ProcedureAction::~ProcedureAction()
	{
	}

	std::unique_ptr<Action> ProcedureAction::clone() const
	{
		return std::make_unique<ProcedureAction>(*this);
	}

	void ProcedureAction::parseFromICS(const Array<String>& icsContent)
	{
		for (const String& line : icsContent)
		{
			if (line.starts_with(U"ATTACH:"))
			{
				m_attachment = line.substr(7);
			}
		}
	}

	void ProcedureAction::setAttachment(const String& attachment)
	{
		m_attachment = attachment;
	}

	String ProcedureAction::getAttachment() const
	{
		return m_attachment;
	}
}
