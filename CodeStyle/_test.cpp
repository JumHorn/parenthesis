#include<iostream>
#include<string>
using namespace std;

bool Check()
{
	return true;
}

//只针对if和for加括号，暂时不管else部分
int main()
{
	if(Check())
		if(Check())
			if(Check())
				if(Check())
					Check();

	for(;Check();)
		for(;Check();)
			for(;Check();)
				for(;Check();)
					Check();

	if(Check())
		for(;Check();)
			if(Check())
				Check();
}