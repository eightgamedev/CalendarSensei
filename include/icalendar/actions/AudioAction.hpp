# pragma once
# include "icalendar/actions/Action.hpp"

namespace icalendar::actions
{
	class AudioAction : public Action
	{
	public:
		AudioAction();
		~AudioAction();
		virtual std::unique_ptr<Action> clone() const override;
		void parseFromICS(const Array<String>& icsContent);
		void setAttachment(const String& attachment);
		Optional<String> getAttachment() const;
	private:
		Optional<String> m_attachment;
	};
}
