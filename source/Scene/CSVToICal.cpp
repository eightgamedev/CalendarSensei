# include "Scene/Common.hpp"
# include "Scene/CSVToICal.hpp"

CSVToICal::CSVToICal(const InitData& init)
	: IScene(init)
{
	// 背景の色を設定する | Set the background color
	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
}

void CSVToICal::update()
{
	backToMenuButton.update();

	if (backToMenuButton.isPushed())
	{
		changeScene(State::MainScene);
	}
}

void CSVToICal::draw() const
{
	backToMenuButton.draw();
}
