#include <iostream>

#include "Type.h"

#include "util/easylogging++.h"
_INITIALIZE_EASYLOGGINGPP
#undef CHECK

int main(int argc, char** argv) {
    std::cout << "Hello World" << std::endl;
	std::cout << sizeof(Variant) << std::endl;
    return 0;
}