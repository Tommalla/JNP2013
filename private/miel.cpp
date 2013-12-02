#include <climits>
#include <iostream>
#include <typeinfo>
#include "../holding.hh"

typedef add_comp<Accountancy, Exchange_office>::type small_company;
typedef multiply_comp<small_company, 1000>::type big_company;

int main(){
	Group<big_company> s0(100);
	s0.set_exo_val(UINT_MAX/100000);
	s0.set_acc_val(0);
	std::cout<<s0.get_value()<<std::endl;                      //jeszcze nie overflow
	s0.set_acc_val(1);
	std::cout<<s0.get_value()<<std::endl;                      //na students jest to overflow
	return 0;
}