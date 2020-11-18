/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#ifndef __OPEN_GL_H__
#define __OPEN_GL_H__

#if defined(GL_WITH_GLAD)
#   include <glad/glad.h>
#else
#if defined(__APPLE__)
#   include "TargetConditionals.h"
#   include <OpenGLES/ES3/gl.h>

#elif defined(_WIN32) || defined(_WIN64)
#include <glad/glad.h>
#elif defined(__ANDROID__)
#  include <GLES3/gl3.h>
#else
#  include <GL/glew.h>
#  include <GL/gl.h>
#endif
#endif /* GL_WITH_GLAD */

#endif /* OPEN_GL_H */
