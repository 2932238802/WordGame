#pragma once
#include"common/constant.h"


namespace LosG {
	// hp
	struct HpComponent {
		d hp = 100;
		d max = 100;
	};

	// hungry
	struct HungerComponent {
		d value = 100;
		d max = 100;
		d decay_rate = 1;
		d decay_hp_rate = 1;
	};

	// sanity
	struct SanityComponent {
		d value = 100.0;
		d decay_rate = 0.05;
		d max = 100.0;
	};

	struct DrowsyTag {};         //  < 50
	struct DizzyTag {};          //  < 20
	struct DiedTag {};
};