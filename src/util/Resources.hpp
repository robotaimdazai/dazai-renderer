#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include "stb/stb_image.h"
#include "../core/Shader.hpp"
#include <map>


//globals
inline static const std::string SHADER_DEFAULT = "default";
inline static const std::string SHADER_DEFAULT_INSTANCED = "defaultinstanced";
inline static const std::string SHADER_LIGHT = "light";
inline static const std::string SHADER_OUTLINE = "outline";
inline static const std::string SHADER_FRAMEBUFFER = "framebuffer";
inline static const std::string SHADER_BLUR = "blur";
inline static const std::string SHADER_SKYBOX = "skybox";

namespace DazaiEngine 
{
	static class Resources 
	{
		public:
            
            //storage
            inline static std::map<std::string, Shader> shaders;

            //-----
            auto static loadShader(const std::string& name, const std::string& vert, const std::string& frag) -> Shader&
            {
                shaders.emplace(name, Shader(vert, frag));
                return shaders.at(name);
            }

            auto static getShader(const std::string& name) -> Shader&
            {
                try
                {
                    return shaders.at(name);
                }
                catch (const std::exception& e)
                {
                    std::cout << "SHADER NOT FOUND EXCEPTION" << std::endl;
                }
            }

            auto static readTextFile(const std::string& path) -> std::string
            {
                try
                {
	                const auto resolvedPath = RESOURCES + path;
                    //std::cout << resolvedPath << std::endl;
	                const std::ifstream stream(resolvedPath);
            
                    if (!stream.is_open())
                    {
                        std::cout << "Resources:: Can't open file" << path << std::endl;
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

            auto static loadRawTexture(const std::string& path, int* width, int* height,
                int* numChannels, int desiredChannels, bool flipVertical = true)->unsigned char*
            {
                std::string resolvedPath = RESOURCES + path;
                    stbi_set_flip_vertically_on_load(flipVertical);
                return stbi_load(resolvedPath.c_str(), width, height, numChannels, 0);
            }

            auto static getFileExtension(const std::string& path)->std::string
            {
               size_t dotPos = path.find_last_of('.');
               if (dotPos != std::string::npos && dotPos != path.length() - 1)
                   return path.substr(dotPos + 1); // extension starts after dot

               return "";
            }

            auto static loadAllShaders()-> void
            {
                loadShader(SHADER_DEFAULT,
                    "shaders/default.vert", "shaders/default.frag");
                loadShader(SHADER_DEFAULT_INSTANCED,
                    "shaders/defaultinstanced.vert", "shaders/default.frag");
                loadShader(SHADER_LIGHT,
                    "shaders/light.vert", "shaders/light.frag");
                loadShader(SHADER_OUTLINE,
                    "shaders/outline.vert", "shaders/outline.frag");
                 loadShader(SHADER_FRAMEBUFFER,
                    "shaders/framebuffer.vert", "shaders/framebuffer.frag");
                 loadShader(SHADER_BLUR,
                    "shaders/framebuffer.vert", "shaders/blur.frag");
                loadShader(SHADER_SKYBOX,
                    "shaders/skybox.vert", "shaders/skybox.frag");
            } 
	};
}