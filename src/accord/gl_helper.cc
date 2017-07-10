#include "accord/gl_helper.h"
#include "GL/glew.h"
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "accord/actor.h"

struct obj_db {
	obj_db() {}
	~obj_db() {}

	obj_db(std::string n, GLint v, GLint e, GLuint ec, GLuint vao) :
		name(n), v_buffer(v), e_buffer(e), e_count(ec), vao(vao)
	{}

	std::string name = "";
	GLint v_buffer = 0;
	GLint e_buffer = 0;
	GLuint e_count = 0;
	GLuint vao = 0;
};
std::vector<obj_db> loaded_obj;

struct vao_db {
	vao_db() {}
	~vao_db() {}

	vao_db(GLuint vao, GLint ch1, GLint ch2, GLint ch3, GLint ch4, GLint ch5) :
		vao(vao), ch1(ch1), ch2(ch2), ch3(ch3), ch4(ch4), ch5(ch5)
	{}

	GLuint vao = 0;
	GLint ch1 = 0;
	GLint ch2 = 0;
	GLint ch3 = 0;
	GLint ch4 = 0;
	GLint ch5 = 0;
};
std::vector<vao_db> used_vao;

int32_t _create_program(std::string vs_shader, std::string fs_shader) {

	std::string vs_path = "../assets/shaders/" + vs_shader;
	std::string fs_path = "../assets/shaders/" + fs_shader;

	std::ifstream vs_ifstream(vs_path);
	std::string vs_str((std::istreambuf_iterator<char>(vs_ifstream)),
		std::istreambuf_iterator<char>());

	if (vs_str.size() == 0) {
		std::cout << "Vertex shader not found \n";
		return -1;
	}

	std::ifstream fs_ifstream(fs_path);
	std::string fs_str((std::istreambuf_iterator<char>(fs_ifstream)),
		std::istreambuf_iterator<char>());

	if (fs_str.size() == 0) {
		std::cout << "Fragment shader not found \n";
		return -1;
	}

	GLint vs = glCreateShader(GL_VERTEX_SHADER);
	GLint v_size = (GLint)strlen(vs_str.c_str());
	const char *vs_char = vs_str.c_str();
	glShaderSource(vs, 1, &vs_char, &v_size);
	glCompileShader(vs);

	GLchar* vs_shader_log = new GLchar[1024];
	glGetShaderInfoLog(vs, 1024, nullptr, vs_shader_log);
	if (strlen(vs_shader_log) != 0) {
		std::cout << "VS shader error: \n" << vs_shader_log << std::endl;
		delete vs_shader_log;
		return -1;
	}

	GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
	GLint f_size = (GLint)strlen(fs_str.c_str());
	const char *fs_char = fs_str.c_str();
	glShaderSource(fs, 1, &fs_char, &f_size);
	glCompileShader(fs);

	GLchar* fs_shader_log = new GLchar[1024];
	glGetShaderInfoLog(fs, 1024, nullptr, fs_shader_log);
	if (strlen(fs_shader_log) != 0) {
		std::cout << "FS shader error: \n" << fs_shader_log << std::endl;
		delete vs_shader_log;
		delete fs_shader_log;
		return -1;
	}

	GLint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	GLchar* info_log = new GLchar[1024];
	glGetProgramInfoLog(program, 1024, nullptr, info_log);
	if (strlen(info_log) != 0) {
		std::cout << "Program link error: \n" << info_log << std::endl;
		delete vs_shader_log;
		delete fs_shader_log;
		delete info_log;
		return -1;
	}

	delete vs_shader_log;
	delete fs_shader_log;
	delete info_log;

	return program;
}

void _load_obj(std::string obj, Mesh* m) {

	for (int i = 0; i < loaded_obj.size(); ++i) {
		if (loaded_obj[i].name == obj && loaded_obj[i].e_buffer != 0) {

			m->m_index_id = loaded_obj[i].e_buffer;
			m->m_vertex_id = loaded_obj[i].v_buffer;
			m->m_index_count = loaded_obj[i].e_count;
			m->m_vao_id = loaded_obj[i].vao;
			return;
		}
	}

	std::string inputfile = "../assets/obj/" + obj;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
	if (!err.empty()) {
		std::cout << err << std::endl;
	}

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				m->m_vertex_data.push_back(glm::vec3(
					attrib.vertices[3 * idx.vertex_index + 0],
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2]
				));

				m->m_vertex_data.push_back(glm::vec3(
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				));

				m->m_index_data.push_back((GLuint)(index_offset + v));
			}
			index_offset += fv;

		}
	}

	//TODO, fill in while loading the obj
	GLint channel1 = 3;
	GLint channel2 = 3;
	GLint channel3 = 0;
	GLint channel4 = 0;
	GLint channel5 = 0;
	bool found_vao = false;

	for (int i = 0; i < used_vao.size(); ++i) {
		if (used_vao[i].ch1 == 3 && used_vao[i].ch2 == 3 && used_vao[i].ch3 == 0 &&
			used_vao[i].ch4 == 0 && used_vao[i].ch5 == 0) {
			m->m_vao_id = used_vao[i].vao;
			found_vao = true;
			break;
		}
	}

	if (!found_vao)	glGenVertexArrays(1, &m->m_vao_id);

	glBindVertexArray(m->m_vao_id);

	glGenBuffers(1, &m->m_vertex_id );
	glBindBuffer(GL_ARRAY_BUFFER, m->m_vertex_id);
	glBufferData(GL_ARRAY_BUFFER, m->m_vertex_data.size() * sizeof(glm::vec3), m->m_vertex_data.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m->m_index_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->m_index_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->m_index_data.size() * sizeof(uint32_t), m->m_index_data.data(), GL_STATIC_DRAW);

	if (!found_vao) {
		GLuint vertex_index = 0;
		glEnableVertexAttribArray(vertex_index);
		glVertexAttribPointer(vertex_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
		GLuint normal_index = 1;
		glEnableVertexAttribArray(normal_index);
		glVertexAttribPointer(normal_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m->m_index_count = (GLuint)m->m_index_data.size();

	loaded_obj.push_back(obj_db(obj, m->m_index_id, m->m_index_id, m->m_index_count, m->m_vao_id));

	m->m_index_data.clear();
	m->m_index_data.shrink_to_fit();

	m->m_vertex_data.clear();
	m->m_vertex_data.shrink_to_fit();
}

std::vector<glm::vec3> _load_obj(std::string obj) {

	std::string inputfile = "../assets/obj/" + obj;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
	if (!err.empty()) {
		std::cout << err << std::endl;
	}

	std::vector<glm::vec3> geometry;

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				geometry.push_back(glm::vec3(
					attrib.vertices[3 * idx.vertex_index + 0],
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2]
				));

			}
		}
	}
	return geometry;
}

void _load_obj_chunk(std::string obj, std::vector<float>* v_buffer, glm::vec3 pos) {

	std::string inputfile = "../assets/obj/" + obj;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
	if (!err.empty()) {
		std::cout << err << std::endl;
	}

	std::vector<glm::vec3> v_data;
	std::vector<uint32_t> order;
	uint32_t max_size = 0;

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];


				glm::vec4 position = glm::vec4(
					attrib.vertices[3 * idx.vertex_index + 0],
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2], 1.0f);
				glm::mat4 move = glm::translate(glm::mat4(1.0f), pos);
				position = move * position;

				v_data.push_back(glm::vec3(
					position.x, position.x, position.z
				));

				v_data.push_back(glm::vec3(
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				));

				v_data.push_back(glm::vec3(
					attrib.texcoords[2 * idx.texcoord_index + 0],
					attrib.normals[2 * idx.texcoord_index + 1],
					0.0f // layer goes here
				));

				uint32_t o = (uint32_t)(index_offset + v);
				max_size = o > max_size ? o : max_size;
				order.push_back(o);
			}
			index_offset += fv;

		}
	}

	std::vector<float> array_data;
	array_data.reserve(max_size * 9);

	for (size_t i = 0; i < order.size(); ++i) {
		uint32_t id = order[i];

		for (int32_t e = 0; e < 9; ++e)
			array_data[id + e] = ((float*)v_data.data())[i + e];
	}

	for (int i = 0; i < array_data.size(); ++i) {
		v_buffer->push_back(array_data[i]);
	}

}

void GLAPIENTRY opengl_error_callback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::string s_source = "";
	std::string s_type = "";
	std::string s_severity = "";
	std::string s_message = std::string((char*)message);

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             s_source += "GL_DEBUG_SOURCE_API";             break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   s_source += "GL_DEBUG_SOURCE_WINDOW_SYSTEM";   break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: s_source += "GL_DEBUG_SOURCE_SHADER_COMPILER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     s_source += "GL_DEBUG_SOURCE_THIRD_PARTY";     break;
	case GL_DEBUG_SOURCE_APPLICATION:     s_source += "GL_DEBUG_SOURCE_APPLICATION";     break;
	case GL_DEBUG_SOURCE_OTHER:           s_source += "GL_DEBUG_SOURCE_OTHER";           break;
	default:                              s_source += "UNKNOWN SOURCE!";                 break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               s_source += "GL_DEBUG_TYPE_ERROR";               break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	s_source += "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  s_source += "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";  break;
	case GL_DEBUG_TYPE_PORTABILITY:         s_source += "GL_DEBUG_TYPE_PORTABILITY";         break;
	case GL_DEBUG_TYPE_PERFORMANCE:         s_source += "GL_DEBUG_TYPE_PERFORMANCE";         break;
	case GL_DEBUG_TYPE_MARKER:              s_source += "GL_DEBUG_TYPE_MARKER";              break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          s_source += "GL_DEBUG_TYPE_PUSH_GROUP";          break;
	case GL_DEBUG_TYPE_POP_GROUP:           s_source += "GL_DEBUG_TYPE_POP_GROUP";           break;
	case GL_DEBUG_TYPE_OTHER:               s_source += "GL_DEBUG_TYPE_OTHER";               break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         s_severity = "ERROR";          break;
	case GL_DEBUG_SEVERITY_MEDIUM:       s_severity = "WARNING";        break;
	case GL_DEBUG_SEVERITY_LOW:          s_severity = "SOFT WARNING";           break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: s_severity = "NOTIFICATION";  break;
	}

	std::string out_log = "[OPENGL][" + s_severity + "][" + s_type + "][" + s_source + "]: " + message;
	std::cout << out_log << "\n";
}

#if 0
void _upload_vertices(std::vector<float>* v_data, GLint* v_buffer, GLuint* vao) {

	//TODO, fill in while loading the obj
	GLint channel1 = 3;
	GLint channel2 = 3;
	GLint channel3 = 3;
	GLint channel4 = 0;
	GLint channel5 = 0;
	bool found_vao = false;

	for (int i = 0; i < used_vao.size(); ++i) {
		if (used_vao[i].ch1 == 3 && used_vao[i].ch2 == 3 && used_vao[i].ch3 == 0 &&
			used_vao[i].ch4 == 0 && used_vao[i].ch5 == 0) {
			d->vao = used_vao[i].vao;
			found_vao = true;
			break;
		}
	}

	if (!found_vao)	glGenVertexArrays(1, &d->vao);

	glBindVertexArray(d->vao);

	glGenBuffers(1, &d->v_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, d->v_buffer);
	glBufferData(GL_ARRAY_BUFFER, d->v_data.size() * sizeof(glm::vec3), d->v_data.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &d->e_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->e_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, d->e_data.size() * sizeof(uint32_t), d->e_data.data(), GL_STATIC_DRAW);

	if (!found_vao) {
		GLuint vertex_index = 0;
		glEnableVertexAttribArray(vertex_index);
		glVertexAttribPointer(vertex_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)0);
		GLuint normal_index = 1;
		glEnableVertexAttribArray(normal_index);
		glVertexAttribPointer(normal_index, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *)(sizeof(float) * 3));
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	d->e_count = (GLuint)d->e_data.size();

	loaded_obj.push_back(obj_db(obj, d->v_buffer, d->e_buffer, d->e_count, d->vao));

	d->e_data.clear();
	d->e_data.shrink_to_fit();

	d->v_data.clear();
	d->v_data.shrink_to_fit();
}
#endif