# include "Scene/SyukujitsuCSVToICal.hpp"

SyukujitsuCSVToICal::SyukujitsuCSVToICal(const InitData& init)
	: IScene(init)
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
}

void SyukujitsuCSVToICal::update()
{
	if (DragDrop::HasNewFilePaths())
	{
		for (const auto& dropped : DragDrop::GetDroppedFilePaths())
		{
			if (dropped.path.includes(U".csv") || dropped.path.includes(U".CSV"))
			{
				inputFilePath = dropped.path;
				japaneseHolidayCSV = s3dex::CSVEX::open(inputFilePath.value());
				// japaneseHolidayCSV = ReadCSV(inputFilePath.value());
				holidayList = convertToholidayList(japaneseHolidayCSV);
			}
		}
	}

	if (selectInputFileButton.isPushed())
	{
		inputFilePath = Dialog::OpenFile({ FileFilter::CSV() });
		if (inputFilePath.has_value())
		{
			japaneseHolidayCSV = s3dex::CSVEX::open(inputFilePath.value());
			holidayList = convertToholidayList(japaneseHolidayCSV);
		}
	}

	if (convertButton.isPushed() && holidayList.has_value())
	{
		DateTime rangeBegin = DateTime{ Parse<int32>(dateRangeBeginYearPulldown.getSelectedItem()), Parse<int32>(dateRangeBeginMonthPulldown.getSelectedItem()), Parse<int32>(dateRangeBeginDayPulldown.getSelectedItem()) };
		DateTime rangeEnd = DateTime{ Parse<int32>(dateRangeEndYearPulldown.getSelectedItem()), Parse<int32>(dateRangeEndMonthPulldown.getSelectedItem()), Parse<int32>(dateRangeEndDayPulldown.getSelectedItem()) };
		icalendar::ICalendar icalendar = makeICalendar(rangeBegin, rangeEnd);
		saveICalendar(icalendar);
	}

	selectInputFileButton.update();
	convertButton.update();
	if (holidayList.has_value() && not isPulldownItemsSet)
	{
		int32 beginInitialYear = Parse<int32>(holidayList->front().first.split(U'/')[0]);
		int32 endInitialYear = Parse<int32>(holidayList->back().first.split(U'/')[0]);
		setPulldownItems(beginInitialYear, endInitialYear);
		isPulldownItemsSet = true;
	}

	updatePulldowns();

	backToMenuButton.update();

	if (backToMenuButton.isPushed())
	{
		changeScene(State::MainScene);
	}
}

void SyukujitsuCSVToICal::draw() const
{
	if (inputFilePath.has_value())
	{
		font(*inputFilePath).draw(100, 110, Palette::Black);
	}
	selectInputFileButton.draw();
	convertButton.draw();
	drawPulldowns();
	backToMenuButton.draw();
}

icalendar::Event SyukujitsuCSVToICal::PerseToEvent(const std::pair<String, String>& holiday) const
{
	icalendar::Event event;
	event.setIsAllDay(true);

	Array<String> dateTimeStrings = holiday.first.split(U'/'); // ex) 2021/01/01 -> { 2021, 1, 1 }

	const int32 year = Parse<int32>(dateTimeStrings[0]);
	const int32 month = Parse<int32>(dateTimeStrings[1]);
	const int32 day = Parse<int32>(dateTimeStrings[2]);

	DateTime startDateTime{ year, month, day };
	event.setStart(startDateTime);
	event.setEnd(startDateTime + 1_d);
	event.setSummary(holiday.second);
	event.setTimeStamp(DateTime::Now());
	return event;
}

icalendar::ICalendar SyukujitsuCSVToICal::makeICalendar(DateTime rangeBegin, DateTime rangeEnd) const
{
	icalendar::ICalendar icalendar;
	for (const auto& holiday : *holidayList)
	{
		icalendar::Event event = PerseToEvent(holiday);
		if (rangeBegin <= event.getStart() && event.getStart() <= rangeEnd)
		{
			icalendar.addEvent(event);
		}
	}
	return icalendar;
}

void SyukujitsuCSVToICal::saveICalendar(const icalendar::ICalendar& icalendar) const
{
	const auto result = Dialog::SaveFile({ FileFilter{ U"iCalendar", {U"ics"} } }, U"", U"iCalendarファイルの保存");
	if (result)
	{
		icalendar.save(result.value());
	}
}

void SyukujitsuCSVToICal::setPulldownItems(int32 beginInitialYear, int32 endInitialYear)
{
	Array<String> years;
	for (int32 i = beginInitialYear; i <= endInitialYear; ++i)
	{
		years.emplace_back(Format(i));
	}

	Array<String> months;
	for (int32 i = 1; i <= 12; ++i)
	{
		months.emplace_back(Format(i));
	}

	Array<String> days;
	for (int32 i = 1; i <= 31; ++i)
	{
		days.emplace_back(Format(i));
	}

	dateRangeBeginYearPulldown.setItems(years);
	dateRangeBeginMonthPulldown.setItems(months);
	dateRangeBeginDayPulldown.setItems(days);

	dateRangeEndYearPulldown.setItems(years);
	dateRangeEndMonthPulldown.setItems(months);
	dateRangeEndDayPulldown.setItems(days);

	dateRangeEndYearPulldown.setIndex(dateRangeEndYearPulldown.getItemsCount() - 1);
	dateRangeEndMonthPulldown.setIndex(dateRangeEndMonthPulldown.getItemsCount() - 1);
	dateRangeEndDayPulldown.setIndex(dateRangeEndDayPulldown.getItemsCount() - 1);
}

void SyukujitsuCSVToICal::updatePulldowns()
{
	const int32 beginYear = dateRangeBeginYearPulldown.getSelectedIndex();
	const int32 beginMonth = dateRangeBeginMonthPulldown.getSelectedIndex();
	const int32 beginDay = dateRangeBeginDayPulldown.getSelectedIndex();

	const int32 endYear = dateRangeEndYearPulldown.getSelectedIndex();
	const int32 endMonth = dateRangeEndMonthPulldown.getSelectedIndex();
	const int32 endDay = dateRangeEndDayPulldown.getSelectedIndex();

	if (beginYear > endYear)
	{
		dateRangeEndYearPulldown.setIndex(beginYear);
	}
	else if (beginYear == endYear)
	{
		if (beginMonth > endMonth)
		{
			dateRangeEndMonthPulldown.setIndex(beginMonth);
		}
		else if (beginMonth == endMonth)
		{
			if (beginDay > endDay)
			{
				dateRangeEndDayPulldown.setIndex(beginDay);
			}
		}
	}

	{
		const Transformer2D beginPulldown{ Mat3x2::Translate(100, 200), TransformCursor::Yes };
		dateRangeBeginYearPulldown.update();
		dateRangeBeginMonthPulldown.update();
		dateRangeBeginDayPulldown.update();
	}

	{
		const Transformer2D endPulldown{ Mat3x2::Translate(500, 200), TransformCursor::Yes };
		dateRangeEndYearPulldown.update();
		dateRangeEndMonthPulldown.update();
		dateRangeEndDayPulldown.update();
	}
}

void SyukujitsuCSVToICal::drawPulldowns() const
{
	{
		const Transformer2D beginPulldown{ Mat3x2::Translate(100, 200), TransformCursor::Yes };
		dateRangeBeginYearPulldown.draw();
		dateRangeBeginMonthPulldown.draw();
		dateRangeBeginDayPulldown.draw();
	}

	{
		const Transformer2D endPulldown{ Mat3x2::Translate(500, 200), TransformCursor::Yes };
		dateRangeEndYearPulldown.draw();
		dateRangeEndMonthPulldown.draw();
		dateRangeEndDayPulldown.draw();
	}
}

CSV SyukujitsuCSVToICal::ReadCSV(const FilePath& path)
{
	// ファイルをバイナリモードで読み込む
	BinaryReader reader(path);
	if (!reader)
	{
		// ファイルのオープンに失敗
		return CSV();
	}

	// ファイルのサイズを取得
	const size_t fileSize = reader.size();

	// ファイルの内容をバイト列として読み込む
	std::string bytes(fileSize, '\0');
	reader.read(bytes.data(), fileSize);

	// ICUの文字コード検出器を開く
	UErrorCode status = U_ZERO_ERROR;
	UCharsetDetector* csd = ucsdet_open(&status);

	// バイト列を文字コード検出器に設定する
	ucsdet_setText(csd, reinterpret_cast<const char*>(bytes.data()), bytes.size(), &status);

	// 最も可能性の高い文字コードを検出する
	const UCharsetMatch* match = ucsdet_detect(csd, &status);
	const char* name = ucsdet_getName(match, &status);

	// 文字コード検出器を閉じる
	ucsdet_close(csd);

	// 文字コードの変換
	std::string convertedBytes(fileSize * 2, '\0'); // 変換後のバイト列の最大サイズを確保
	UErrorCode convertStatus = U_ZERO_ERROR;
	int32_t convertedSize = ucnv_convert("UTF-8", name, convertedBytes.data(), convertedBytes.size(), bytes.data(), bytes.size(), &convertStatus);
	if (U_FAILURE(convertStatus))
	{
		// 変換に失敗
		return CSV();
	}

	// バイト列を文字列に変換
	const std::string str(convertedBytes.begin(), convertedBytes.begin() + convertedSize);
	const String text = Unicode::FromUTF8(str);
	CSV csv;
	for (const auto& line : text.split(U'\n'))
	{
		if (line.isEmpty()) // 空行を無視
		{
			continue;
		}
		for (auto& column : line.split(U','))
		{
			column.trim();
			csv.write(column);
		}
		csv.newLine();
	}
	return csv;
}

Array<std::pair<String, String>> SyukujitsuCSVToICal::convertToholidayList(CSV csv)
{
	Array<std::pair<String, String>> holidayList;
	for (size_t row = 1; row < csv.rows(); ++row)
	{
		holidayList.emplace_back(csv[row][0], csv[row][1]);
	}
	return holidayList;
}




