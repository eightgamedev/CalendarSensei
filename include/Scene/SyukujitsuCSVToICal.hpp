#pragma once
# include "Scene/Common.hpp"
# include "CustomizedSimpleGUI/Button.hpp"
# include "CustomizedSimpleGUI/Pulldown.hpp"
# include "icalendar/ICalendar.hpp"
# include <unicode/ucnv.h>
# include <unicode/ucsdet.h>
# include "s3dex/CSVEX.hpp"

class SyukujitsuCSVToICal : public App::Scene
{
public:
	SyukujitsuCSVToICal(const InitData& init);
	void update() override;
	void draw() const override;
private:
	[[nodiscard]] icalendar::Event PerseToEvent(const std::pair<String, String>& holiday) const;
	[[nodiscard]] icalendar::ICalendar makeICalendar(DateTime rangeBegin, DateTime rangeEnd) const;
	void saveICalendar(const icalendar::ICalendar& icalendar) const;
	void setPulldownItems(int32 beginInitialYear, int32 endInitialYear);
	void updatePulldowns();
	void drawPulldowns() const;
	CSV ReadCSV(const FilePath& path);
	Array<std::pair<String, String>> convertToholidayList(CSV csv);
	Font font{ 24 };

	Optional<Array<std::pair<String, String>>> holidayList;

	Optional<FilePath> inputFilePath;
	const ListBoxState inputFileCharacterSets;

	const double yearPulldownWidth = 100;
	const double monthPulldownWidth = 70;
	const double dayPulldownWidth = 70;
	const size_t pulldownListCount = 4;

	CSV japaneseHolidayCSV;

	CustomizedSimpleGUI::Button selectInputFileButton{ U"CSVを選択", 24, Vec2{ 100, 70 } };

	CustomizedSimpleGUI::Pulldown dateRangeBeginYearPulldown{ font, Vec2{ 0, 0 }, yearPulldownWidth, pulldownListCount };
	CustomizedSimpleGUI::Pulldown dateRangeBeginMonthPulldown{ font, Vec2{ yearPulldownWidth, 0 }, monthPulldownWidth, pulldownListCount };
	CustomizedSimpleGUI::Pulldown dateRangeBeginDayPulldown{ font, Vec2{ yearPulldownWidth + monthPulldownWidth, 0 }, dayPulldownWidth, pulldownListCount };

	CustomizedSimpleGUI::Pulldown dateRangeEndYearPulldown{ font, Vec2{ 0, 0 }, yearPulldownWidth, pulldownListCount };
	CustomizedSimpleGUI::Pulldown dateRangeEndMonthPulldown{ font, Vec2{ yearPulldownWidth, 0 }, monthPulldownWidth, pulldownListCount };
	CustomizedSimpleGUI::Pulldown dateRangeEndDayPulldown{ font, Vec2{ yearPulldownWidth + monthPulldownWidth, 0 }, dayPulldownWidth, pulldownListCount };
	bool isPulldownItemsSet = false;

	CustomizedSimpleGUI::Button convertButton{ U"icsファイルに変換", 24, Vec2{ 100, 400 } };
	CustomizedSimpleGUI::Button backToMenuButton{ U"メニューに戻る", 24, Vec2{ 0, 0 } };
};
