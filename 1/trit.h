#pragma once

enum Trit {
	False = 1,
	Unknown = 0,
	True = 2,
};

Trit operator!(Trit a);
Trit operator&(Trit a, Trit b);
Trit operator|(Trit a, Trit b);