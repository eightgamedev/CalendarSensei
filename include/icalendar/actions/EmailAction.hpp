# pragma once
# include "icalendar/actions/Action.hpp"

namespace icalendar::actions
{
	class EmailAction : public Action
	{
	public:
		EmailAction();
		~EmailAction();
		virtual std::unique_ptr<Action> clone() const override;
		void parseFromICS(const Array<String>& icsContent);
		void setDescription(const String& description);
		void setSummary(const String& summary);
		void setAttendee(const String& attendee);
		void setAttachment(const String& attachment);
		String getDescription() const;
		String getSummary() const;
		String getAttendee() const;
		Optional<String> getAttachment() const;
	private:
		String m_description;
		String m_summary;
		String m_attendee;
		Optional<String> m_attachment;
	};
}
