#include "gtest/gtest.h"
#include "tritset.h"

TEST(TritSet_Test, DefaultCheck) {
	TritSet set(1000);
	size_t allocLength = set.capacity();
	EXPECT_TRUE(allocLength >= 1000 * 2 / 8 / sizeof(uint8_t));

	set[1000000000] = Unknown;
	EXPECT_EQ(allocLength, set.capacity());
	EXPECT_EQ(set.length(), 0);

	if (set[2000000000] == True) {
	}

	EXPECT_EQ(allocLength, set.capacity());
	EXPECT_EQ(set.length(), 0);

	set[1000000000] = True;
	EXPECT_TRUE(allocLength < set.capacity());
	EXPECT_EQ(set.length(), 1000000001);

	allocLength = set.capacity();
	set[1000000000] = Unknown;
	set[1000000] = False;
	EXPECT_EQ(allocLength, set.capacity());
	EXPECT_TRUE(set.cardinality(False) == 1 && set.cardinality(True) == 0);
	EXPECT_EQ(set.length(), 1000001);
	EXPECT_ANY_THROW(set[-1] = True);
	EXPECT_NO_FATAL_FAILURE(set.shrink());
	EXPECT_NO_FATAL_FAILURE(TritSet(0).shrink());
	EXPECT_TRUE(allocLength > set.capacity());
}

TEST(TritSet_Test, Concan) {
	TritSet setA(100);
	setA[5] = True;
	TritSet setB(2000);
	EXPECT_EQ((setA & setB).capacity(), setB.capacity());
	EXPECT_EQ((setA | setB).capacity(), setB.capacity());

	TritSet setD = !setA;
	EXPECT_EQ(setD.length(), setA.length());
	EXPECT_EQ(setD.capacity(), setA.capacity());
	EXPECT_EQ(setD, !setA);
	EXPECT_EQ(setA, !!setA);

	setA[14] = Trit(14);
	setA[15] = Trit(15);
	EXPECT_TRUE(setA[14] == Unknown && setA[15] == Unknown);
}

TEST(TritSet_Test, LogicTable) {
	TritSet set(3);
	set[0] = False;
	set[1] = Unknown;
	set[2] = True;
	EXPECT_TRUE((!set[0] == True) && (!set[1] == Unknown) && (!set[2] == False));

	EXPECT_TRUE(((set[0] & set[0]) == False) && ((set[0] & set[1]) == False) && ((set[0] & set[2]) == False));
	EXPECT_TRUE(((set[1] & set[0]) == False) && ((set[1] & set[1]) == Unknown) && ((set[1] & set[2]) == Unknown));
	EXPECT_TRUE(((set[2] & set[0]) == False) && ((set[2] & set[1]) == Unknown) && ((set[2] & set[2]) == True));

	EXPECT_TRUE(((set[0] | set[0]) == False) && ((set[0] | set[1]) == Unknown) && ((set[0] | set[2]) == True));
	EXPECT_TRUE(((set[1] | set[0]) == Unknown) && ((set[1] | set[1]) == Unknown) && ((set[1] | set[2]) == True));
	EXPECT_TRUE(((set[2] | set[0]) == True) && ((set[2] | set[1]) == True) && ((set[2] | set[2]) == True));
}

TEST(TritSet_Test, Trim) {
	TritSet set(50);
	set[0] = True;
	set[2] = False;
	set[50] = False;
	set[47] = False;
	size_t len = set.length();
	EXPECT_NO_FATAL_FAILURE(set.trim(50));
	EXPECT_TRUE(len > set.length());
	EXPECT_EQ(set.length(), 47);
	EXPECT_NO_FATAL_FAILURE(set.trim(40));
	EXPECT_EQ(set.length(), 2);
	EXPECT_NO_FATAL_FAILURE(set.trim(1000));
}

TEST(TritSet_Test, Cardinality) {
	TritSet setF(50);
	setF[0] = True;
	setF[2] = False;
	setF[50] = False;
	setF[47] = False;

	EXPECT_EQ(1, setF.cardinality(True));
	EXPECT_EQ(3, setF.cardinality(False));
	EXPECT_EQ(setF.length() - setF.cardinality(True) - setF.cardinality(False), setF.cardinality(Unknown));

	auto cardinality = setF.cardinality();
	EXPECT_EQ(setF.cardinality(True), cardinality[True]);
	EXPECT_EQ(setF.cardinality(False), cardinality[False]);
	EXPECT_EQ(setF.cardinality(Unknown), cardinality[Unknown]);
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}