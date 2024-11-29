#include <bits/stdc++.h>

std::string requirement2expression(std::string requirement, std::set<std::string> learnt)
{
	int len = requirement.length();
	std::string expression = "";
	for(int i = 0; i < len; i++)
	{
		if(!isalpha(requirement[i])) expression+=requirement[i];
		else
		{
			std::string courseCode = requirement.substr(i, 7);
			if(learnt.find(courseCode) == learnt.end()) expression += "F";
			else expression += "T";
			i += 6;
		}
	}
	return expression;
}
char cal(char x, char y, char op)
{
	if(op == '|') return (x == 'T' || y == 'T')?'T':'F';
	if(op == '&') return (x == 'T' && y == 'T')?'T':'F';
}
bool pre_request_test(std::string requirement, std::set<std::string> learnt)
{
	std::string expression = requirement2expression(requirement, learnt);
	std::stack<char> op, num;
	int len = expression.length();
	for(int i = 0; i < len; i++)
	{
		if(expression[i] == 'T' || expression[i] == 'F') num.push(expression[i]);
		else if(expression[i] == '(') op.push(expression[i]);
		else if(expression[i] == ')')
		{
			while(!op.empty() && op.top() != '(')
			{
				char x = num.top(); num.pop();
				char y = num.top(); num.pop();
				num.push(cal(x, y, op.top())); op.pop();
			}
			if(!op.empty()) op.pop();
		}
		else
		{
			while(!op.empty() && op.top() != '(')
			{
				char x = num.top(); num.pop();
				char y = num.top(); num.pop();
				num.push(cal(x, y, op.top())); op.pop();
			}
			op.push(expression[i]);
		}
	}
	while(!op.empty())
	{
		char x = num.top(); num.pop(); char y = num.top(); num.pop();
		num.push(cal(x, y, op.top())); op.pop();
	}
	return num.top() == 'T'?true:false;
}
int main()
{
	std::string requirement = "(CSC3003|CSC3002)&CSC3100&(MAT1001|MAT1002)";
	std::set<std::string> learnt;
	learnt.insert("CSC3001"); learnt.insert("CSC3100"); learnt.insert("MAT1002"); // [warning] course code has to be 3 letters and 4 digits rather than other forms such as STA2001H. 
	printf("%s\n", requirement.c_str());
	printf("%s\n", requirement2expression(requirement, learnt).c_str());
	printf("%d\n", pre_request_test(requirement, learnt));
	return 0;
}
