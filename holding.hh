#ifndef HOLDING_HH
#define HOLDING_HH
#include <cstdio>
#include <iostream>

typedef unsigned int Quantity;

namespace util {
	template<typename T>
	constexpr const T& max(const T& a, const T& b) {
		return (a > b) ? a : b;
	}

	template<typename T>
	constexpr const T safe_add(const T& a, const T& b) {
		return max(max(a, b), a + b);
	}

	template<typename T>
	constexpr const T safe_sub(const T& a, const T& b) {
		return (a >= b) ? a - b : 0;
	}

	template<typename T>
	constexpr const T safe_div(const T& a, const T& b) {
		return (b == 0) ? 0 : (a / b);
	}
}

template<Quantity accNum = 0u, Quantity hsNum = 0u, Quantity exNum = 0u>
struct Company {
	static constexpr auto _accNum = accNum;
	static constexpr auto _hsNum = hsNum;
	static constexpr auto _exNum = exNum;
};

typedef Company<1, 0, 0> Accountancy;
typedef Company<0, 1, 0> Hunting_shop;
typedef Company<0, 0, 1> Exchange_office;

template<class C1, class C2>
struct add_comp {
	typedef Company<C1::_accNum + C2::_accNum, C1::_hsNum + C2::_hsNum, C1::_exNum + C2::_exNum> type;
};

template<class C1, class C2>
struct remove_comp {
	typedef Company<util::safe_sub(C1::_accNum, C2::_accNum), util::safe_sub(C1::_hsNum, C2::_hsNum),
		util::safe_sub(C1::_exNum, C2::_exNum)> type;
};

template<class C, unsigned int n>
struct multiply_comp {
	typedef Company<C::_accNum * n, C::_hsNum * n, C::_exNum * n> type;
};

template<class C, unsigned int n>
struct split_comp {
	typedef Company<util::safe_div(C::_accNum, n), util::safe_div(C::_hsNum, n),
		util::safe_div(C::_exNum, n)> type;
};

template<class C>
struct additive_expand_comp {
	typedef Company<C::_accNum + 1, C::_hsNum + 1, C::_exNum + 1> type;
};

template<class C>
struct additive_rollup_comp {
	typedef Company<util::safe_sub(C::_accNum, 1u), util::safe_sub(C::_hsNum, 1u),
		util::safe_sub(C::_exNum, 1u)> type;
};

//TODO: template Group
template<class Company>
class Group {
	public:
		typedef Company company_type;
		static constexpr company_type company = Company();

		//constructors
		Group() : qty(1), acc_val(DEF_ACC_VAL), hs_val(DEF_HS_VAL), exo_val(DEF_EXO_VAL) {}

		Group(unsigned int k) : Group{} {
			qty = k;
		}

		Group(Group<Company> const & other) : Group{} {
			qty = other.qty;
		}

		//setters
		void set_acc_val(unsigned int i) {
			acc_val = i;
		}

		void set_hs_val(unsigned int i) {
			hs_val = i;
		}

		void set_exo_val(unsigned int i) {
			exo_val = i;
		}

		//getters
		unsigned int get_size() const {
			return qty;
		}

		unsigned int get_acc_val() const {
			return acc_val;
		}

		unsigned int get_hs_val() const {
			return hs_val;
		}

		unsigned int get_exo_val() const {
			return exo_val;
		}

		unsigned int get_value() const {
			return qty * (Company::_accNum * acc_val + Company::_hsNum * hs_val
				+ Company::_exNum * exo_val);
		}

		//operators:
		//FIXME try and figure out how to get rid of this copypasta
		const Group<Company>& operator+= (const Group<Company>& op) {
			acc_val = weightedAvg(acc_val, op.acc_val, qty, op.qty);
			hs_val = weightedAvg(hs_val, op.hs_val, qty, op.qty);
			exo_val = weightedAvg(exo_val, op.exo_val, qty, op.qty);

			qty += op.qty;

			return *this;
		}

		const Group<Company> operator+ (const Group<Company>& op) {
			Group<Company> res(*this);
			res += op;
			return res;
		}

 		const Group<Company>& operator-= (const Group<Company>& op) {
			acc_val = weightedAvg(acc_val, op.acc_val, qty, op.qty, true);
			hs_val = weightedAvg(hs_val, op.hs_val, qty, op.qty, true);
			exo_val = weightedAvg(exo_val, op.exo_val, qty, op.qty, true);

			qty = util::safe_sub(qty, op.qty);

			return *this;
 		}

 		const Group<Company> operator- (const Group<Company>& op) {
			Group<Company> res(*this);
			res -= op;
			return res;
 		}

 		const Group<Company>& operator*= (const Quantity& op) {
			qty *= op;
			acc_val = util::safe_div(acc_val, op);
			hs_val = util::safe_div(hs_val, op);
			exo_val = util::safe_div(exo_val, op);

			return *this;
 		}

 		const Group<Company> operator* (const Quantity& op) {
			Group<Company> res(*this);
			res *= op;
			return res;
 		}

 		const Group<Company>& operator/= (const Quantity& op) {
			qty = util::safe_div(qty, op);
			acc_val *= op;
			hs_val *= op;
			exo_val *= op;

			return *this;
 		}

 		const Group<Company> operator/ (const Quantity &op) {
			Group<Company> res(*this);
			res /= op;
			return res;
 		}

 		template<class C1, class C2>
 		friend bool operator== (const Group<C1>& a, const Group<C2>& b);

 		template<class C1, class C2>
 		friend bool operator!= (const Group<C1>& a, const Group<C2>& b);

		template<class C1, class C2>
		friend bool operator< (const Group<C1>& a, const Group<C2>& b);

		template<class C1, class C2>
		friend bool operator<= (const Group<C1>& a, const Group<C2>& b);

		template<class C1, class C2>
		friend bool operator> (const Group<C1>& a, const Group<C2>& b);

		template<class C1, class C2>
		friend bool operator>= (const Group<C1>& a, const Group<C2>& b);

 		template<class T>
 		friend std::ostream& operator<< (std::ostream& out, const Group<T>& g);

	private:
		Quantity qty, acc_val, hs_val, exo_val;

		static constexpr Quantity DEF_ACC_VAL = 15;
		static constexpr Quantity DEF_HS_VAL = 150;
		static constexpr Quantity DEF_EXO_VAL = 50;

		const Quantity weightedAvg(const Quantity val1, const Quantity val2,
			const Quantity qty1, const Quantity qty2, const bool sub = false) {
			if (!sub)
				return util::safe_div(val1 * qty1 + val2 * qty2, qty1 + qty2);
			return util::safe_div(util::safe_sub(val1 * qty1, val2 * qty2), util::safe_sub(qty1, qty2));
		}

};

//FIXME: Those might need changing according to the forum
template<class C1, class C2>
bool operator== (const Group<C1>& a, const Group<C2>& b) {
	return typeid(C1) == typeid(C2) && a.qty == b.qty && a.hs_val == b.hs_val &&
		a.exo_val == b.exo_val;
}

template<class C1, class C2>
bool operator!= (const Group<C1>& a, const Group<C2>& b) {
	return !(a == b);
}

template<class C1, class C2>
bool operator< (const Group<C1>& a, const Group<C2>& b) {
	return a <= b && a != b;
}

template<class C1, class C2>
bool operator<= (const Group<C1>& a, const Group<C2>& b) {
	if (typeid(C1) == typeid(C2))
		return a.qty <= b.qty;
	return a.qty * C1::_exNum <= b.qty * C2::_exNum && a.qty * C1::_hsNum <= b.qty * C2::_hsNum;
}

template<class C1, class C2>
bool operator> (const Group<C1>&a, const Group<C2>& b) {
	return b < a;
}

template<class C1, class C2>
bool operator>= (const Group<C1>& a, const Group<C2>& b) {
	return b <= a;
}

template<class Company>
std::ostream& operator<< (std::ostream& out, const Group<Company>& g) {
	out << "Number of companies: " << g.qty <<"; Value: " << g.get_value() <<
	"\nAccountancies value: " << g.acc_val << ", Hunting shops value: " << g.hs_val <<
	", Exchange offices value: " << g.exo_val << "\nAccountancies: " << Company::_accNum <<
	", Hunting shops: " << Company::_hsNum << ", Exchange offices: " << Company::_exNum << "\n";
	return out;
}

//global methods

//Copies vals from s2 to s1
template<class C1, class C2>
void copyVals(Group<C1>& s1, Group<C2> const& s2) {
	s1.set_acc_val(s2.get_acc_val());
	s1.set_hs_val(s2.get_hs_val());
	s1.set_exo_val(s2.get_exo_val());
}

template<class C>
Group<typename additive_expand_comp<C>::type> const additive_expand_group(Group<C> const &s1) {
	Group<typename additive_expand_comp<C>::type> res(s1.get_size());
	copyVals(res, s1);

	return res;
}

template<class C>
Group<typename multiply_comp<C, 10>::type> const multiplicative_expand_group(Group<C> const &s1) {
	Group<typename multiply_comp<C, 10>::type> res(s1.get_size());
	copyVals(res, s1);

	return res;
}

template<class C>
Group<typename additive_rollup_comp<C>::type> const additive_rollup_group(Group<C> const &s1) {
	Group<typename additive_rollup_comp<C>::type> res(s1.get_size());
	copyVals(res, s1);

	return res;
}

template<class C>
Group<typename split_comp<C, 10>::type> const multiplicative_rollup_group(Group<C> const &s1) {
	Group<typename split_comp<C, 10>::type> res(s1.get_size());
	copyVals(res, s1);

	return res;
}

template<class C1, class C2, class C3>
bool solve_auction(Group<C1> const &g1, Group<C2> const &g2, Group<C3> const &g3) {
	return (g1 >= g2 && g1 >= g3) || (g2 >= g1 && g2 >= g3) || (g3 >= g1 && g3 >= g2);
}

#endif