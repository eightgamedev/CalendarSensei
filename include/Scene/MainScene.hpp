#pragma once
# include "Scene/Common.hpp"
# include "CustomizedSimpleGUI/Button.hpp"
# include "CustomizedSimpleGUI/Pulldown.hpp"
# include "icalendar/ICalendar.hpp"
# include "s3dex/CSVEX.hpp"
# include "SimpleGridViewer/SpreadSheet.hpp"

class MainScene : public App::Scene
{
public:
	MainScene(const InitData& init);
	void update() override;
	void draw() const override;
private:
	void drawCalendarProperty(const icalendar::ICalendar& icalendar) const;
	CSV convertICalToCSV(const icalendar::ICalendar& icalendar) const;
	Grid<String> convertCSVToGrid(const CSV& csv) const;

	Optional<FilePath> m_inputFilePath;
	icalendar::ICalendar m_icalendar;
	CSV m_csv;
	ListBoxState m_charsets{ { U"UTF-8", U"Shift_JIS" } };
	Font m_font{ 24 };
	CustomizedSimpleGUI::Pulldown m_charsetPulldown{ m_charsets, m_font, Vec2{ 100, 200 }, 300.0, 2 };
	size_t columnCount = 100;
	size_t rowCount = 100;

	SimpleGridViewer::SpreadSheet m_spreadSheetGUI{ {columnCount, rowCount }, { 26, 30 }, { 0, 0 } };
	const Array<std::pair<String, Array<String>>> m_menus
	{
		{ U"ファイル", { U"新規", U"インポート", U"エクスポート", U"終了"}},
		{ U"ヘルプ", { U"\U000F0625  遊び方", U"\U000F14F7  リリースノート", U"ライセンス" }},
	};

	SimpleMenuBar m_menuBar{ m_menus };
};
