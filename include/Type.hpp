#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <sstream>

#ifdef BT_SHOW_MESSAGES

	#include <iostream>
	#define BT_MESSAGE(message, value, end) {std::cout << message << value << end;}
	#define BT_PRINT_LINE() {std::cout << "\n";}

#else
	#define BT_MESSAGE(message, value, end) {}
	#define BT_PRINT_LINE() {}

#endif

using textGetterFunction = std::function<std::string(std::stringstream& buffer)>;

template <class T>
using textCasterFunction = std::function<T(const std::string&)>;

using array_size_t = unsigned int;

class TypeI
{
	public:
		TypeI(textGetterFunction getter) : mTextGetter(getter) {}
		virtual ~TypeI() {}

	public:
		virtual void write(std::fstream& file, std::stringstream& getterBuffer) = 0;
		virtual void write(std::fstream& file, const std::string& text) = 0;

	protected:
		textGetterFunction mTextGetter;
};

template <class T>
class Type : public TypeI
{
	public:
		Type(textGetterFunction getter, textCasterFunction<T> caster)
				: TypeI(getter), mTextCaster(caster) {}
		~Type() {}

	public:
		void write(std::fstream& file, std::stringstream& getterInput) override
		{
			const auto&& text = this->mTextGetter(getterInput);
			write(file, text);
		}

		void write(std::fstream& file, const std::string& text) override
		{
			const auto&& value = mTextCaster(text);

			BT_MESSAGE("Writing value \"", value, "\"\n");

			file.write((char*)&value, sizeof(T));
		}

	private:
		textCasterFunction<T> mTextCaster;
};

template <>
inline void Type<std::string>::write(std::fstream& file, const std::string& text)
{
	const auto&& value = mTextCaster(text);
	const auto&& size = static_cast<array_size_t>(value.size());

	BT_MESSAGE("Writing text \"", value, "\", ");
	BT_MESSAGE("with size = ", size, "\n");

	file.write((char*)&size, sizeof(array_size_t));
	file.write(value.c_str(), value.size() * sizeof(char));
}
