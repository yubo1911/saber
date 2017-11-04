#include <boost/python.hpp>

char const* greet()
{
	return "hello, boost";
}

BOOST_PYTHON_MODULE(hello_boostpy)
{
	using namespace boost::python;
	def("greet", greet);
}
