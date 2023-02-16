#include "BinaryTranslator.hpp"
#include "Type.hpp"

#include <stdexcept>
#include <string>
#include <binders.h>

BinaryTranslator::BinaryTranslator()
{
	registerTypes();
}

BinaryTranslator::~BinaryTranslator()
{

}

void BinaryTranslator::registerTypes()
{
	registerDefaultTypes();
}

void BinaryTranslator::registerDefaultTypes()
{
	REGISTER_TYPE("text", std::string, getTextAsString, text);

	REGISTER_TYPE("int", int, getTextAsNumeral, std::stoi(text));
	REGISTER_TYPE("uint", unsigned int, getTextAsNumeral, std::stoul(text));
	REGISTER_TYPE("float", float, getTextAsNumeral, std::stof(text));

	REGISTER_TYPE("hint", int, getTextAsNumeral, std::stoi(text, nullptr, 16));
	REGISTER_TYPE("huint", unsigned int, getTextAsNumeral, std::stoul(text, nullptr, 16));
}

std::string BinaryTranslator::getTextAsString(std::stringstream& input)
{
	std::string text;

	char buffer;
	input >> buffer; // get the first '"'

	input >> std::noskipws;

	while(input >> buffer && buffer != '\"')
	{
		text.push_back(buffer);
	}

	input >> std::skipws;

	return text;
}

std::string BinaryTranslator::getTextAsNumeral(std::stringstream& input)
{
	std::string text;
	input >> text;

	return text;
}


void BinaryTranslator::translate(const char* inputPath, const char* outputPath)
{
	readInputData(inputPath);
	write(outputPath);
}

void BinaryTranslator::readInputData(const char* path)
{
	mFile.open(path, std::ios::in);
	mInput << mFile.rdbuf();
	mFile.close();
}

void BinaryTranslator::write(const char* path)
{
	mFile.open(path, std::ios::out | std::ios::binary | std::ios::trunc);

	writeTypesFromInput();

	mFile.close();
}

void BinaryTranslator::writeTypesFromInput()
{
	std::string type;
	while(mInput >> type)
	{
		if(tryToWriteType(type))
			continue;

		else if(tryToWriteArray(type))
			continue;

		else
			throw std::runtime_error("Invalid use of '" + type + "' type.");
	}
}

bool BinaryTranslator::tryToWriteType(const std::string& typeName)
{
	const auto typeIt = mTypes.find(typeName);
	
	if(!isValidType(typeIt))
		return false;

	BT_MESSAGE("Writing type ", typeName, "\n");

	auto& type = typeIt->second;
	type->write(mFile, mInput);

	BT_PRINT_LINE();

	return true;
}

bool BinaryTranslator::tryToWriteArray(const std::string& type)
{
	if(type != "array")
		return false;

	std::string arrayType;
	mInput >> arrayType;

	const auto&& status = tryToWriteArrayType(arrayType);

	return status;
}

bool BinaryTranslator::tryToWriteArrayType(const std::string& typeName)
{
	const auto typeIt = mTypes.find(typeName);
	if(!isValidType(typeIt))
		return false;


	getArrayValues();

	BT_MESSAGE("Writing array of type ", typeName, "\n");

	writeArray(typeIt->second);

	BT_PRINT_LINE();
	BT_MESSAGE("-> array input = {\n", mArrayValues.str(), "\n}\n\n")

	cleanArrayData();

	return true;
}

void BinaryTranslator::getArrayValues()
{
	std::string value;

	while(mInput >> value && value != "end")
	{
		mArrayValues << value;
		mArraySize++;
	}
}


void BinaryTranslator::writeArray(std::shared_ptr<TypeI> type)
{
	mFile.write((char*)&mArraySize, sizeof(array_size_t));

	for(array_size_t i = 0; i < mArraySize; i++)
	{
		type->write(mFile, mArrayValues);
	}
}

void BinaryTranslator::cleanArrayData()
{
	mArraySize = 0;
	mArrayValues.str("");
	mArrayValues.clear();
}


bool BinaryTranslator::isValidType(const BinaryTranslator::TypesMap::iterator it)
{
	if(it == mTypes.end())
		return false;

	return true;
}


