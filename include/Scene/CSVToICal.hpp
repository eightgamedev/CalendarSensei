#pragma once
# include "CustomizedSimpleGUI/Button.hpp"
# include "CustomizedSimpleGUI/Pulldown.hpp"
# include "icalendar/ICalendar.hpp"

class CSVToICal : public App::Scene
{
public:
	CSVToICal(const InitData& init);
	void update() override;
	void draw() const override;
private:


	CustomizedSimpleGUI::Button backToMenuButton{ U"メニューに戻る", 24, Vec2{ 100, 70 } };
};
