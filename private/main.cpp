#include <typeinfo>
#include <cassert>
#include <iostream>
#include <vector>

#include "../holding.hh"

typedef add_comp<Accountancy, Exchange_office>::type small_company;
typedef add_comp<multiply_comp<Hunting_shop, 10>::type, multiply_comp<Exchange_office, 20>::type>::type money;
typedef add_comp<multiply_comp<Hunting_shop, 30>::type, multiply_comp<Exchange_office, 10>::type>::type guns;

int main() {
	Group<Accountancy> s1;
	Group<money> s2(20);
	Group<money> s3(20);
	Group<Exchange_office> s4;
	Group<guns> s5(20);

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

	assert(typeid(add_comp<Accountancy, Exchange_office>::type) == typeid(small_company));
	assert(typeid(remove_comp<small_company, Accountancy>::type) == typeid(Exchange_office));
	assert(typeid(split_comp<small_company, 2>::type) == typeid(Company<0, 0, 0>));
	assert(typeid(additive_rollup_comp<multiply_comp<small_company, 2>::type>::type) ==
	typeid(small_company));
	assert(typeid(additive_expand_comp<small_company>::type) ==
	typeid(add_comp<Hunting_shop, multiply_comp<small_company, 2>::type>::type));

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
