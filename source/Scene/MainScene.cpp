﻿# include "Scene/MainScene.hpp"

MainScene::MainScene(const InitData& init)
	: IScene(init)
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	Grid<String> values(columnCount, rowCount);

	m_spreadSheet.setColumnNames({ U"Subject", U"Start Date", U"Start Time", U"End Date", U"End Time", U"All Day Event", U"Description", U"Location", U"Private" });
	m_spreadSheet.setValues(values);
	m_spreadSheet.setIndexFont(Font(10));
	m_spreadSheet.setTextFont(Font(10));
}

void MainScene::update()
{
	//if (DragDrop::HasNewFilePaths())
	//{
	//	for (const auto& dropped : DragDrop::GetDroppedFilePaths())
	//	{
	//		if (dropped.path.includes(U".ics"))
	//		{
	//			m_inputFilePath = dropped.path;
	//			m_icalendar.load(m_inputFilePath.value());
	//			m_csv = convertICalToCSV(m_icalendar);
	//			m_spreadSheetGUI.setValues(convertCSVToGrid(m_csv));
	//		}
	//	}
	//}

	if (m_inputFilePath.has_value())
	{
		if (m_inputFilePath.value().includes(U".ics")
		 || m_inputFilePath.value().includes(U".ical")
		 || m_inputFilePath.value().includes(U".icalendar"))
		{
			m_icalendar.load(m_inputFilePath.value());
			m_csv = convertICalToCSV(m_icalendar);
			m_spreadSheet.setValues(convertICalToGrid(m_icalendar));
			m_inputFilePath.reset();
			m_calendarPropertyNode = createCalendarPropertyNode(m_icalendar.getCalendarProperty());
			m_eventNode = createEventNode(m_icalendar.getEvents().front());
		}
	}

	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(0, 300), TransformCursor::Yes);
		m_spreadSheet.update();
	}

	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(1200, 50), TransformCursor::Yes);
		if (m_calendarPropertyNode)
		{
			m_calendarPropertyNode->update({ 0, 0 });
		}
	}

	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(1200, 300), TransformCursor::Yes);
		if (m_eventNode)
		{
			m_eventNode->update({ 0, 0 });
		}
	}

}

void MainScene::draw() const
{
	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(0, 300), TransformCursor::Yes);
		m_spreadSheet.draw();
	}

	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(1200, 50), TransformCursor::Yes);
		if (m_calendarPropertyNode)
		{
			m_calendarPropertyNode->draw();
		}
	}

	drawToolWindow();
	drawImportICalWindow();
	drawExportCSVWindow();

	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(1200, 300), TransformCursor::Yes);
		if (m_eventNode)
		{
			m_eventNode->draw();
		}
	}
}

void MainScene::drawToolWindow() const
{
	if (m_openingWindow != OpeningWindow::None)
	{
		return;
	}

	gui.frameBegin();
	gui.windowBegin(U"", SasaGUI::WindowFlag::NoMove | SasaGUI::WindowFlag::NoTitlebar);
	{
		gui.setWindowSize({ 600, 250 });
		switch (gui.tab(U"ToolMenu", { U"インポート", U"エクスポート", U"タイムゾーン"}))
		{
		case 0:
			if (gui.button(U".ics"))
			{
				m_openingWindow = OpeningWindow::ImportICal;
			}
			gui.button(U".csv");
			gui.button(U".xlsx");
			gui.button(U"syukujitsu.csv(shukujitsu.csv)");
			break;
		case 1:
			gui.button(U".ics");
			if (gui.button(U".csv"))
			{
				m_openingWindow = OpeningWindow::ExportCSV;
			}
			gui.button(U".xlsx");
			break;
		case 2:
			gui.label(U"時刻の数値はそのまま、タイムゾーンのみ変更");
			gui.button(U"JST -> UTC");
			gui.sameLine();
			gui.button(U"UTC -> JST");

			gui.label(U"タイムゾーンはそのまま、時刻の数値のみ変更");
			gui.button(U"-1時間");
			gui.sameLine();
			gui.button(U"+1時間");
			break;
		}
	}
	gui.windowEnd();
	gui.frameEnd();
}

void MainScene::drawImportICalWindow() const
{
	if (m_openingWindow != OpeningWindow::ImportICal)
	{
		return;
	}

	gui.frameBegin();
	gui.windowBegin(U"iCalファイルをインポート", SasaGUI::WindowFlag::NoMove | SasaGUI::WindowFlag::AutoResize);
	{
		gui.setWindowPos(Arg::center = Scene::Center());
		gui.setWindowSize(Size(600, 450));
		gui.label(U"ファイルを選択してください");
		if (gui.button(U"キャンセル"))
		{
			m_openingWindow = OpeningWindow::None;
		}
		gui.sameLine();
		if (gui.button(U"ファイルを選択"))
		{
			m_inputFilePath = Dialog::OpenFile({ FileFilter{ U"iCalendar", { U"ics", U"ical", U"icalendar" }}});
			m_openingWindow = OpeningWindow::None;
		}
	}
	gui.windowEnd();
	gui.frameEnd();
}

void MainScene::drawExportCSVWindow() const
{
	if (m_openingWindow != OpeningWindow::ExportCSV)
	{
		return;
	}

	gui.frameBegin();
	gui.windowBegin(U"CSVとしてエクスポート", SasaGUI::WindowFlag::NoMove | SasaGUI::WindowFlag::AutoResize);
	{
		gui.setWindowPos(Arg::center = Scene::Center());
		gui.setWindowSize(Size(600, 500));
		
		gui.label(U"文字コード");
		gui.radiobutton(m_exportCSVCharsetIndex, 0, U"UTF-8");
		gui.radiobutton(m_exportCSVCharsetIndex, 1, U"Shift-JIS");
		gui.label(U"");

		gui.label(U"日付のフォーマット");
		gui.radiobutton(m_dateFormatIndex, 0, U"yyyy/MM/dd");
		gui.radiobutton(m_dateFormatIndex, 1, U"yyyy/M/d");
		gui.label(U"");

		gui.label(U"時刻のフォーマット");
		gui.radiobutton(m_timeFormatIndex, 0, U"HH:mm");
		gui.radiobutton(m_timeFormatIndex, 1, U"HH時mm分");
		gui.label(U"");

		if (gui.button(U"キャンセル"))
		{
			m_openingWindow = OpeningWindow::None;
		}
		gui.sameLine();
		if (gui.button(U"エクスポート"))
		{
			m_openingWindow = OpeningWindow::None;
		}

	}
	gui.windowEnd();
	gui.frameEnd();
}

void MainScene::drawCalendarProperty(const icalendar::ICalendar& icalendar) const
{	
	const auto& calendarProperty = icalendar.getCalendarProperty();
	const Color color = Palette::Black;

	m_font(U"Calendar Property").draw(0, 10, Palette::Black);
	m_font(U"Version: {}"_fmt(calendarProperty.getVersion())).draw(0, 30, color);
	m_font(U"Product ID: {}"_fmt(calendarProperty.getProdId())).draw(0, 50, color);
	m_font(U"Scale: {}"_fmt(calendarProperty.getCalscale())).draw(0, 70, color);
	m_font(U"Method: {}"_fmt(calendarProperty.getMethod())).draw(0, 90, color);
}

CSV MainScene::convertICalToCSV(const icalendar::ICalendar& icalendar) const
{
	CSV csv;
	csv.writeRow(
		U"Subject",
		U"Start Date",
		U"Start Time",
		U"End Date",
		U"End Time",
		U"All Day Event",
		U"Description",
		U"Location",
		U"Private");
	for (const auto& event : icalendar.getEvents())
	{
		csv.writeRow(
			event.getSummary(),
			event.getDateTimeStart().format(U"yyyy/MM/dd"),
			event.isAllDay() ? U"" : event.getDateTimeStart().format(U"HH:mm"),
			event.getDateTimeEnd().format(U"yyyy/MM/dd"),
			event.isAllDay() ? U"" : event.getDateTimeEnd().format(U"HH:mm"),
			event.isAllDay() ? U"True" : U"False",
			event.getDescription().has_value() ? event.getDescription().value() : U"",
			event.getLocation().has_value() ? event.getLocation().value() : U"",
			event.getClass().value_or(U"True")
		);
	}
	return csv;
}

Grid<String> MainScene::convertCSVToGrid(const CSV& csv) const
{
	Grid<String> grid{ csv.columns(0), csv.rows() };
	for (size_t row = 0; row < csv.rows(); ++row)
	{
		for (size_t column = 0; column < csv.columns(0); ++column)
		{
			grid[row][column] = csv.get(row, column);
		}
	}
	return grid;
}

Grid<String> MainScene::convertICalToGrid(const icalendar::ICalendar& icalendar) const
{
	Grid<String> grid{ 9, icalendar.getEvents().size() };

	size_t row = 0;
	for (const auto& event : icalendar.getEvents())
	{
		grid[row][0] = event.getSummary();
		grid[row][1] = event.getDateTimeStart().format(U"yyyy/MM/dd");
		grid[row][2] = event.isAllDay() ? U"" : event.getDateTimeStart().format(U"HH:mm");
		grid[row][3] = event.getDateTimeEnd().format(U"yyyy/MM/dd");
		grid[row][4] = event.isAllDay() ? U"" : event.getDateTimeEnd().format(U"HH:mm");
		grid[row][5] = event.isAllDay() ? U"True" : U"False";
		grid[row][6] = event.getDescription().has_value() ? event.getDescription().value() : U"";
		grid[row][7] = event.getLocation().has_value() ? event.getLocation().value() : U"";
		grid[row][8] = event.getClass().value_or(U"");
		++row;
	}
	return grid;
}

std::shared_ptr<TreeGUI::Node> MainScene::createCalendarPropertyNode(const icalendar::CalendarProperty& calendarProperty) const
{
	std::shared_ptr<TreeGUI::Node> node = std::make_shared<TreeGUI::Node>(U"Calendar Property");
	node->addChild(U"Version", U"Version: {}"_fmt(calendarProperty.getVersion()));
	node->addChild(U"Product ID", U"Product ID: {}"_fmt(calendarProperty.getProdId()));
	node->addChild(U"Scale", U"Scale: {}"_fmt(calendarProperty.getCalscale()));
	node->addChild(U"Method", U"Method: {}"_fmt(calendarProperty.getMethod()));
	return node;
}

std::shared_ptr<TreeGUI::Node> MainScene::createEventNode(const icalendar::Event& event) const
{
	std::shared_ptr<TreeGUI::Node> node = std::make_shared<TreeGUI::Node> (event.getSummary());
	node->addChild(U"StartDateTime", U"Start DateTime: {}"_fmt(event.getDateTimeStart().format(U"yyyy/MM/dd HH:mm")));
	node->addChild(U"EndDateTime", U"End DateTime: {}"_fmt(event.getDateTimeEnd().format(U"yyyy/MM/dd HH:mm")));
	node->addChild(U"AllDayEvent", U"All Day Event: {}"_fmt(event.isAllDay() ? U"true" : U"false"));
	node->addChild(U"Description", U"Description: {}"_fmt(event.getDescription().value_or(U"null")));
	node->addChild(U"Location", U"Location: {}"_fmt(event.getLocation().value_or(U"null")));
	node->addChild(U"TimeStamp", U"TimeStamp: {}"_fmt(event.getTimeStamp().format(U"yyyy/MM/dd HH:mm")));
	node->addChild(U"UniqueID", U"UniqueID: {}"_fmt(event.getUniqueID().value_or(U"")));
	node->addChild(U"CreatedDateTime", U"Created DateTime: {}"_fmt(event.getCreated().has_value() ? event.getCreated().value().format(U"yyyy/MM/dd HH:mm") : U""));
	node->addChild(U"Last Modified DateTime: {}"_fmt(event.getLastModified().has_value() ? event.getLastModified().value().format(U"yyyy/MM/dd HH:mm") : U""));
	node->addChild(U"Sequence: {}"_fmt(event.getSequence().value_or(U"null")));
	node->addChild(U"Status: {}"_fmt(event.getStatus().value_or(U"null")));
	node->addChild(U"Transparency: {}"_fmt(event.getTransparent().value_or(U"null")));
	node->addChild(U"Class: {}"_fmt(event.getClass().value_or(U"null")));

	node->addChild(U"Alarms", U"Alarms: ");
	for (const auto& alarm : event.getAlarms())
	{
		auto& alarmNode = node->operator[](U"Alarms");
		alarmNode->addChild(U"Action", U"Action:")
			->addChild(U"Duration", U"Duration: {}"_fmt(alarm->getAction()->getDuration()))->p(1)
			->addChild(U"Repeat", U"Repeat: {}"_fmt(alarm->getAction()->getRepeat()));

		alarmNode
			->addChild(U"Trigger", U"Trigger: {}"_fmt(alarm->getTrigger()))->p(1)
			->addChild(U"Description", U"Description: {}"_fmt(alarm->getDescription()));
	}

	return node;
}
