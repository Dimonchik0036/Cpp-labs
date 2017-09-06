#include "gtest/gtest.h"
#include "tritset.h"

int main() {
	TritSet set(1000);
	size_t allocLength = set.capacity();
	EXPECT_TRUE(allocLength >= 1000 * 2 / 8 / sizeof(uint8_t));

	set[1000000000] = Unknown;
	EXPECT_TRUE(allocLength == set.capacity());
	EXPECT_TRUE(set.length() == 0);

	if (set[2000000000] == True) {
	}
	EXPECT_TRUE(allocLength == set.capacity());
	EXPECT_TRUE(set.length() == 0);

	set[1000000000] = True;
	EXPECT_TRUE(allocLength < set.capacity());
	EXPECT_TRUE(set.length() == 1000000001);

	allocLength = set.capacity();
	set[1000000000] = Unknown;
	set[1000000] = False;
	EXPECT_TRUE(allocLength == set.capacity());
	EXPECT_TRUE(set.cardinality(False) == 1 && set.cardinality(True) == 0);
	EXPECT_TRUE(set.length() == 1000001);

	set.shrink();
	EXPECT_TRUE(allocLength > set.capacity());
	TritSet setA(100);
	setA[5] = True;
	TritSet setB(2000);
	TritSet setC = setA & setB;
	EXPECT_TRUE(setC.capacity() == setB.capacity());
	TritSet setD = !setA;
	EXPECT_TRUE(setD.length() == setA.length());
	EXPECT_TRUE(setD.capacity() == setA.capacity());
	EXPECT_TRUE(setD == !setA);
	EXPECT_TRUE(setA == !!setA);

	setA[14] = Trit(14);
	setA[15] = Trit(15);
	EXPECT_TRUE(setA[14] == Unknown && setA[15] == Unknown);

	TritSet setE(3);
	setE[0] = False;
	setE[1] = Unknown;
	setE[2] = True;

	EXPECT_TRUE((!setE[0] == True) && (!setE[1] == Unknown) && (!setE[2] == False));

	EXPECT_TRUE(((setE[0] & setE[0]) == False) && ((setE[0] & setE[1]) == False) && ((setE[0] & setE[2]) == False));
	EXPECT_TRUE(((setE[1] & setE[0]) == False) && ((setE[1] & setE[1]) == Unknown) && ((setE[1] & setE[2]) == Unknown));
	EXPECT_TRUE(((setE[2] & setE[0]) == False) && ((setE[2] & setE[1]) == Unknown) && ((setE[2] & setE[2]) == True));

	EXPECT_TRUE(((setE[0] | setE[0]) == False) && ((setE[0] | setE[1]) == Unknown) && ((setE[0] | setE[2]) == True));
	EXPECT_TRUE(((setE[1] | setE[0]) == Unknown) && ((setE[1] | setE[1]) == Unknown) && ((setE[1] | setE[2]) == True));
	EXPECT_TRUE(((setE[2] | setE[0]) == True) && ((setE[2] | setE[1]) == True) && ((setE[2] | setE[2]) == True));
}