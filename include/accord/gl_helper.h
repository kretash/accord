#pragma once
#include <stdint.h>
#include <string>
#include <fstream>
#include <streambuf>

#include "accord/accord.h"

class Mesh;

int32_t _create_program(std::string vs_shader, std::string fs_shader);
void _load_obj( std::string obj, Mesh* d );
std::vector<glm::vec3> _load_obj(std::string obj);
void _load_obj_chunk(std::string obj, std::vector<float>* v_buffer, glm::vec3 pos);
void _upload_vertices(std::vector<float>* v_data, GLint* v_buffer, GLuint* vao);
void GLAPIENTRY opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);