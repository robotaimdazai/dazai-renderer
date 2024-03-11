#pragma once
#include <iostream>
#include <sstream>
#include <fstream>




namespace DazaiEngine 
{
  
	class FileSystem 
	{
		public:
            auto static loadFile(const std::string& path) -> std::string
            {
                try
                {
           
                    std::ifstream stream(path);
            
                    if (!stream.is_open())
                    {
                        std::cout << "FileSystem:: Can't open file" << std::endl;
                    }

                    std::stringstream buffer;
                    buffer << stream.rdbuf();
                    return buffer.str();
                }
                catch (const std::exception& ex)
                {
                    std::cout << "Exception occurred: " << ex.what() << std::endl;
                    return "";
                }
            }

	};
}