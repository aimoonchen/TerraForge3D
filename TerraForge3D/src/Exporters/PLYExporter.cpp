#include "Exporters/PLYExporter.h"
#include "Base/BinaryFileWriter.h"
#include "Utils/Utils.h"

#include <string>
#include <sstream>

PLYExporter::PLYExporter()
{
}

PLYExporter::~PLYExporter()
{
}

bool PLYExporter::ExportBinary(const std::string& path, Mesh* mesh, float* progress)
{
	if (!progress) progress = &m_Progress; *progress = 0.0f;
	BinaryFileWriter writer(path);
	if (!writer.IsOpen()) return false;
	std::string format = ""; int num = 1;
	if (*(char*)&num == 1) format = "binary_little_endian"; else format = "binary_big_endian";
	if (!this->WriteHeader(&writer, format, mesh)) return false;
	static uint8_t faceVertexCount = 3;
	float tmpp = 1.0f / mesh->GetVertexCount();
	for (int i = 0; i < mesh->GetVertexCount(); i++)
	{
		const auto& va = mesh->GetVertex(i);
		const float data[8] = {va.position.x, va.position.y, va.position.z, va.normal.x, va.normal.y, va.normal.z, va.texCoord.x, va.texCoord.y};
		writer.Write(data, sizeof(float) * 8);
		*progress = tmpp * i * 0.70f + 0.01f;
	}
	tmpp = 1.0f / mesh->GetIndexCount();
	for (int i = 0; i < mesh->GetFaceCount(); i++)
	{
		const auto& face = mesh->GetFace(i);
		writer.Write(faceVertexCount);
		const int32_t data[3] = { face.a, face.b, face.c };
		writer.Write(data, sizeof(int32_t) * 3);
		*progress = tmpp * i * 0.25f + 0.70f;
	}
	*progress = 1.0f;
	return true;
}


bool PLYExporter::ExportASCII(const std::string& path, Mesh* mesh, float* progress)
{
	if (!progress) progress = &m_Progress; *progress = 0.0f;
	BinaryFileWriter writer(path);
	if (!writer.IsOpen()) return false;
	if (!this->WriteHeader(&writer, "ascii", mesh)) return false;
	std::stringstream out_strm;
	float tmpp = 1.0f / mesh->GetVertexCount();
	for (int i = 0; i < mesh->GetVertexCount(); i++)
	{
		const auto& va = mesh->GetVertex(i);
		sprintf(buffer,
			"%f %f %f %f %f %f %f %f\n",
			va.position.x, va.position.y, va.position.z,
			va.normal.x, va.normal.y, va.normal.z,
			va.texCoord.x, va.texCoord.y
		);
		out_strm << buffer;
		*progress = tmpp * i * 0.70f + 0.01f;
	}
	tmpp = 1.0f / mesh->GetIndexCount();
	for (int i = 0; i < mesh->GetFaceCount(); i++)
	{
		const auto& face = mesh->GetFace(i);
		sprintf(buffer, "3 %d %d %d\n", face.a, face.b, face.c);
		out_strm << buffer;
		*progress = tmpp * i * 0.25f + 0.70f;
	}
	const std::string out_str = out_strm.str();
	writer.Write(out_str.data(), out_str.size());
	*progress = 1.0f;
	return true;
}

bool PLYExporter::WriteHeader(BinaryFileWriter* writer, const std::string& format, Mesh* mesh)
{
	sprintf(buffer, 
		"ply" "\n"
		"format %s 1.0" "\n"
		"comment Generated by TerraForge3D Gen 2" "\n"
		"comment Generation time : %s" "\n"
		"element vertex %d" "\n"
		"property float x" "\n"
		"property float y" "\n"
		"property float z" "\n"
		"property float nx" "\n"
		"property float ny" "\n"
		"property float nz" "\n"
		"property float s" "\n"
		"property float t" "\n"
		"element face %d" "\n"
		"property list uchar int vertex_index" "\n"
		"end_header" "\n",
		format.data(),
		GetTimeStamp().data(),
		mesh->GetVertexCount(),
		(mesh->GetFaceCount())
	);
	writer->Write(buffer, std::strlen(buffer));
	return true;
}