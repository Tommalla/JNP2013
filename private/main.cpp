#include <typeinfo>
#include <cassert>
#include <iostream>
#include <vector>

#include "../holding.hh"

using std::is_same;

typedef add_comp<Accountancy, Exchange_office>::type small_company;
typedef add_comp<multiply_comp<Hunting_shop, 10>::type, multiply_comp<Exchange_office, 20>::type>::type money;
typedef add_comp<multiply_comp<Hunting_shop, 30>::type, multiply_comp<Exchange_office, 10>::type>::type guns;

int main() {

	static_assert(is_same<add_comp<Accountancy, Exchange_office>::type, Company<1, 0, 1>>::value, "add_comp doesn't work");
	static_assert(is_same<remove_comp<Accountancy, Exchange_office>::type, Accountancy>::value, "remove_comp doesn't work[1]");
	static_assert(is_same<remove_comp<Company<1, 2, 3>, Company<1, 1, 1>>::type, Company<0, 1, 2>>::value,
		      "remove_comp doesn't work[2]");
	static_assert(is_same<multiply_comp<Company<3, 2, 1>, 2>::type, Company<6, 4, 2>>::value, "multiply_comp doesn't work[1]");
	//static_assert(!is_same<multiply_comp<Company<3, 2, 1>, -1>::type, Company<-3, -2, -1>>::value, "multiply_comp doesn't work[2]");
	static_assert(is_same<split_comp<Company<3, 2, 1>, 2>::type, Company<1, 1, 0>>::value, "split_comp doesn't work");
	static_assert(is_same<additive_expand_comp<Company<3, 2, 1>>::type, Company<4, 3, 2>>::value, "additive_expand_comp doesn't work");
	static_assert(is_same<additive_rollup_comp<Company<3, 2, 0>>::type, Company<2, 1, 0>>::value, "additive_expand_comp doesn't work");
 	Group<Accountancy> s1;
 	Group<money> s2(20);
 	Group<money> s3(20);
 	Group<Exchange_office> s4;
 	Group<guns> s5(20);
	Group<Company<2, 1, 1>> g;

	assert(g.get_value() == 50 + 150 + 30);

 	assert((s2 + s3).get_size() == 40);
 	assert((s2 += s2).get_size() == 40);
 	assert(s2.get_size() == 40);
 	assert((s2 - s3).get_size() == 20);
 	assert((s3 - s2).get_size() == 0);

 	s3.set_exo_val(25);

	std::cout << s2;
	std::cout << s3;
	std::cout << s2 + s3;
	std::cout << s2 - s3;
	std::cout << s2 * 4;
	std::cout << s2 / 2;

 	assert((s2 + s3).get_value() == 139200);
 	assert((s2 - s3).get_value() == 60000);
 	assert((s2 * 4).get_value() == 97600);
 	assert((s2 / 2).get_value() == 100000);

 	assert(s2 > s3);
 	assert(s1 != s4);
 	assert(s1 == s1);
//
 	assert(typeid(add_comp<Accountancy, Exchange_office>::type) == typeid(small_company));
 	assert(typeid(remove_comp<small_company, Accountancy>::type) == typeid(Exchange_office));
 	assert(typeid(split_comp<small_company, 2>::type) == typeid(Company<0, 0, 0>));
 	assert(typeid(additive_rollup_comp<multiply_comp<small_company, 2>::type>::type) ==
 	typeid(small_company));
 	assert(typeid(additive_expand_comp<small_company>::type) ==
 	typeid(add_comp<Hunting_shop, multiply_comp<small_company, 2>::type>::type));
//
 	assert(typeid(Group<add_comp<Accountancy, Exchange_office>::type>::company_type) ==
 	typeid(small_company));
 	assert(typeid(additive_expand_group(s1).company) ==
 	typeid(add_comp<multiply_comp<Accountancy, 2>::type,
 	       add_comp<Hunting_shop, Exchange_office>::type>::type));
 	assert(typeid(additive_rollup_group(s1).company) == typeid(Company<0,0,0>));
 	assert(typeid(multiplicative_expand_group(s1).company) ==
		typeid(multiply_comp<Accountancy, 10>::type));
 	assert(typeid(multiplicative_rollup_group(s5).company) ==
		typeid(add_comp<multiply_comp<Hunting_shop, 3>::type, Exchange_office>::type));

	assert(solve_auction(s1, s2, s5) == false);

	return 0;
}
