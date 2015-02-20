#ifndef MODELEXPORTER_H
#define MODELEXPORTER_H

//#include <string>
#include <vector>
#include <fstream>

class ModelExporter{
public:
	std::fstream file;
	std::ifstream ifile;
	bool OpenFileForRead(const char* fileDir)
	{
		if (ifile.is_open())
			ifile.close();
		ifile.open(fileDir, std::ios::in | std::ios::binary);
		return ifile.is_open();
	}

	bool OpenFileForWrite(const char* fileDir)
	{
		if (file.is_open())
			file.close();
		file.open(fileDir, std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
		return file.is_open();
	}
	void CloseFile()
	{
		file.close();
		ifile.close();
	}

	void AddDataToFile(std::vector<float> data)
	{
		int byteSize = data.size() * sizeof(float);
		file.write((char*)&byteSize, sizeof(int));
		file.write((char*)data.data(), byteSize);
	}

	std::vector<float> ReadDataFromFile()
	{
		int size;
		ifile.read((char*)&size, sizeof(int));
		if (size < 0)
			size = 0;
		std::vector<float> data(size / sizeof(float));
		if (size > 0)
			ifile.read((char*)&data[0], size);

		return data;
	}
};
#endif