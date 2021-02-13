#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

struct TempVertex
{
	glm::vec3 vector;
	glm::vec2 uv;
};

class Sphere
{
private:
	static GLuint id;
	static int indexCount;
public:
	static GLuint SetupSphere();
	static int GetIndexCount() { return Sphere::indexCount; }
};
GLuint Sphere::id = 0;
int Sphere::indexCount = 0;

void ComputeTangentBasis(
	//inputs
	TempVertex v1,
	TempVertex v2,
	TempVertex v3,
	// outputs
	glm::vec3 &tangent,
	glm::vec3 &bitangent
) 
{
	// Edges of the triangle : position delta
	glm::vec3 deltaPos1 = v2.vector - v1.vector;
	glm::vec3 deltaPos2 = v3.vector - v1.vector;

	// UV delta
	glm::vec2 deltaUV1 = v2.uv - v1.uv;
	glm::vec2 deltaUV2 = v3.uv - v1.uv;

	float denominator = (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	if (denominator < 0.0001f && denominator > -0.0001f)
		denominator = 0.0001f;
	denominator *= -1.0f;

	float r = 1.0f / denominator;
	tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
	bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
}

GLuint Sphere::SetupSphere()
{
	if (id == 0)
	{
		glGenVertexArrays(1, &id);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));

				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back(y		  * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		// ************************* Calculating Tangents and Bitangents ******************************
		std::vector<glm::vec3> tangents(positions.size(), glm::vec3(0.0f));
		std::vector<glm::vec3> bitangents(positions.size(), glm::vec3(0.0f));
		TempVertex v1, v2, v3;
		glm::vec3 tmpTangent, tmpBitangent;
		for (int i = 0; i < indexCount - 2; i ++)
		{

			v1.vector = positions[indices[i]];
			v1.uv = uv[indices[i]];
			v2.vector = positions[indices[i + 1]];
			v2.uv = uv[indices[i + 1]];
			v3.vector = positions[indices[i + 2]];
			v3.uv = uv[indices[i + 2]];

			ComputeTangentBasis(v1, v2, v3, tmpTangent, tmpBitangent);
			tangents[indices[i]] += tmpTangent;
			tangents[indices[i + 1]] += tmpTangent;
			tangents[indices[i + 2]] += tmpTangent;

		}

		//****************************************8****************************************************

		std::vector<float> data;
		for (std::size_t i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
			if (tangents.size() > 0)
			{
				data.push_back(tangents[i].x);
				data.push_back(tangents[i].y);
				data.push_back(tangents[i].z);
			}
		}

		glBindVertexArray(id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
	}
	return id;
}