#include "BinaryTranslator.hpp"

#include <string>

int main(int argc, char** argv)
{
	if(argc < 2)
		return -1;

	try
	{
		std::string name(argv[1]);

		auto input = "..\\input\\" + name + ".txt";
		auto output = "..\\output\\" + name + ".bin";

		BinaryTranslator bt;
		bt.translate(input.c_str(), output.c_str());
		
	}
	catch(std::runtime_error& e)
	{
		std::cout << "Runtime error: " << e.what() << "\n";
	}
	
	return 0;
}