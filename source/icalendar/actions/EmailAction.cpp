# pragma once
# include "icalendar/actions/EmailAction.hpp"

namespace icalendar::actions
{
	EmailAction::EmailAction()
	{
	}

	EmailAction::~EmailAction()
	{
	}

	std::unique_ptr<Action> EmailAction::clone() const
	{
		auto clone = std::make_unique<EmailAction>();
		clone->setDuration(getDuration());
		clone->setRepeat(getRepeat());
		clone->setDescription(getDescription());
		clone->setSummary(getSummary());
		clone->setAttendee(getAttendee());
		clone->setAttachment(getAttachment().value_or(U""));
		return clone;
	}

	void EmailAction::parseFromICS(const Array<String>& icsContent)
	{
		HashTable<String, std::function<void(const String&)>> prefixHandlers = {
			{ U"DESCRIPTION", [&](const String& value) {
				setDescription(value);
			} },
			{ U"SUMMARY", [&](const String& value) {
				setSummary(value);
			} },
			{ U"ATTENDEE", [&](const String& value) {
				setAttendee(value);
			} },
			{ U"ATTACH", [&](const String& value) {
				setAttachment(value);
			} },
		};

		for (const auto& line : icsContent)
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

	void EmailAction::setDescription(const String& description)
	{
		m_description = description;
	}

	void EmailAction::setSummary(const String& summary)
	{
		m_summary = summary;
	}

	void EmailAction::setAttendee(const String& attendee)
	{
		m_attendee = attendee;
	}

	void EmailAction::setAttachment(const String& attachment)
	{
		m_attachment = attachment;
	}

	String EmailAction::getDescription() const
	{
		return m_description;
	}

	String EmailAction::getSummary() const
	{
		return m_summary;
	}

	String EmailAction::getAttendee() const
	{
		return m_attendee;
	}

	Optional<String> EmailAction::getAttachment() const
	{
		return m_attachment;
	}
}

