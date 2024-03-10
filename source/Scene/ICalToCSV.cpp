# include "Scene/ICalToCSV.hpp"

ICalToCSV::ICalToCSV(const InitData& init)
	: IScene(init)
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	Grid<String> values(columnCount, rowCount);

	m_spreadSheetGUI.setValues(values);
	m_spreadSheetGUI.setTextFont(Font(10));
}

void ICalToCSV::update()
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

	if (m_saveButton.isPushed())
	{
		const auto savePath = Dialog::SaveFile({ FileFilter::CSV() });
		if (savePath.has_value())
		{
			const String charset = m_charsetPulldown.getSelectedItem();
			s3dex::CSVEX::save(m_csv, savePath.value(), charset);
		}
	}

	if (m_backToMenuButton.isPushed())
	{
		changeScene(State::MainScene);
	}

	m_spreadSheetGUI.update();
	m_backToMenuButton.update();
	m_charsetPulldown.update();
	m_saveButton.update();
}

void ICalToCSV::draw() const
{
	m_backToMenuButton.draw();
	m_charsetPulldown.draw();
	m_saveButton.draw();
	m_spreadSheetGUI.draw();
}

CSV ICalToCSV::convertICalToCSV(const icalendar::ICalendar& icalendar) const
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

Grid<String> ICalToCSV::convertCSVToGrid(const CSV& csv) const
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
