# include "icalendar/actions/DisplayAction.hpp"

namespace icalendar::actions
{
	DisplayAction::DisplayAction()
	{
	}

	DisplayAction::~DisplayAction()
	{
	}

	std::unique_ptr<Action> DisplayAction::clone() const
	{
		return std::make_unique<DisplayAction>(*this);
	}

	void DisplayAction::parseFromICS(const Array<String>& icsContent)
	{
		HashTable<String, std::function<void(const String&)>> prefixHandlers = {
			{ U"DURATION", [&](const String& value) {
				setDuration(value);
			} },
			{ U"REPEAT", [&](const String& value) {
				setRepeat(value);
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

	void DisplayAction::setDescription(const String& description)
	{
		m_description = description;
	}

	String DisplayAction::getDescription() const
	{
		return m_description;
	}
}
