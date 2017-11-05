#include <boost/python.hpp>
#include <vector>
#include <string>
#include <sstream>
using namespace boost::python;

struct Person
{
	void set_name(std::string name) { this->name = name; }
	std::string print_info();
	void set_items(list& prices, list& discounts);
	
	
	std::string name;
	std::vector<double> item_prices;
	std::vector<double> item_discounts;
};

std::string Person::print_info()
{
	std::ostringstream info;
	double total_price = 0;
	auto it_price = item_prices.begin();
	auto it_discount = item_discounts.begin();
	while(it_price != item_prices.end() && it_discount != item_discounts.end())
	{
		total_price += (*it_price) * (1 - *it_discount);
		++it_price;
		++it_discount;
	}
	info << name <<" $"<<total_price;
	return info.str();
}

void Person::set_items(list& prices, list& discounts)
{
	for(int i = 0; i < len(prices); ++i)
	{
		double price = extract<double>(prices[i]);
		double discount = extract<double>(discounts[i]);
		item_prices.push_back(price);
		item_discounts.push_back(discount);
	}
}

BOOST_PYTHON_MODULE(person)
{
	class_<Person>("Person")
		.def("set_name", &Person::set_name)
		.def("print_info", &Person::print_info)
		.def("set_items", &Person::set_items)
	;	
}
