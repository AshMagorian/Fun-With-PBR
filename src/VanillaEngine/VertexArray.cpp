#include "VertexArray.h"
#include "Exception.h"
#include "PrimitiveShape.h"

#include <fstream>
#include <iostream>

void VertexArray::SplitStringWhitespace(std::string& input, std::vector<std::string>& output)
{
	std::string curr;

	output.clear();

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == ' ' ||
			input.at(i) == '\r' ||
			input.at(i) == '\n' ||
			input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}

void VertexArray::SplitString(std::string& input, char splitter, std::vector<std::string>& output)
{
	std::string curr;

	output.clear();

	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == splitter)
		{
			output.push_back(curr);
			curr = "";
		}
		else
		{
			curr += input.at(i);
		}
	}

	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}

VertexArray::VertexArray()
{
	dirty = false;

	buffers.resize(10);
	glGenVertexArrays(1, &id);
	if (!id)
	{
		throw std::exception();
	}
}

/**
*\brief Reads the model file and stores the data
*/
VertexArray::VertexArray(std::string path)
{
	if (path == "sphere")
	{
		id = PrimitiveShape::SetupSphere();
		indexCount = PrimitiveShape::GetIndexCount();
		dirty = false;
		return;
	}
	if (path == "plane")
	{
		id = PrimitiveShape::SetupPlane();
		vertexCount = 6;
		dirty = false;
		return;
	}

	glGenVertexArrays(1, &id);

	if (!id)
	{
		throw std::exception();
	}

	buffers.resize(10);
	std::ifstream file(path.c_str());


	if (!file.is_open())
	{
		throw Exception("Model file not found, '" + path + "' cannot be loaded");
	}

	std::string line;
	std::vector<std::string> splitLine;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	std::shared_ptr<VertexBuffer> positionBuffer = NULL;
	std::shared_ptr<VertexBuffer> texCoordBuffer = NULL;
	std::shared_ptr<VertexBuffer> normalBuffer = NULL;

	while (!file.eof())
	{
		std::getline(file, line);
		if (line.length() < 1) continue;
		SplitStringWhitespace(line, splitLine);
		if (splitLine.size() < 1) continue;

		if (splitLine.at(0) == "v")
		{
			if (!positionBuffer) positionBuffer = std::make_shared<VertexBuffer>();

			positions.push_back(glm::vec3(
				atof(splitLine.at(1).c_str()),
				atof(splitLine.at(2).c_str()),
				atof(splitLine.at(3).c_str())));
		}
		else if (splitLine.at(0) == "vt")
		{
			if (!texCoordBuffer) texCoordBuffer = std::make_shared<VertexBuffer>();

			texCoords.push_back(glm::vec2(
				atof(splitLine.at(1).c_str()),
				1.0f - atof(splitLine.at(2).c_str())));
		}
		else if (splitLine.at(0) == "vn")
		{
			if (!normalBuffer) normalBuffer = std::make_shared<VertexBuffer>();

			normals.push_back(glm::vec3(
				atof(splitLine.at(1).c_str()),
				atof(splitLine.at(2).c_str()),
				atof(splitLine.at(3).c_str())));
		}
		else if (splitLine.at(0) == "f")
		{
			std::vector<std::string> subsplit;
			SplitString(splitLine.at(1), '/', subsplit);

			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(2), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(3), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));

			if (splitLine.size() < 5) continue;
			SplitString(splitLine.at(3), '/', subsplit);

			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(4), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
			SplitString(splitLine.at(1), '/', subsplit);
			positionBuffer->add(positions.at(atoi(subsplit.at(0).c_str()) - 1));
			if (texCoordBuffer) texCoordBuffer->add(texCoords.at(atoi(subsplit.at(1).c_str()) - 1));
			if (normalBuffer) normalBuffer->add(normals.at(atoi(subsplit.at(2).c_str()) - 1));
		}
	}

	std::shared_ptr<VertexBuffer> tangentBuffer = std::make_shared<VertexBuffer>();
	TempVertex v1, v2, v3;
	glm::vec3 tmpTangent, tmpBitangent;
	std::vector<float> pos = positionBuffer->GetData();
	std::vector<float> uv = texCoordBuffer->GetData();
	int j = 0;
	for (int i = 0; i < positionBuffer->GetDataSize(); i += 9)
	{
		v1.vector.x = pos[i]; v1.vector.y = pos[i + 1]; v1.vector.z = pos[i + 2];
		v1.uv.x = uv[j++]; v1.uv.y = uv[j++];
		v2.vector.x = pos[i + 3]; v2.vector.y = pos[i + 4]; v2.vector.z = pos[i + 5];
		v2.uv.x = uv[j++]; v2.uv.y = uv[j++];
		v3.vector.x = pos[i + 6]; v3.vector.y = pos[i + 7]; v3.vector.z = pos[i + 8];
		v3.uv.x = uv[j++]; v3.uv.y = uv[j++];

		PrimitiveShape::ComputeTangentBasis(v1, v2, v3, tmpTangent, tmpBitangent);
		tangentBuffer->add(tmpTangent);
		tangentBuffer->add(tmpTangent);
		tangentBuffer->add(tmpTangent);
	}

	SetBuffer("in_Position", positionBuffer);
	if (texCoordBuffer) SetBuffer("in_TexCoord", texCoordBuffer);
	if (normalBuffer) SetBuffer("in_Normal", normalBuffer);
	if (tangentBuffer) SetBuffer("in_Tangent", tangentBuffer);

	vertexCount = buffers.at(0)->GetDataSize() / buffers.at(0)->GetComponents();
}

/**
*\brief Stores a buffer into the vector. It's position in teh vector depends on it's attrbute which is passed through
*/
void VertexArray::SetBuffer(std::string attribute, std::shared_ptr<VertexBuffer> buffer)
{

	if (attribute == "in_Position")
	{
		buffers.at(0) = buffer;
	}
	else if (attribute == "in_Color")
	{
		buffers.at(1) = buffer;
	}
	else if (attribute == "in_TexCoord")
	{
		buffers.at(2) = buffer;
	}
	else if (attribute == "in_Normal")
	{
		buffers.at(3) = buffer;
	}
	else if (attribute == "in_Tangent")
	{
		buffers.at(4) = buffer;
	}

	else
	{
		throw Exception(attribute + " buffer can't be set");
	}
	dirty = true;
}

int VertexArray::GetVertexCount()
{
	//if (!buffers.at(0))
	//{
	//	throw Exception("Buffers don't exist");
	//}
	//return buffers.at(0)->GetDataSize() / buffers.at(0)->GetComponents();
	return vertexCount;
}

GLuint VertexArray::GetId()
{
	if (dirty)
	{
		glBindVertexArray(id);

		for (size_t i = 0; i < buffers.size(); i++)
		{
			if (buffers.at(i))
			{
				glBindBuffer(GL_ARRAY_BUFFER, buffers.at(i)->GetId());

				glEnableVertexAttribArray(i);

				glVertexAttribPointer(i, buffers.at(i)->GetComponents(), GL_FLOAT, GL_FALSE,
					buffers.at(i)->GetComponents() * sizeof(GLfloat), (void *)0);
			}
			else
			{
				glDisableVertexAttribArray(i);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		dirty = false;
	}
	return id;
}