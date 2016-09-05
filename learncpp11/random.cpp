#include <random>
#include <iostream>

int main()
{
	std::cout<<"test default random engine:\n";
	std::default_random_engine e;
	e.seed(time(0));
	for(size_t i = 0; i < 10; i++)
		std::cout<<e()<<'\t';
	std::cout<<'\n';
	std::cout<<"test default random engine done.\n"<<std::endl;
	
	std::cout<<"test random distribution:\n";
	e.seed(time(0));
	std::uniform_int_distribution<unsigned> u(0, 9);
	for(size_t i = 0; i < 10; i++)
		std::cout<<u(e)<<'\t';
	std::cout<<'\n';
	std::cout<<"test random distribution done.\n"<<std::endl;

	std::cout<<"test real distribution:\n";
	e.seed(time(0));
	std::uniform_real_distribution<double> u2(0, 1);
	for(size_t i = 0; i < 10; i++)
		std::cout<<u2(e)<<'\t';
	std::cout<<'\n';
	std::cout<<"test real distribution done.\n"<<std::endl;
	
	std::cout<<"test normal distribution:\n";
	e.seed(time(0));
	std::normal_distribution<> n(4, 1.5);
	std::vector<unsigned> vals(9);
	for(size_t i = 0; i < 250; i++)
	{
		unsigned v = lround(n(e));
		if(v < vals.size()) vals[v]++;
	}

	for(size_t i = 0; i < vals.size(); i++)
	{
		std::cout<<i<<": "<<std::string(vals[i], '*')<<std::endl;
	}
	std::cout<<"test normal distribution done.\n"<<std::endl;

	std::cout<<"test bernoulli distribution:\n";
	e.seed(time(0));
	std::bernoulli_distribution b(0.7);
	std::vector<unsigned> bers(2);
	for(size_t i = 0; i < 200; i++)
	{
		if(b(e)) bers[1]++;
		else bers[0]++;
	}
	std::cout<<"True: "<<bers[1]<<std::endl;
	std::cout<<"False: "<<bers[0]<<std::endl;
	std::cout<<"test bernoulli distribution done.\n";
}
