# pragma once

namespace icalendar
{
	class CalendarProperty
	{
	public:
		CalendarProperty();
		~CalendarProperty();
		static CalendarProperty parseFromICS(const Array<String>& icsContent);
		void setVersion(const String& version);
		void setProdId(const String& prodId);
		void setCalscale(const String& calscale);
		void setMethod(const String& method);
		void setGoogle_CALNAME(const String& google_CALNAME);
		void setGoogle_CALDESC(const String& google_CALDESC);
		void setGoogle_TIMEZONE(const String& google_TIMEZONE);
		String getVersion() const;
		String getProdId() const;
		String getCalscale() const;
		String getMethod() const;
		Optional<String> getGoogle_CALNAME() const;
		Optional<String> getGoogle_CALDESC() const;
		Optional<String> getGoogle_TIMEZONE() const;
	private:
		// バージョン
		String m_version;

		// プロダクトID (ex. "-//Siv3D//Siv3D Calendar//JP")
		String m_prodId;

		// 暦法 (ex. "GREGORIAN")
		String m_calscale;

		String m_method;

		Optional<String> m_google_CALNAME;

		Optional<String> m_google_CALDESC;

		Optional<String> m_google_TIMEZONE;
	};
}
