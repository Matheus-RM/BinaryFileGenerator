#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>

class BinaryTranslator
{
	public:
		BinaryTranslator();
		~BinaryTranslator();

	public:
		void translate(const char* inputPath, const char* outputPath);

	private:
		void read(const char* path);
		void write(const char* path);


		bool checkTypes(const std::string& type);
		bool checkArray(const std::string& type);
		void checkArrayTypes(const std::string& type);

		template <typename T>
		void writeValue()
		{
			T value;
			mInput >> value;

			mFile.write((char*)&value, sizeof(value));
		}

		template <typename T>
		void writeValue(T value)
		{
			mFile.write((char*)&value, sizeof(value));
		}

		void writeText();
		void writeText(const std::string& text);

		template <typename T>
		void writeArray(std::function<T(const std::string& text)> caster)
		{
			std::vector<std::string> data;
			std::string buffer;

			while(mInput >> buffer)
			{
				if(buffer == "end")
					break;

				data.push_back(buffer);
			}

			writeValue<unsigned int>(data.size());

			for(const auto& value : data)
			{
				std::cout << "value: " << value << "\n";
				writeValue<T>(caster(value));
			}
		}
		void writeTextArray();
		
		std::string readText();

	private:
		std::fstream mFile;
		std::stringstream mInput;
};