# include "Scene/Common.hpp"
# include "Scene/TestScene.hpp"

TestScene::TestScene(const InitData& init)
	: IScene(init)
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
}

void TestScene::update()
{
	CSVToICalButton.update();
	ICalToCSVButton.update();
	SyukujitsuCSVToICalButton.update();

	if (CSVToICalButton.isPushed())
	{
		changeScene(State::CSVToICal);
	}
	else if (ICalToCSVButton.isPushed())
	{
		changeScene(State::ICalToCSV);
	}
	else if (SyukujitsuCSVToICalButton.isPushed())
	{
		changeScene(State::SyukujitsuCSVToICal);
	}
}

void TestScene::draw() const
{
	CSVToICalButton.draw();
	ICalToCSVButton.draw();
	SyukujitsuCSVToICalButton.draw();
}
