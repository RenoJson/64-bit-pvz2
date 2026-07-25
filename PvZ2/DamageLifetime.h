#pragma once
#include <Sexy/RtObject.h>
#include <Sexy/RtWeakPtr.h>

class DamagePhase : public Sexy::RtObject {
public:
	float StartAtHealthPercent;
	char pad[4];
	SexyString Animation;
	std::vector<SexyString> Layers;
};

class DamageLifetime : public Sexy::RtObject {
public:
	std::vector<DamagePhase> Phases;
};
