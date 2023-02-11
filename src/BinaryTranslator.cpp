#include "BinaryTranslator.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <binders.h>

BinaryTranslator::BinaryTranslator()
{

}

BinaryTranslator::~BinaryTranslator()
{

}


void BinaryTranslator::translate(const char* inputPath, const char* outputPath)
{
	read(inputPath);
	write(outputPath);
}

void BinaryTranslator::read(const char* path)
{
	mFile.open(path, std::ios::in);
	mInput << mFile.rdbuf();
	mFile.close();
}

void BinaryTranslator::write(const char* path)
{
	mFile.open(path, std::ios::out | std::ios::binary | std::ios::trunc);

	std::string type;
	while(mInput >> type)
	{
		if(checkTypes(type))
			continue;

		else if(checkArray(type))
			continue;

		else
			throw std::runtime_error("Invalid use of '" + type + "' type.");
	}

	mFile.close();
}

bool BinaryTranslator::checkTypes(const std::string& type)
{
	if(type == "text")
	{
		std::cout << "writing text \n";
		writeText();
		
		return true;
	}
	else if(type == "int")
	{
		std::cout << "writing int \n";
		writeValue<int>();

		return true;
	}
	else if(type == "uint")
	{
		std::cout << "writing uint \n";
		writeValue<unsigned int>();
		
		return true;
	}
	else if(type == "hint")
	{
		std::cout << "writing hex int \n";

		int value;
		mInput >> std::hex >> value;

		writeValue<int>(value);

		return true;
	}
	else if(type == "huint")
	{
		std::cout << "writing hex uint \n";

		unsigned int value;
		mInput >> std::hex >> value;

		writeValue<unsigned int>(value);
		
		return true;
	}
	else if(type == "float")
	{
		std::cout << "writing float \n";
		writeValue<float>();
		
		return true;
	}

	return false;
}

bool BinaryTranslator::checkArray(const std::string& type)
{
	if(type == "array")
	{
		std::string arrayType;
		mInput >> arrayType;

		checkArrayTypes(arrayType);

		return true;
	}

	return false;
}

void BinaryTranslator::checkArrayTypes(const std::string& type)
{
	std::cout << "writing array \n";

	if(type == "text")
	{
		std::cout << "writing text array\n";
		writeTextArray();
	}
	else if(type == "int")
	{
		std::cout << "writing int array\n";
		writeArray(std::function<int(const std::string& text)>(
					[&](const std::string& text){ return std::stoi(text); }));
	}
	else if(type == "uint")
	{
		std::cout << "writing uint array\n";
		writeArray(std::function<unsigned int(const std::string& text)>(
					[&](const std::string& text){ return std::stoi(text); }));
	}
	else if(type == "float")
	{
		std::cout << "writing float array\n";
		writeArray(std::function<float(const std::string& text)>(
					[&](const std::string& text){ return std::stof(text); }));
	}
}


void BinaryTranslator::writeTextArray()
{
	std::vector<std::string> data;
	std::string buffer;

	while(mInput >> buffer)
	{
		if(buffer == "end")
			break;

		std::cout << "text: " << buffer << "\n";
		data.push_back(buffer);
	}

	writeValue<unsigned int>(data.size());

	for(const auto& value : data)
	{
		writeText(value);
	}
}

void BinaryTranslator::writeText()
{
	auto&& text = readText();

	std::cout << "text: " << text << "\n";

	writeValue<unsigned int>(text.size());
	mFile.write(text.c_str(), text.size() * sizeof(char));
}

void BinaryTranslator::writeText(const std::string& text)
{
	std::cout << "text: " << text << "\n";

	writeValue<unsigned int>(text.size());
	mFile.write(text.c_str(), text.size() * sizeof(char));
}

std::string BinaryTranslator::readText()
{
	std::string text;
	char buffer;
	mInput >> buffer;
	mInput >> std::noskipws;
	while(mInput >> buffer)
	{
		if(buffer == '\"')
			break;

		text.push_back(buffer);
	}
	mInput >> std::skipws;

	return text;
}

