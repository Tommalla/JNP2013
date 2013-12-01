#ifndef HOLDING_HH
#define HOLDING_HH

typedef unsigned int Quantity;

//TODO: everything
namespace util {
	template<typename T>
	constexpr const T& max(const T& a, const T& b) {
		return (a > b) ? a : b;
	}

	template<typename T>
	constexpr const T safe_sub(const T& a, const T& b) {
		return (a >= b) ? a - b : 0;
	}
}

template<Quantity accNum = 0, Quantity hsNum = 0, Quantity exNum = 0> struct Company {
	static constexpr auto _accNum = accNum;
	static constexpr auto _hsNum = hsNum;
	static constexpr auto _exNum = exNum;
};

typedef Company<1, 0, 0> Accountancy;
typedef Company<0, 1, 0> Huntihg_shop;
typedef Company<0, 0, 1> Exchange_office;

template<class C1, class C2> struct add_comp {
	typedef Company<C1::_accNum + C2::_accNum, C1::_hsNum + C2::_hsNum, C1::_exNum + C2::_exNum> type;
};

template<class C1, class C2> struct remove_comp {
	typedef Company<util::safe_sub(C1::_accNum, C2::_accNum), util::safe_sub(C1::_hsNum, C2::_hsNum),
		util::safe_sub(C1::_exNum, C2::_exNum)> type;
};

template<class C, unsigned int n> struct multiply_comp {
	typedef Company<util::max(C::_accNum * n, C::_accNum), util::max(C::_hsNum * n, C::_hsNum),
		util::max(C::_exNum * n, C::_exNum)> type;
};

template<class C, unsigned int n> struct split_comp {
	typedef Company<C::_accNum / n, C::_hsNum / n, C::_exNum / n> type;
};

#endif