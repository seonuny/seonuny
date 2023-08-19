#include <iostream>
#include <memory>
#include "CConn.hpp"


int main(int argc, char **argv)
{
    std::shared_ptr<CConn> ptr = std::make_shared<CConn>();
    return 0;
}
