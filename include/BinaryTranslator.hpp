#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <memory>

#include "Type.hpp"

#define CREATE_TYPE(name, type, getter, casterReturn)		 				\
		{ 																	\
			name, std::make_shared<Type<type>>( 							\
				std::bind(&BinaryTranslator::getter, this, 					\
							std::placeholders::_1							\
						),													\
		 		std::function<type(const std::string&)>(					\
		 					[&](const auto& text) {return casterReturn;}	\
		 				)													\
		 		)															\
		}

#define REGISTER_TYPE(name, type, getter, casterReturn) 					\
		mTypes.insert(CREATE_TYPE(name, type, getter, casterReturn));
		

class BinaryTranslator
{
	public:
		BinaryTranslator();
		virtual ~BinaryTranslator();

	public:
		using TypesMap = std::unordered_map<std::string, std::shared_ptr<TypeI> >;

	protected:
		virtual void registerTypes();
		void registerDefaultTypes();

	public:
		void translate(const char* inputPath, const char* outputPath);

	private:
		void readInputData(const char* path);
		void write(const char* path);
		void writeTypesFromInput();

		bool tryToWriteArray(const std::string& type);
		bool tryToWriteArrayType(const std::string& type);
		void getArrayValues();
		void writeArray(std::shared_ptr<TypeI> type);
		void cleanArrayData();

		bool tryToWriteType(const std::string& type);

		std::string getTextAsString(std::stringstream& input);
		std::string getTextAsNumeral(std::stringstream& input);

		bool isValidType(const TypesMap::iterator it);

	private:
		std::fstream mFile;
		std::stringstream mInput;

		array_size_t mArraySize = 0;
		std::stringstream mArrayValues;

	protected:
		TypesMap mTypes;
};