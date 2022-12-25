#include <iostream>

int main(int argc, char* argv[])
{
	int score = 0;
	
	char a, b;
	while(std::cin>>a>>b)
	{
		a-='A';
		b-='X';
		
		score+=b+1;
		
		const auto dist = (b-a+3)%3;
		
		if(dist==0)
			score+=3;
		else if(dist==1)
			score+=6;
	}
	std::cout<<score;
}
