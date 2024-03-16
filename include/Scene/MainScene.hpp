#pragma once
# include "Scene/Common.hpp"
# include "CustomizedSimpleGUI/Button.hpp"
# include "CustomizedSimpleGUI/Pulldown.hpp"
# include "icalendar/ICalendar.hpp"
# include "s3dex/CSVEX.hpp"
# include "SimpleGridViewer/SpreadSheet.hpp"
# include "TreeGUI/Node.hpp"

class MainScene : public App::Scene
{
public:
	MainScene(const InitData& init);
	void update() override;
	void draw() const override;
private:
	void drawToolWindow() const;
	void drawImportICalWindow() const;
	void drawExportCSVWindow() const;
	void drawCalendarProperty(const icalendar::ICalendar& icalendar) const;
	CSV convertICalToCSV(const icalendar::ICalendar& icalendar) const;
	Grid<String> convertCSVToGrid(const CSV& csv) const;
	std::shared_ptr<TreeGUI::Node> createCalendarPropertyNode(const icalendar::CalendarProperty& calendarProperty) const;
	std::shared_ptr<TreeGUI::Node> createEventNode(const icalendar::Event& event) const;

	mutable Optional<FilePath> m_inputFilePath;
	icalendar::ICalendar m_icalendar;
	CSV m_csv;
	Font m_font{ 24 };
	size_t columnCount = 100;
	size_t rowCount = 100;

	SimpleGridViewer::SpreadSheet m_spreadSheetGUI{ {columnCount, rowCount }, { 13, 30 }, { 0, 0 } };

	mutable SasaGUI::GUIManager gui;
	Array<std::shared_ptr<TreeGUI::Node>> m_eventNodes;
	std::shared_ptr<TreeGUI::Node> m_calendarPropertyNode = nullptr;

	enum class OpeningWindow
	{
		None,
		ImportICal,
		ExportCSV
	};

	mutable OpeningWindow m_openingWindow = OpeningWindow::None;

	mutable int32 m_exportCSVCharsetIndex = 0;
	mutable int32 m_dateFormatIndex = 0;
	mutable int32 m_timeFormatIndex = 0;
};
