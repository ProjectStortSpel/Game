#include "LoadingScreen.h"
#include "Console/Console.h"
#include "HomePath.h"
#include "Game/Quaternion.h"
#include "FileSystem/File.h"

LoadingScreen::LoadingScreen()
{
	m_r = 0;
	m_g = 0;
	m_b = 0;
	m_graphicDevice = 0;

	m_color[0] = 0;
	m_color[1] = 0;
	m_color[2] = 0;

	m_scaleX = 1.0f;
	m_scaleY = 0.1f;

	//m_textMatrix

	m_textMatrix = glm::mat4(1);
	m_backgroundMatrix = glm::mat4(1);

	m_isActive = false;
	m_accessLevel = 0;
}

LoadingScreen::~LoadingScreen()
{
}

LoadingScreen& LoadingScreen::GetInstance()
{
	static LoadingScreen instance;
	return instance;
}

void LoadingScreen::SetGraphicsDevice(Renderer::GraphicDevice* _graphicDevice)
{
	m_graphicDevice = _graphicDevice;

	//m_graphicDevice->
	m_backgroundMatrix = glm::mat4(1);
	m_backgroundMatrix *= glm::translate(glm::vec3(0, 0, -1.002));

	//scale
	int width, height;
	m_graphicDevice->GetWindowSize(width, height);
	m_backgroundMatrix *= glm::scale(glm::vec3(2.004f, 2.004f * ((float)height / (float)width), 1.0f));
}

void LoadingScreen::SetTextColor(float _r, float _g, float _b)
{
	m_r = _r;
	m_g = _g;
	m_b = _b;
}

void LoadingScreen::SetLoadingText(std::string _text)
{
	if (m_isActive)
	{
		SDL_Color color = { static_cast<Uint8>(255 * m_r), static_cast<Uint8>(255 * m_g), static_cast<Uint8>(255 * m_b), 0 };
		float ratio = m_graphicDevice->CreateTextTexture("loadingscreentext", _text, 0, color);
		m_graphicDevice->ChangeModelTexture(m_textModel, "loadingscreentext");


		m_textMatrix = glm::mat4(1);

		//translate
		m_textMatrix *= glm::translate(glm::vec3(0, 0, -1.001));

		//scale
		float scaleX = m_scaleX;
		float scaleY = m_scaleY;
		if (m_scaleY * ratio > m_scaleX)
			scaleY = scaleX / ratio;
		else
			scaleX = scaleY * ratio;

		m_textMatrix *= glm::scale(glm::vec3(scaleX, scaleY, 1));

		SDL_Log("Loadingscreen: %s", _text.c_str());
		//m_graphicDevice->Update(0.0f);
		//m_graphicDevice->Render();
	}
}

void LoadingScreen::SetBackground(std::string _path)
{
	if (m_isActive)
	{
		if (FileSystem::File::Exist(_path))
		{
			m_graphicDevice->ChangeModelTexture(m_backgroundModel, _path);
			SDL_Log("Set Loadingscreen background: %s", _path.c_str());

			//m_graphicDevice->Update(0.0f);
			//m_graphicDevice->Render();
			return;
		}
		SDL_Log("Couldn't find background: %s", _path.c_str());
	}
}

void LoadingScreen::SetBackground(std::vector<std::string> _paths, std::string _file)
{
	if (m_isActive)
	{

		for (int i = 0; i < _paths.size(); ++i)
		{
			std::string path = _paths[i];
			path.append(_file);

			if (FileSystem::File::Exist(path))
			{
				m_graphicDevice->ChangeModelTexture(m_backgroundModel, path);
				SDL_Log("Set Loadingscreen background: %s", _file.c_str());

				//m_graphicDevice->Update(0.0f);
				//m_graphicDevice->Render();
				return;
			}
			SDL_Log("Couldn't find background: %s", _file.c_str());
		}
	}
}

void LoadingScreen::SetActive()
{
	if (!m_isActive)
	{
		std::vector<std::string> dirs = HomePath::GetHomePaths(HomePath::Type::Server);
		for (int i = 0; i < dirs.size(); ++i)
			dirs[i].append("models/quad/");
		m_backgroundModel = m_graphicDevice->LoadModel(dirs, "loadingscreen.object", &m_backgroundMatrix, RENDER_VIEWSPACE, &m_color[0], false, false);

		dirs.clear();
		dirs = HomePath::GetHomePaths(HomePath::Type::Server);
		for (int i = 0; i < dirs.size(); ++i)
			dirs[i].append("models/text/");
		m_textModel = m_graphicDevice->LoadModel(dirs, "quad.object", &m_textMatrix, RENDER_INTERFACE, &m_color[0], false, false);

		//m_graphicDevice->ActiveModel(m_backgroundModel, true);
		//m_graphicDevice->ActiveModel(m_textModel, true);
		m_isActive = true;

		//m_graphicDevice->Update(0.0f);
		//m_graphicDevice->Render();
	}
}

void LoadingScreen::SetInactive(int _level)
{
	if (m_isActive && _level >= m_accessLevel)
	{
		m_graphicDevice->RemoveModel(m_backgroundModel);
		m_graphicDevice->RemoveModel(m_textModel);

		//m_graphicDevice->ActiveModel(m_backgroundModel, false);
		//m_graphicDevice->ActiveModel(m_textModel, false);
		m_isActive = false;
		m_accessLevel = 0;
		//m_graphicDevice->Update(0.0f);
		//m_graphicDevice->Render();
	}
}

bool LoadingScreen::IsActive()
{
	return m_isActive;
}