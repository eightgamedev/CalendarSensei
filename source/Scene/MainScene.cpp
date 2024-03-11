﻿# include "Scene/MainScene.hpp"

MainScene::MainScene(const InitData& init)
	: IScene(init)
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	Grid<String> values(columnCount, rowCount);

	m_spreadSheetGUI.setValues(values);
	m_spreadSheetGUI.setTextFont(Font(10));
}

void MainScene::update()
{
	if (DragDrop::HasNewFilePaths())
	{
		for (const auto& dropped : DragDrop::GetDroppedFilePaths())
		{
			if (dropped.path.includes(U".ics"))
			{
				m_inputFilePath = dropped.path;
				m_icalendar.load(m_inputFilePath.value());
				m_csv = convertICalToCSV(m_icalendar);
				m_spreadSheetGUI.setValues(convertCSVToGrid(m_csv));
			}
		}
	}

	{
		const Transformer2D t(Mat3x2::Scale(0.85).translated(0, 400), TransformCursor::Yes);
		m_spreadSheetGUI.update();
	}
}

void MainScene::draw() const
{
	{
		const Transformer2D t(Mat3x2::Scale(0.85).translated(0, 400), TransformCursor::Yes);
		m_spreadSheetGUI.draw();
	}

	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(1500, 50), TransformCursor::Yes);
		drawCalendarProperty(m_icalendar);
	}

	drawToolWindow();
	drawImportICalWindow();
	drawExportCSVWindow();
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
		switch (gui.tab(U"ToolMenu", { U"インポート", U"エクスポート" }))
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
		U"Reminder On/Off",
		U"Reminder Date",
		U"Reminder Time",
		U"Meeting Organizer",
		U"Description",
		U"Location",
		U"Private");
	for (const auto& event : icalendar.getEvents())
	{
		csv.writeRow(
			event.getSummary(),
			event.getStart().format(U"yyyy/MM/dd"),
			event.getStart().format(U"HH:mm"),
			event.getEnd().format(U"yyyy/MM/dd"),
			event.getEnd().format(U"HH:mm"),
			event.isAllDay() ? U"True" : U"False",
			U"False",
			U"",
			U"",
			U"",
			event.getDescription().has_value() ? event.getDescription().value() : U"",
			event.getLocation().has_value() ? event.getLocation().value() : U"",
			U"False"
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
