# include <Siv3D.hpp>

# include "Scene/Common.hpp"
# include "Scene/TestScene.hpp"
# include "Scene/SyukujitsuCSVToICal.hpp"
# include "Scene/MainScene.hpp"
# include "Scene/CSVToICal.hpp"

void Main()
{
	const String applicationName = U"Calendar Editor";
	const String version = U"v1.0.0";
	Window::SetTitle(U"{} {}"_fmt(applicationName, version));
	Window::SetStyle(WindowStyle::Sizable);
	Scene::Resize(1920, 1009);
	Window::Maximize();
	Scene::SetResizeMode(ResizeMode::Keep);

	App manager;
	manager.add<TestScene>(State::TestScene)
		.add<SyukujitsuCSVToICal>(State::SyukujitsuCSVToICal)
		.add<MainScene>(State::MainScene)
		.add<CSVToICal>(State::CSVToICal);

	manager.init(State::MainScene);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
