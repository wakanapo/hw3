#include <cstdio>
#include <iostream>
#include <vector>

double read_number(std::string str) {
  return std::stod(str, nullptr);
}


double calculate(std::vector<char> operators, std::vector<double> numbers) {
  int i;
  for (i = 1; i < (int)operators.size(); i++) {
    if(operators[i] == '*') {
      numbers[i - 1] = numbers[i - 1] * numbers[i];
      numbers.erase(numbers.begin() + i);
      operators.erase(operators.begin() + i);
      i--;
    }
    else if (operators[i] == '/') {
      numbers[i - 1] = numbers[i - 1] / numbers[i];
      numbers.erase(numbers.begin() + i);
      operators.erase(operators.begin() + i);
      i--;
    }    
  }
  
  double ans = 0;  
  for (i = 0; i < (int)operators.size(); i++) {
    if(operators[i] == '+') {
      ans += numbers[i];
    }
    else if (operators[i] == '-') {
      ans -= numbers[i];
    }
  }
  return ans;
}

double tokenized(std::string str) {
  std::string::size_type pos_start = 0;
  std::string::size_type pos_end = 0;
  std::string numstr = "";
  std::vector<char> operators{'+'};
  std::vector<double> numbers;
  while (1) {
    pos_end = str.find_first_of("+-*/", pos_start);
    if(pos_end == pos_start) {
      pos_end = str.find_first_of("+-*/", pos_end + 1);
    }
    numstr = (pos_end == std::string::basic_string::npos) ?
      str.std::string::basic_string::substr(pos_start):
      str.std::string::basic_string::substr(pos_start, pos_end - pos_start);
    numbers.push_back(read_number(numstr));
    if (pos_end == std::string::basic_string::npos)
      break;
    operators.push_back(str.std::string::basic_string::at(pos_end));
    pos_start = pos_end + 1;
  }
  return calculate(operators, numbers);
}

int main() {
  std::string str;
  std::cin >> str;
  std::cout << tokenized(str) << std::endl;
  return 0;
}
