#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "stb/stb_image.h"

namespace DazaiEngine 
{
	static class FileSystem 
	{
		public:
            auto static loadFile(const std::string& path) -> std::string
            {
                try
                {
	                const auto resolvedPath = RESOURCES + path;
                    //std::cout << resolvedPath << std::endl;
	                const std::ifstream stream(resolvedPath);
            
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

            auto static getFileExtension(const std::string& path)->std::string
            {
               size_t dotPos = path.find_last_of('.');
               if (dotPos != std::string::npos && dotPos != path.length() - 1)
                   return path.substr(dotPos + 1); // extension starts after dot

               return "";
            }


	};
}