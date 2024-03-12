# include "icalendar/Alarm.hpp"

namespace icalendar
{
	Alarm::Alarm()
	{
	}

	Alarm::~Alarm()
	{
	}

	void Alarm::parseFromICS(const Array<String>& icsContent)
	{
		HashTable<String, std::function<void(const String&)>> prefixHandlers = {
			{ U"ACTION", [&](const String& value) {
				if (value == U"AUDIO")
				{
					setAction(std::make_unique<actions::AudioAction>());
				}
				else if (value == U"DISPLAY")
				{
					setAction(std::make_unique<actions::DisplayAction>());
				}
				else if (value == U"EMAIL")
				{
					setAction(std::make_unique<actions::EmailAction>());
				}
				else if (value == U"PROCEDURE")
				{
					setAction(std::make_unique<actions::ProcedureAction>());
				}
			} },

			{ U"DESCRIPTION", [&](const String& value) {
				setDescription(value);
			} },
			{ U"TRIGGER", [&](const String& value) {
				setTrigger(value);
			} },
		};

		for (const String& line : icsContent)
		{
			size_t colonIndex = line.indexOf(U':');
			auto prefix = line.substr(0, colonIndex);
			auto value = line.substr(colonIndex + 1);
			if (prefixHandlers.contains(prefix))
			{
				prefixHandlers[prefix](value);
			}
		}
	}

	void Alarm::setAction(std::unique_ptr<actions::Action> action)
	{
		m_action = std::move(action);
	}

	void Alarm::setDescription(const String& description)
	{
		m_description = description;
	}

	void Alarm::setTrigger(const String& trigger)
	{
		m_trigger = trigger;
	}

	actions::Action* Alarm::getAction() const
	{
		return m_action.get();
	}

	String Alarm::getDescription() const
	{
		return m_description;
	}

	String Alarm::getTrigger() const
	{
		return m_trigger;
	}
}
