#pragma once

class IScene
{
public:
	auto virtual load()->void = 0;
	auto virtual update()->void = 0;
	auto virtual onGui()->void = 0;
};

