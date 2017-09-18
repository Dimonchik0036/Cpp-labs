#include "trit.h"

Trit operator!(Trit a) {
	if (a == Unknown) {
		return Unknown;
	}

	if (a == True) {
		return False;
	}

	return True;
}

Trit operator&(Trit a, Trit b) {
	if (a == False || b == False) {
		return False;
	}

	if (a == Unknown || b == Unknown) {
		return Unknown;
	}

	return True;
}

Trit operator|(Trit a, Trit b) {
	if (a == True || b == True) {
		return True;
	}

	if (a == Unknown || b == Unknown) {
		return Unknown;
	}

	return False;
}
