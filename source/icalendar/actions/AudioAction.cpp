﻿# include "icalendar/actions/AudioAction.hpp"

namespace icalendar::actions
{
	AudioAction::AudioAction()
	{
	}

	AudioAction::~AudioAction()
	{
	}

	void AudioAction::parseFromICS(const Array<String>& icsContent)
	{
		for (const String& line : icsContent)
		{
			if (line.starts_with(U"ATTACH:"))
			{
				m_attachment = line.substr(7);
			}
		}
	}

	void AudioAction::setAttachment(const String& attachment)
	{
		m_attachment = attachment;
	}

	Optional<String> AudioAction::getAttachment() const
	{
		return m_attachment;
	}
}
