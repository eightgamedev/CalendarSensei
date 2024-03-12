# pragma once
# include "icalendar/actions/Action.hpp"

namespace icalendar::actions
{
	class AudioAction : public Action
	{
	public:
		AudioAction();
		~AudioAction();
		void parseFromICS(const Array<String>& icsContent);
		void setAttachment(const String& attachment);
		Optional<String> getAttachment() const;
	private:
		Optional<String> m_attachment;
	};
}
