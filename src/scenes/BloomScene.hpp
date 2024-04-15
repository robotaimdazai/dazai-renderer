#pragma once
#include "IScene.hpp"
#include "../core/Shader.hpp"

class BloomScene:public IScene
{
public:
	auto load() -> void override;
	auto update() -> void override;
	auto onGui() -> void override;
};