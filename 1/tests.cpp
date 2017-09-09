#include "gtest/gtest.h"
#include "tritset.h"

TritSet *tritSetFromString(std::string str) {
	if (!str.length()) {
		return new TritSet();
	}

	TritSet *set = new TritSet(str.length());
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	size_t pos = 0;
	for (char c : str) {
		switch (c) {
			case 'f':
				(*set)[pos] = Trit(False);
				break;
			case 'u':
				(*set)[pos] = Trit(Unknown);
				break;
			case 't':
				(*set)[pos] = Trit(True);
				break;
			default:
				delete set;
				return new TritSet();
		}
		pos++;
	}

	return set;
}

TEST(ConstructorTritSetTest, ZeroMemory) {
	TritSet set;

	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
}

TEST(ConstructorTritSetTest, AllocatingMemory) {
	TritSet set(1000);

	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 1000);
}

TEST(ConstructorTritSetTest, SettingDefaultValue) {
	TritSet set(1000);
	for (size_t i = 0; i < 1000; ++i)
		set[i] = Trit(True);

	ASSERT_EQ(set.length(), 1000);
	ASSERT_GE(set.capacity(), 1000);

	for (size_t i = 0; i < 1000; i++)
		ASSERT_EQ(set[i], Trit(True));
}

TEST(MethodsTritSetTest, TrimSet) {
	TritSet set(100);
	for (size_t i = 0; i < 100; ++i)
		set[i] = Trit(True);


	ASSERT_EQ(set.length(), 100);
	ASSERT_GE(set.capacity(), 100);

	set.trim(50);

	ASSERT_EQ(set.length(), 50);
	ASSERT_GE(set.capacity(), 50);

	// Проверка оставшихся
	for (size_t i = 0; i < 50; i++)
		ASSERT_EQ(set[i], Trit(True));

	// Проверка удаленных
	for (size_t i = 50; i < 100; i++)
		ASSERT_EQ(set[i], Trit(Unknown));
}

TEST(MethodsTritSetTest, ShrinkSet) {
	TritSet set(1000);

	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 1000);

	set.shrink();

	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);

	set[99] = Trit(True);

	ASSERT_EQ(set.length(), 100);
	ASSERT_GE(set.capacity(), 100);

	set[49] = Trit(False);

	ASSERT_EQ(set.length(), 100);
	ASSERT_GE(set.capacity(), 100);

	set[99] = Trit(Unknown);

	ASSERT_EQ(set.length(), 50);
	ASSERT_GE(set.capacity(), 100);

	set.shrink();

	ASSERT_EQ(set.length(), 50);
	ASSERT_GE(set.capacity(), 50);
}

TEST(MethodsTritSetTest, Cardinality) {
	TritSet emptySet, setFalse(100, Trit(False)), setUnknown(100, Trit(Unknown)), setTrue(100, Trit(True));

	ASSERT_EQ(emptySet.cardinality(True), 0);
	ASSERT_EQ(emptySet.cardinality(False), 0);
	ASSERT_EQ(emptySet.cardinality(Unknown), 0);

	ASSERT_EQ(setTrue.cardinality(True), 100);
	ASSERT_EQ(setTrue.cardinality(False), 0);
	ASSERT_EQ(setTrue.cardinality(Unknown), 0);

	ASSERT_EQ(setFalse.cardinality(True), 0);
	ASSERT_EQ(setFalse.cardinality(False), 100);
	ASSERT_EQ(setFalse.cardinality(Unknown), 0);

	ASSERT_EQ(setUnknown.cardinality(True), 0);
	ASSERT_EQ(setUnknown.cardinality(False), 0);
	ASSERT_EQ(setUnknown.cardinality(Unknown), 0);

	TritSet *set = tritSetFromString("FFFUUUTTT");
	std::unordered_map<Trit, size_t, std::hash<size_t> > map = {
			{True,    3},
			{False,   3},
			{Unknown, 3},
	};

	ASSERT_EQ(set->cardinality(), map);
	delete set;
}

/** NOT оператор. */
TEST(OperatorsTritSetTest, OperatorNOTEmpty) {
	TritSet emptySet;

	ASSERT_EQ(emptySet.length(), 0);
	ASSERT_GE(emptySet.capacity(), 0);

	emptySet = ~emptySet;

	ASSERT_EQ(emptySet.length(), 0);
	ASSERT_GE(emptySet.capacity(), 0);
}

TEST(OperatorsTritSetTest, OperatorNOTFalse) {
	TritSet setFalse(10, Trit(False));

	ASSERT_EQ(setFalse.length(), 10);
	ASSERT_GE(setFalse.capacity(), 10);

	setFalse = ~setFalse;

	ASSERT_EQ(setFalse.length(), 10);
	ASSERT_GE(setFalse.capacity(), 10);

	for (size_t i = 0; i < 10; i++)
		ASSERT_EQ(setFalse[i], Trit(True));
}

TEST(OperatorsTritSetTest, OperatorNOTUnknown) {
	TritSet setUnknown(10, Trit(Unknown));

	ASSERT_EQ(setUnknown.length(), 0);
	ASSERT_GE(setUnknown.capacity(), 10);

	setUnknown = ~setUnknown;

	ASSERT_EQ(setUnknown.length(), 0);
	ASSERT_GE(setUnknown.capacity(), 0);

	for (size_t i = 0; i < 10; i++)
		ASSERT_EQ(setUnknown[i], Trit(Unknown));
}

TEST(OperatorsTritSetTest, OperatorNOTTrue) {
	TritSet setTrue(10, Trit(True));

	ASSERT_EQ(setTrue.length(), 10);
	ASSERT_GE(setTrue.capacity(), 10);

	setTrue = ~setTrue;

	ASSERT_EQ(setTrue.length(), 10);
	ASSERT_GE(setTrue.capacity(), 10);

	for (size_t i = 0; i < 10; i++)
		ASSERT_EQ(setTrue[i], Trit(False));
}

TEST(OperatorsTritSetTest, OperatorNOTAll) {
	TritSet *setAll = tritSetFromString("FUTUTF");

	ASSERT_EQ(setAll->length(), 6);
	ASSERT_GE(setAll->capacity(), 6);

	*setAll = ~(*setAll);

	TritSet *notSet = tritSetFromString("TUFUFT");

	ASSERT_EQ((*setAll), (*notSet));

	delete setAll;
	delete notSet;
}

/** OR оператор. */
TEST(OperatorsTritSetTest, OperatorORContinually) {
	TritSet setEmpty;
	TritSet setTrue(10, True), setFalse(10, False), setUnknown(10, Unknown);

	ASSERT_EQ(setEmpty.length(), 0);
	ASSERT_GE(setEmpty.capacity(), 0);

	ASSERT_EQ(setTrue.length(), 10);
	ASSERT_GE(setTrue.capacity(), 10);

	ASSERT_EQ(setFalse.length(), 10);
	ASSERT_GE(setFalse.capacity(), 10);

	ASSERT_EQ(setUnknown.length(), 0);
	ASSERT_GE(setUnknown.capacity(), 10);

	// Empty | Empty
	TritSet set = setEmpty | setEmpty;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// True | Empty
	set = setTrue | setEmpty;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setTrue);

	// Unknown | Empty
	set = setUnknown | setEmpty;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// False | Empty
	set = setFalse | setEmpty;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// False | False
	set = setFalse | setFalse;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setFalse);

	// True | True
	set = setTrue | setTrue;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setTrue);

	// True | False
	set = setTrue | setFalse;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setTrue);

	// Unknown | Unknown
	set = setUnknown | setUnknown;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown | True
	set = setUnknown | setTrue;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setTrue);

	// Unknown | False
	set = setUnknown | setFalse;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);
}

TEST(OperatorsTritSetTest, OperatorORDifferent) {
	TritSet *setLeft = tritSetFromString("FUT");
	TritSet *setRight = tritSetFromString("TFU");
	TritSet *setLeftSmall = tritSetFromString("TU");
	TritSet *setRightSmall = tritSetFromString("UF");

	TritSet set = (*setLeft) | (*setRight);
	ASSERT_EQ(set.length(), 3);
	ASSERT_GE(set.capacity(), 3);
	TritSet *temp = tritSetFromString("TUT");
	ASSERT_EQ(set, (*temp));
	delete temp;
	set = (*setLeft) | (*setLeftSmall);
	ASSERT_EQ(set.length(), 3);
	ASSERT_GE(set.capacity(), 3);
	temp = tritSetFromString("TUT");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) | (*setRightSmall);
	ASSERT_EQ(set.length(), 2);
	ASSERT_GE(set.capacity(), 2);
	temp = tritSetFromString("TF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) | (*setLeftSmall);
	ASSERT_EQ(set.length(), 1);
	ASSERT_GE(set.capacity(), 1);
	temp = tritSetFromString("T");
	ASSERT_EQ(set, (*temp));
	delete temp;

	delete setLeft;
	delete setRight;
	delete setLeftSmall;
	delete setRightSmall;
}

/** AND оператор. */
TEST(OperatorsTritSetTest, OperatorANDContinually) {
	TritSet setEmpty;
	TritSet setTrue(10, True), setFalse(10, False), setUnknown(10, Unknown);

	ASSERT_EQ(setEmpty.length(), 0);
	ASSERT_GE(setEmpty.capacity(), 0);

	ASSERT_EQ(setTrue.length(), 10);
	ASSERT_GE(setTrue.capacity(), 10);

	ASSERT_EQ(setFalse.length(), 10);
	ASSERT_GE(setFalse.capacity(), 10);

	ASSERT_EQ(setUnknown.length(), 0);
	ASSERT_GE(setUnknown.capacity(), 10);

	// Empty & Empty
	TritSet set = setEmpty & setEmpty;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// True & Empty
	set = setTrue & setEmpty;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown & Empty
	set = setUnknown & setEmpty;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// False & Empty
	set = setFalse & setEmpty;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setFalse);

	// False & False
	set = setFalse & setFalse;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setFalse);

	// True & True
	set = setTrue & setTrue;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setTrue);

	// True & False
	set = setTrue & setFalse;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setFalse);

	// Unknown & Unknown
	set = setUnknown & setUnknown;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown & True
	set = setUnknown & setTrue;
	ASSERT_EQ(set.length(), 0);
	ASSERT_GE(set.capacity(), 0);
	ASSERT_EQ(set, setEmpty);

	// Unknown & False
	set = setUnknown & setFalse;
	ASSERT_EQ(set.length(), 10);
	ASSERT_GE(set.capacity(), 10);
	ASSERT_EQ(set, setFalse);
}

TEST(OperatorsTritSetTest, OperatorANDDifferent) {
	TritSet *setLeft = tritSetFromString("FUT");
	TritSet *setRight = tritSetFromString("TFU");
	TritSet *setLeftSmall = tritSetFromString("TU");
	TritSet *setRightSmall = tritSetFromString("UF");

	TritSet set = (*setLeft) & (*setRight);
	ASSERT_EQ(set.length(), 2);
	ASSERT_GE(set.capacity(), 2);
	TritSet *temp = tritSetFromString("FF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setLeft) & (*setLeftSmall);
	ASSERT_EQ(set.length(), 1);
	ASSERT_GE(set.capacity(), 1);
	temp = tritSetFromString("F");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) & (*setRightSmall);
	ASSERT_EQ(set.length(), 2);
	ASSERT_GE(set.capacity(), 2);
	temp = tritSetFromString("UF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	set = (*setRight) & (*setLeftSmall);
	ASSERT_EQ(set.length(), 2);
	ASSERT_GE(set.capacity(), 2);
	temp = tritSetFromString("TF");
	ASSERT_EQ(set, (*temp));
	delete temp;

	delete setLeft;
	delete setRight;
	delete setLeftSmall;
	delete setRightSmall;
}

/** Оператор сравнения. */

TEST(OperatorsTritSetTest, OperatorEquals) {
	TritSet setTrue(10, True), setFalse(10, False), setUnknown(10, Unknown);
	TritSet *setLeft = tritSetFromString("FUT");
	TritSet *setRight = tritSetFromString("UTF");
	TritSet *setSmall = tritSetFromString("TF");

	ASSERT_EQ(setTrue, setTrue);
	ASSERT_EQ(setFalse, setFalse);
	ASSERT_EQ(setUnknown, setUnknown);
	ASSERT_EQ((*setLeft), (*setLeft));
	ASSERT_EQ((*setSmall), (*setSmall));

	ASSERT_NE((*setLeft), (*setRight));
	ASSERT_NE(setTrue, setFalse);
	ASSERT_NE(setTrue, setUnknown);
	ASSERT_NE(setFalse, setUnknown);

	delete setLeft;
	delete setRight;
	delete setSmall;
}

/** Оператор получения по индексу. */
TEST(OperatorsTritSetTest, OperatorGet) {
	TritSet set;

	ASSERT_EQ((set[0] = Trit(True))[0], Trit(True));
	ASSERT_EQ((set[10] = Trit(True))[10], Trit(True));
}

/** Оператор установки по индексу. */
TEST(OperatorsTritSetTest, OperatorSet) {
	TritSet set;

	ASSERT_EQ((set[0] = Trit(Unknown)).length(), 0);
	ASSERT_EQ(set.capacity(), 0);

	ASSERT_EQ((set[10] = Trit(Unknown)).length(), 0);
	ASSERT_EQ(set.capacity(), 0);

	ASSERT_EQ((set[0] = Trit(True)).length(), 1);
	ASSERT_GE(set.capacity(), 1);

	ASSERT_EQ((set[9] = Trit(True)).length(), 10);
	ASSERT_GE(set.capacity(), 10);
}

int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}