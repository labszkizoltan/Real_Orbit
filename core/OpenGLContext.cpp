#include "OpenGLContext.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#define GL_MAJOR_VERSION 0x821B // copied from glad.h -> but not working here, the SFML opengl header includes GL.h, but it doesnt contain these macros
#define GL_MINOR_VERSION 0x821C // the problem was that the graphics context was not initialized, 

void OpenGLContext::GetMinorMajorVersion(int& minor, int& major)
{
	sf::Context context;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
}
