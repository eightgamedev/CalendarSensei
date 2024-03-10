# include "icalendar/CalendarProperty.hpp"

namespace icalendar
{
	CalendarProperty::CalendarProperty()
	{
	}

	CalendarProperty::~CalendarProperty()
	{
	}

	void CalendarProperty::setVersion(const String& version)
	{
		m_version = version;
	}

	void CalendarProperty::setProdId(const String& prodId)
	{
		m_prodId = prodId;
	}

	void CalendarProperty::setCalscale(const String& calscale)
	{
		m_calscale = calscale;
	}

	void CalendarProperty::setMethod(const String& method)
	{
		m_method = method;
	}

	void CalendarProperty::setGoogle_CALNAME(const String& google_CALNAME)
	{
		m_google_CALNAME = google_CALNAME;
	}

	void CalendarProperty::setGoogle_CALDESC(const String& google_CALDESC)
	{
		m_google_CALDESC = google_CALDESC;
	}

	void CalendarProperty::setGoogle_TIMEZONE(const String& google_TIMEZONE)
	{
		m_google_TIMEZONE = google_TIMEZONE;
	}

	String CalendarProperty::getVersion() const
	{
		return m_version;
	}

	String CalendarProperty::getProdId() const
	{
		return m_prodId;
	}

	String CalendarProperty::getCalscale() const
	{
		return m_calscale;
	}

	String CalendarProperty::getMethod() const
	{
		return m_method;
	}

	Optional<String> CalendarProperty::getGoogle_CALNAME() const
	{
		return m_google_CALNAME;
	}

	Optional<String> CalendarProperty::getGoogle_CALDESC() const
	{
		return m_google_CALDESC;
	}

	Optional<String> CalendarProperty::getGoogle_TIMEZONE() const
	{
		return m_google_TIMEZONE;
	}

	

}
