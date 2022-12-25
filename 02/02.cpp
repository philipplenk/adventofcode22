#include <iostream>

enum class result_type
{
	loss = 'X',
	draw = 'Y',
	win = 'Z'
};

int main(int argc, char* argv[])
{
	char a, b;
	int score = 0;
	while(std::cin>>a>>b)
	{
		a-='A';
		
		switch(result_type{b})
		{
			case result_type::loss: b = a-1; break;
			case result_type::draw: b = a; score+=3; break;
			case result_type::win: b = a+1; score+=6; break;
		}
		
		b = (b+3)%3;
		score+=b+1;
		
	}
	std::cout<<score;
}
