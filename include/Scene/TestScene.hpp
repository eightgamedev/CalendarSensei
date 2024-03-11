#pragma once
# include "CustomizedSimpleGUI/Button.hpp"
# include "CustomizedSimpleGUI/Pulldown.hpp"

class TestScene : public App::Scene
{
public:
	TestScene(const InitData& init);
	void update() override;
	void draw() const override;
private:
	CustomizedSimpleGUI::Button CSVToICalButton{ U"CSVをiCalendarに変換", 24, Vec2{ 100, 170 } };
	CustomizedSimpleGUI::Button ICalToCSVButton{ U"iCalendarをCSVに変換", 24, Vec2{ 100, 270 } };
	CustomizedSimpleGUI::Button SyukujitsuCSVToICalButton{ U"祝日CSVをiCalendarに変換", 24, Vec2{ 100, 370 } };
};
