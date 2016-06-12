#include <cstdio>
#include <iostream>

std::string::size_type pos_operator_before = 0;
std::string::size_type pos_operator_after = 0;

enum Type {
  NUMBER,
  PLUS,
  MINUS,
  TIMES,
  DIVIDE
};

typedef struct formula {
  Type type;
  double num;
  formula *left;
  formula *right;

  formula(Type type, double num) {
    this->type = type;
    this->num = num;
  }

  formula(Type type, formula* left, formula* right) {
    this->type = type;
    this->left = left;
    this->right = right;
  }
}formula;

formula* tokenize(std::string str);

double read_number(std::string str, std::string::size_type pos_start,
                   std::string::size_type pos_end) {
  std::string number_str = (pos_end == std::string::basic_string::npos) ?
    str.substr(pos_start):
    str.substr(pos_start, pos_end - pos_start);
  return std::stod(number_str, nullptr);
}

formula* make_plus_node(formula* formula, ::formula* new_formula) {
  return new ::formula(PLUS, formula, new_formula);
}

formula* make_minus_node(formula* formula, ::formula* new_formula) {
  return new ::formula(MINUS, formula, new_formula);
}

formula* make_times_node(formula* formula, ::formula* new_formula) {
  formula->right = new ::formula(TIMES, formula->right, new_formula);
  return formula;
}

formula* make_divide_node(formula* formula, ::formula* new_formula) {
  formula->right = new ::formula(DIVIDE, formula->right, new_formula);
  return formula;
}

formula* make_brackets_node(formula* formula, std::string str) {
  if (str.at(pos_operator_before) == '+') {
    pos_operator_before = pos_operator_after;
    return make_plus_node(formula, tokenize(str));
  }
  if (str.at(pos_operator_before) == '-') {
    pos_operator_before = pos_operator_after;
    return make_minus_node(formula, tokenize(str));
  }
  if (str.at(pos_operator_before) == '*') {
    pos_operator_before = pos_operator_after;
    return make_times_node(formula, tokenize(str));
  }
  if (str.at(pos_operator_before) == '/') {
    pos_operator_before = pos_operator_after;
    return make_divide_node(formula, tokenize(str));
  }
  return tokenize(str);
}

double evaluate(formula* formula) {
  double ans = 0;
  if (formula->type == PLUS)
    ans = evaluate(formula->left) + evaluate(formula->right);
  if (formula->type == MINUS)
    ans = evaluate(formula->left) - evaluate(formula->right);
  if (formula->type == TIMES)
    ans = evaluate(formula->left) * evaluate(formula->right);
  if (formula->type == DIVIDE)
    ans = evaluate(formula->left) / evaluate(formula->right);
  if (formula->type == NUMBER)
    ans = formula->num;
  return ans;
}

formula* tokenize(std::string str) {
  formula* formula = new ::formula(NUMBER, 0);
  double num;

  while (pos_operator_before + 1 < str.length()) {
    pos_operator_after = str.find_first_of("+-*/()", pos_operator_before + 1);
    if (pos_operator_after != std::string::basic_string::npos &&
        str.at(pos_operator_after) == '(') {
      formula = make_brackets_node(formula, str);
      continue;
    }
    
    if (pos_operator_after != std::string::basic_string::npos &&
        str.at(pos_operator_after) == ')' &&
        pos_operator_after - pos_operator_before == 1) {
      pos_operator_after = str.find_first_of("+-*/)", pos_operator_after+1);
    }
    num = read_number(str, pos_operator_before + 1, pos_operator_after);
    if (str.at(pos_operator_before) == '+' ||
        (str.at(pos_operator_before) == '('))
      formula = make_plus_node(formula, new ::formula(NUMBER, num));
    if (str.at(pos_operator_before) == '-')
      formula = make_minus_node(formula, new ::formula(NUMBER, num));
    if (str.at(pos_operator_before) == '*')
      formula = make_times_node(formula, new ::formula(NUMBER, num));
    if (str.at(pos_operator_before) == '/')
      formula = make_divide_node(formula, new ::formula(NUMBER, num));
    if ( pos_operator_after == std::string::basic_string::npos)
      break;
    pos_operator_before = pos_operator_after;
    if (str.at(pos_operator_after) == ')') {
      pos_operator_before++;
      break;
    }
  }
  return formula;
}

int main() {
  std::string str;
  std::cin >> str;
  str = '+' + str;
  formula* formula = tokenize(str);
  std::cout << evaluate(formula) << std::endl;
  return 0;
}
