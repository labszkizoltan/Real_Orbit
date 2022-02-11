
#ifndef ROFONT_H
#define ROFONT_H

#include <string>
#include <unordered_map>
#include <vector>

#include <core/scene/CoreComponents.h>
#include <core/rendering/drawables/LetterMesh.h>


struct LetterData
{
	float u;
	float v;
	float w;
	float h;
};

// https://www.w3schools.com/charsets/ref_html_ascii.asp
struct AlphabetDescription
{
	AlphabetDescription() = default;
	AlphabetDescription(const std::vector<std::string>& description_lines);
	AlphabetDescription(const std::string& description_file);

	~AlphabetDescription() = default;

private:
	void Init(const std::vector<std::string>& description_lines);

public:
	std::unordered_map<char, LetterData> letterDescriptions;
};


class ROFont
{
public:

	ROFont() = default;
	ROFont(const std::string& textureFile, const std::string& description_file);
	~ROFont() = default;

	void RODrawText(const std::string& text, Vec3D screen_coord, Vec3D letter_colour, float scale);


private:
	// the texture and the instance buffers are already part of this mesh structure
	std::unique_ptr<LetterMesh> m_LetterMesh = nullptr;
	AlphabetDescription m_AlphabetDescription;
	std::vector<TransformComponent> m_LetterTransforms;
};






#endif // ROFONT_H
