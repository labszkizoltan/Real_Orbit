#include "ROFont.h"

#include <fstream>
#include <sstream>

#include <core/Application.h>
#include <core/rendering/Renderer.h>

AlphabetDescription::AlphabetDescription(const std::vector<std::string>& description_lines)
{
	Init(description_lines);
}

AlphabetDescription::AlphabetDescription(const std::string& description_file)
{
	std::string line;
	std::vector<std::string> description_lines;
	std::ifstream myfile(description_file);

	while (getline(myfile, line))
	{
		if (!line.empty() && line.find("//")) { description_lines.push_back(line); } // leave out empty lines and the ones that begin with "//"
	}
	myfile.close();

	Init(description_lines);
}

void AlphabetDescription::Init(const std::vector<std::string>& description_lines)
{
	for (int i = 1; i < description_lines.size(); i++)
	{
		char letter;
		LetterData data;

		std::stringstream ss(description_lines[i]);
		ss >> letter;
		ss >> data.u;
		ss >> data.v;
		ss >> data.w;
		ss >> data.h;
		letterDescriptions[letter] = data;
	}
}

ROFont::ROFont(const std::string& textureFile, const std::string& description_file)
	: m_LetterMesh(nullptr), m_AlphabetDescription(description_file)
{
	m_LetterMesh = std::make_unique<LetterMesh>(textureFile);
	SetWidthAndHeightInShader();
}

void ROFont::RODrawText(const std::string& text, Vec3D screen_coord, Vec3D letter_colour, float scale)
{
	m_LetterTransforms.clear();

	// change the units to pixels and make the bottom left corner the 0,0 coordinate
	TransformComponent current_screen_loc;
	current_screen_loc.location = screen_coord;

	for (int i = 0; i < text.size(); i++)
	{
		LetterData ldata = m_AlphabetDescription.letterDescriptions[text[i]];
		current_screen_loc.location.z = ldata.w;
		current_screen_loc.orientation.f1 = Vec3D(ldata.u, ldata.v, ldata.h);
		current_screen_loc.orientation.f2.x = scale;
		current_screen_loc.orientation.f3 = letter_colour;

		m_LetterTransforms.push_back(current_screen_loc);

		current_screen_loc.location.x += scale * ldata.w;
	}

	Renderer::BindShader(MeshType::LETTER_MESH);
	m_LetterMesh->DrawInstances(m_LetterTransforms);
}

void ROFont::SetWidthAndHeightInShader()
{
	m_ScreenW = Application::Get().GetWindow().GetWidth();
	m_ScreenH = Application::Get().GetWindow().GetHeight();
	std::shared_ptr<Shader> shader = Renderer::BindShader(MeshType::LETTER_MESH);
	shader->UploadUniformFloat("screen_w", m_ScreenW);
	shader->UploadUniformFloat("screen_h", m_ScreenH);
}
