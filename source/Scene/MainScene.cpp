# include "Scene/MainScene.hpp"

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


	if (const auto item = m_menuBar.update())
	{
		// 「インポート」が押されたら
		if (item == MenuBarItemIndex{ 0, 1 })
		{

		}

		// 「エクスポート」が押されたら
		if (item == MenuBarItemIndex{ 0, 2 })
		{
			const auto savePath = Dialog::SaveFile({ FileFilter::CSV() });
			if (savePath.has_value())
			{
				const String charset = m_charsetPulldown.getSelectedItem();
				s3dex::CSVEX::save(m_csv, savePath.value(), charset);
			}
		}

		// 「ライセンス」が押されたら
		if (item == MenuBarItemIndex{ 1, 2 })
		{
			LicenseManager::ShowInBrowser();
		}
	}

	{
		const Transformer2D t(Mat3x2::Scale(0.85).translated(0, 400), TransformCursor::Yes);
		m_spreadSheetGUI.update();
	}
	m_charsetPulldown.update();
	m_menuBar.update();
}

void MainScene::draw() const
{
	m_charsetPulldown.draw();
	{
		const Transformer2D t(Mat3x2::Scale(0.85).translated(0, 400), TransformCursor::Yes);
		m_spreadSheetGUI.draw();
	}

	{
		const Transformer2D t(Mat3x2::Scale(1.0).translated(1500, 50), TransformCursor::Yes);
		drawCalendarProperty(m_icalendar);
	}

	m_menuBar.draw();

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
