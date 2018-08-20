/*
Copyright(C) 2018  JumHorn <JumHorn@gmail.com>

This program is free software.you can redistribute it and / or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or any later version.
*/

#include<iostream>
#include<fstream>
#include<algorithm>
#include "formatter.h"
#include "char.h"
using namespace std;

#define OutofBounds(a,b) if(a==b)break;

Formatter::Formatter()
{
}

Formatter::~Formatter()
{
}

bool Formatter::getContent(const string& inputfile)
{
	ifstream fin(inputfile);
	if (!fin)
	{
		return false;
	}
	//construct string,list,vector waiting for optimization
	//string filestr((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	filelist.assign(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
	//filelist.assign(filestr.begin(), filestr.end());
	//filevec.assign(filestr.begin(),filestr.end());
	return true;
}

bool Formatter::beforCheck(char c) const
{
	unsigned int temp = ARRAY_SIZE(befor_check);
	return befor_check + temp != find(befor_check, befor_check + temp, c);
}

bool Formatter::afterCheck(char c) const
{
	unsigned int temp = ARRAY_SIZE(after_check);
	return after_check + temp != find(after_check, after_check + temp, c);
}

bool Formatter::compare(list<char>::iterator& start, const list<char>::iterator& end, const string& token) const
{
	if (distance(start, end) <= (int)token.length())
	{
		return false;
	}
	for (string::size_type i = 0; i < token.length(); i++)
	{
		if (*start++ != token[i])
		{
			return false;
		}
	}
	return true;
}

void Formatter::changeIfStyle(list<char>::iterator& start, const list<char>::iterator& end)
{
	list<char>::iterator walker;
	list<char>::iterator runner;
	list<char>::iterator temp;
	temp = start;
	while (temp != end)
	{
		IgnoreComments(temp, end);//ignore comments
		IgnoreApostrophe(temp, end);
		IgnoreQuotation(temp, end);
		OutofBounds(temp, end);

		runner = walker = temp;
		if (!compare(runner, end, "if"))
		{
			temp++;
			continue;
		}

		walker--;
		temp = runner;
		if (runner != end && beforCheck(*walker) && afterCheck(*runner))//handle if token
		{
			IgnoreComments(runner, end);
			IgnoreApostrophe(runner, end);
			IgnoreQuotation(runner, end);
			IgnoreParenthesis(runner, end);
			temp = runner;
			//now the runner is at the position of the right ) after if
			walker = runner;
			for (; runner != end; runner++)
			{
				IgnoreComments(runner, end);
				IgnoreApostrophe(runner, end);
				IgnoreQuotation(runner, end);
				IgnoreComments(runner, end);

				if (*runner == '{' || *runner == '#')
				{
					break;
				}
				else if (*runner == ';')
				{
					runner++;
					filelist.insert(runner, '}');
					filelist.insert(walker, '{');
					break;
				}
				else if (!beforCheck(*runner))
				{
					filelist.insert(runner, '{');
					for (walker = runner; walker != end; walker++)
					{
						IgnoreComments(walker, end);
						IgnoreApostrophe(walker, end);
						IgnoreQuotation(walker, end);
						IgnoreParenthesis(walker, end);

						if (*walker == ';')
						{
							walker++;
							IgnoreOneLineComments(walker, end);
							filelist.insert(walker, '}');
							break;
						}
						else if (*walker == '{')
						{
							IgnoreBrace(walker, end);
							filelist.insert(walker, '}');
							break;
						}
					}
					break;
				}
			}
		}
		else
		{
			continue;
		}

		if (temp != end)
		{
			temp++;
		}
	}
}

void Formatter::changeForStyle(list<char>::iterator& start, const list<char>::iterator& end)
{
	list<char>::iterator walker;
	list<char>::iterator runner;
	list<char>::iterator temp;
	temp = start;
	while (temp != end)
	{
		IgnoreComments(temp, end);
		IgnoreApostrophe(temp, end);
		IgnoreQuotation(temp, end);
		OutofBounds(temp, end);

		runner = walker = temp;
		if (!compare(runner, end, "for"))
		{
			temp++;
			continue;
		}

		walker--;
		temp = runner;
		if (runner != end && beforCheck(*walker) && afterCheck(*runner)) //handle for token
		{
			IgnoreComments(runner, end);
			IgnoreApostrophe(runner, end);
			IgnoreQuotation(runner, end);
			IgnoreParenthesis(runner, end);
			temp = runner;
			//now the runner is at the position of the right ) after for
			walker = runner;
			for (; runner != end; runner++)
			{
				IgnoreComments(runner, end);
				IgnoreApostrophe(runner, end);
				IgnoreQuotation(runner, end);
				IgnoreComments(runner, end);

				if (*runner == '{' || *runner == '#')
				{
					break;
				}
				else if (*runner == ';')
				{
					runner++;
					filelist.insert(runner, '}');
					filelist.insert(walker, '{');
					break;
				}
				else if (!beforCheck(*runner))
				{
					IgnoreComments(runner, end);
					filelist.insert(runner, '{');

					for (walker = runner; walker != end; walker++)
					{
						IgnoreComments(walker, end);
						IgnoreApostrophe(walker, end);
						IgnoreQuotation(walker, end);
						IgnoreParenthesis(walker, end);

						if (*walker == ';')
						{
							walker++;
							IgnoreOneLineComments(walker, end);
							filelist.insert(walker, '}');
							break;
						}
						else if (*walker == '{')
						{
							IgnoreBrace(walker, end);
							filelist.insert(walker, '}');
							break;
						}
					}
					break;

				}
			}
		}
		else
		{
			continue;
		}

		if (temp != end)
		{
			temp++;
		}
	}
}

void Formatter::changeElseStyle(list<char>::iterator& start, const list<char>::iterator& end)
{
	list<char>::iterator walker;
	list<char>::iterator runner;
	list<char>::iterator temp;
	temp = start;
	while (temp != end)
	{
		IgnoreComments(temp, end);
		IgnoreApostrophe(temp, end);
		IgnoreQuotation(temp, end);
		OutofBounds(temp, end);

		runner = walker = temp;
		if (!compare(runner, end, "else"))
		{
			temp++;
			continue;
		}

		walker--;
		temp = runner;
		if (runner != end && beforCheck(*walker) && afterCheck(*runner)) //handle else token
		{
			IgnoreComments(runner, end);
			IgnoreApostrophe(runner, end);
			IgnoreQuotation(runner, end);
			IgnoreParenthesis(runner, end);

			if (*runner == '{' || *runner == '#')
			{
				temp = runner;
				continue;
			}

			if (*runner == 'i')
			{
				runner++;
				if (*runner == 'f')
				{
					runner++;
					if (afterCheck(*runner))
					{
						temp = runner;
						continue;
					}
					runner--;
				}
				runner--;
			}

			walker = runner;
			for (; runner != end; runner++)
			{
				IgnoreComments(runner, end);
				IgnoreApostrophe(runner, end);
				IgnoreQuotation(runner, end);
				IgnoreComments(runner, end);

				if (*runner == '{')
				{
					break;
				}
				else if (*runner == ';')
				{
					runner++;
					filelist.insert(runner, '}');
					filelist.insert(walker, '{');
					break;
				}
				else if (!beforCheck(*runner))
				{
					IgnoreComments(runner, end);
					filelist.insert(walker, '{');

					for (walker = runner; walker != end; walker++)
					{
						IgnoreComments(walker, end);
						IgnoreApostrophe(walker, end);
						IgnoreQuotation(walker, end);
						IgnoreParenthesis(walker, end);

						if (*walker == ';')
						{
							walker++;
							IgnoreOneLineComments(walker, end);
							filelist.insert(walker, '}');
							break;
						}
						else if (*walker == '{')
						{
							IgnoreBrace(walker, end);
							filelist.insert(walker, '}');
							break;
						}
					}
					break;

				}
			}
		}
		else
		{
			continue;
		}

		if (temp != end)
		{
			temp++;
		}
	}
}

void Formatter::addElse(list<char>::iterator& start, const list<char>::iterator& end)
{
	list<char>::iterator walker;
	list<char>::iterator runner;
	list<char>::iterator temp;
	temp = start;
	while (temp != end)
	{
		IgnoreComments(temp, end);
		IgnoreApostrophe(temp, end);
		IgnoreQuotation(temp, end);
		OutofBounds(temp, end);

		runner = walker = temp;
		if (!compare(runner, end, "else"))
		{
			temp++;
			continue;
		}

		walker--;
		temp = runner;
		if (runner != end && beforCheck(*walker) && afterCheck(*runner)) //handle else token
		{
			walker = runner;
			IgnoreComments(runner, end);
			IgnoreApostrophe(runner, end);
			IgnoreQuotation(runner, end);
			IgnoreParenthesis(runner, end);

			if (*runner == '{')
			{
				runner++;
				continue;
			}

			if (*runner == 'i')
			{
				runner++;
				if (*runner == 'f')
				{
					runner++;
					if (afterCheck(*runner))
					{
						IgnoreComments(runner, end);
						IgnoreApostrophe(runner, end);
						IgnoreQuotation(runner, end);
						IgnoreParenthesis(runner, end);
						IgnoreComments(runner, end);
						IgnoreBrace(runner, end);
						temp = runner;

						IgnoreComments(runner, end);

						if (*runner == '#')
						{
							continue;
						}

						if (!compare(runner, end, "else"))
						{
							string streles = "else{}";
							filelist.insert(temp, streles.begin(), streles.end());
							temp = walker;
						}
						else
						{
							if (!afterCheck(*runner))
							{
								string streles = "else{}";
								filelist.insert(temp, streles.begin(), streles.end());
								temp = walker;
							}
							else
							{
								runner = walker;
								temp = walker;
							}
						}
						continue;
					}
					runner--;
				}
				runner--;
			}

			walker = runner;
		}
		else
		{
			continue;
		}

		if (temp != end)
		{
			temp++;
		}
	}
}

void Formatter::changeStyle(list<char>::iterator& start, const list<char>::iterator& end)
{
	changeIfStyle(start, end);
	changeForStyle(start, end);
	changeElseStyle(start, end);
	addElse(start, end);
}

void Formatter::start()
{
	changeStyle(filelist.begin(), filelist.end());
}

void Formatter::writeBack(const string& outputfile) const
{
	ofstream fout(outputfile);
	for (list<char>::const_iterator iter = filelist.begin(); iter != filelist.end(); iter++)
	{
		fout << *iter;
	}
}

template<typename T>
void Formatter::IgnoreComments(T& t, const T& end)
{
	if (t == end)
	{
		return;
	}

	while (*t == ' ' || *t == '\t' || *t == '\n' || *t == '\r')
	{
		t++;
		if (t == end)
		{
			return;
		}

	}

	if (*t == '/')
	{
		t++;
		if (*t == '/')
		{
			while (t != end && *t != '\n')t++;
			if (t != end)
			{
				t++;
				return IgnoreComments(t, end);
			}
			return;
		}
		else if (*t == '*')
		{
			while (t != end)
			{
				t++;
				if (*t != '*')
				{
					continue;
				}
				else
				{
					t++;
					if (*t == '*')
					{
						t--;
						continue;
					}
					else if (*t != '/')
					{
						continue;
					}
					else
					{
						t++;
						return IgnoreComments(t, end);
					}
				}
			}
			return;
		}
	}
	return;
}

template<typename T>
void Formatter::IgnoreOneLineComments(T& t, const T& end)
{
	if (t == end)
	{
		return;
	}

	while (*t == ' ' || *t == '\t')
	{
		t++;
		if (t == end)
		{
			return;
		}

	}

	if (*t == '/')
	{
		t++;
		if (*t == '/')
		{
			while (t != end && *t != '\n')t++;
			if (t != end)
			{
				t++;
				return;
			}
			return;
		}
		else if (*t == '*')
		{
			while (t != end)
			{
				t++;
				if (*t != '*')
				{
					continue;
				}
				else
				{
					t++;
					if (*t == '*')
					{
						t--;
						continue;
					}
					else if (*t != '/')
					{
						continue;
					}
					else
					{
						t++;
						return;
					}
				}
			}
			return;
		}
	}
	return;
}

template<typename T>
void Formatter::IgnoreApostrophe(T& t, const T& end)
{
	if (t == end)
	{
		return;
	}

	if (*t == '\'')
	{
		while (true)
		{
			t++;
			if (*t == '\\')
			{
				t++;
			}
			else if (*t == '\'')
			{
				t++;
				return;
			}
			else
			{
			}
		}
	}
	return;
}

template<typename T>
void Formatter::IgnoreQuotation(T& t, const T& end)
{
	if (t == end)
	{
		return;
	}

	if (*t == '\"')
	{
		while (true)
		{
			t++;
			if (*t == '\\')
			{
				t++;
			}
			else if (*t == '\"')
			{
				t++;
				return IgnoreQuotation(t, end);
			}
			else
			{
			}
		}
	}
	return;
}

template<typename T>
void Formatter::IgnoreParenthesis(T& t, const T& end)
{
	if (t == end)
	{
		return;
	}

	int parenthesis = 1;
	if (*t == '(')
	{
		while (true)
		{
			t++;
			IgnoreComments(t, end);
			IgnoreApostrophe(t, end);
			IgnoreQuotation(t, end);

			if (*t == '(')
			{
				parenthesis++;
			}
			else if (*t == ')')
			{
				parenthesis--;
			}
			else
			{
			}

			if (parenthesis == 0)
			{
				t++;
				return;
			}
		}
	}
	return;
}

template<typename T>
void Formatter::IgnoreBrace(T& t, const T& end)
{
	if (t == end)
	{
		return;
	}

	int parenthesis = 1;
	if (*t == '{')
	{
		while (true)
		{
			t++;
			IgnoreComments(t, end);
			IgnoreApostrophe(t, end);
			IgnoreQuotation(t, end);

			if (*t == '{')
			{
				parenthesis++;
			}
			else if (*t == '}')
			{
				parenthesis--;
			}
			else
			{
			}

			if (parenthesis == 0)
			{
				t++;
				return;
			}
		}
	}
	return;
}